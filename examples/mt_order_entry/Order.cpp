// Copyright (c) 2017 Object Computing, Inc.
// All rights reserved.
// See the file license.txt for licensing information.
#include "Order.h"
#include <sstream>
#include <iomanip>
#include <iostream>

namespace orderentry
{

Order::Order(const std::string & id,
    bool buy_side,
    liquibook::book::Quantity quantity,
    std::string & symbol,
    liquibook::book::Price price,
    liquibook::book::Price stopPrice,
    bool aon,
    bool ioc,
    const std::string & extern_order_id)
    : id_(id)
    , buy_side_(buy_side)
    , symbol_(symbol)
    , quantity_(quantity)
    , price_(price)
    , stopPrice_(stopPrice)
    , ioc_(ioc)
    , aon_(aon)
    , extern_order_id_(extern_order_id)
    , quantityFilled_(0)
    , quantityOnMarket_(0)
    , fillCost_(0)
    , verbose_(false)

{
}

std::string 
Order::order_id() const
{
    return id_;
}

std::string 
Order::extern_order_id() const
{
    return extern_order_id_;
}

bool 
Order::is_limit() const
{
    return price() != 0;
}

bool 
Order::is_buy() const
{
    return buy_side_;
}

bool 
Order::all_or_none() const
{
    return aon_;
}

bool 
Order::immediate_or_cancel() const
{
    return ioc_;
}

std::string 
Order::symbol() const
{
   return symbol_;
}

liquibook::book::Price 
Order::price() const
{
    return price_;
}

liquibook::book::Quantity 
Order::order_qty() const
{
    return quantity_;
}


liquibook::book::Price 
Order::stop_price() const
{
    return stopPrice_;
}

liquibook::book::Quantity 
Order::quantityOnMarket() const
{
    return quantityOnMarket_;
}

liquibook::book::Quantity 
Order::quantityFilled() const
{
    return quantityFilled_;
}

liquibook::book::Price
Order::fillCost() const
{
    return fillCost_;
}


const Order::History & 
Order::history() const
{
    return history_;
}

const Order::StateChange & 
Order::currentState() const
{
    return history_.back();
}


Order & 
Order::verbose(bool verbose)
{
    verbose_ = verbose;
    return *this;
}

bool
Order::isVerbose() const
{
    return verbose_;
}

void 
Order::onSubmitted()
{
    std::stringstream msg;
    msg << (is_buy() ? "BUY " : "SELL ") << quantity_ << ' ' << symbol_ << " @";
    if( price_ == 0)
    {
        msg << "MKT";
    }
    else
    {
        msg << price_;
    }
    history_.emplace_back(Submitted, msg.str());
}

void 
Order::onAccepted()
{
    quantityOnMarket_ = quantity_;
    history_.emplace_back(Accepted);
}

void 
Order::onRejected(const char * reason)
{
    history_.emplace_back(Rejected, reason);
}

void 
Order::onFilled(
    liquibook::book::Quantity fill_qty, 
    liquibook::book::Cost fill_cost)
{
    quantityOnMarket_ -= fill_qty;
    fillCost_ += fill_cost;

    std::stringstream msg;
    msg << fill_qty << " for " << fill_cost;
    if(quantityOnMarket_ > 0) {
        history_.emplace_back(PartialFilled, msg.str());    
    } else {
        history_.emplace_back(Filled, msg.str());    
    }
}

void 
Order::onCancelRequested()
{
    history_.emplace_back(CancelRequested);
}

void 
Order::onCancelled()
{
    quantityOnMarket_ = 0;
    history_.emplace_back(Cancelled);
}

void 
Order::onCancelRejected(const char * reason)
{
    history_.emplace_back(CancelRejected, reason);
}

void 
Order::onReplaceRequested(
    const liquibook::book::QuantityDelta& size_delta, 
    liquibook::book::Price new_price)
{
    std::stringstream msg;
    if(size_delta != liquibook::book::SIZE_UNCHANGED)
    {
        msg << "Quantity change: " << size_delta << ' ';
    }
    if(new_price != liquibook::book::PRICE_UNCHANGED)
    {
        msg << "New Price " << new_price;
    }
    history_.emplace_back(ModifyRequested, msg.str());
}

void 
Order::onReplaced(const liquibook::book::QuantityDelta& size_delta, 
    liquibook::book::Price new_price)
{
    std::stringstream msg;
    if(size_delta != liquibook::book::SIZE_UNCHANGED)
    {
        quantity_ += size_delta;
        quantityOnMarket_ += size_delta;
        msg << "Quantity change: " << size_delta << ' ';
    }
    if(new_price != liquibook::book::PRICE_UNCHANGED)
    {
        price_ = new_price;
        msg << "New Price " << new_price;
    }
    history_.emplace_back(Modified, msg.str());
}

void 
Order::onReplaceRejected(const char * reason)
{
    history_.emplace_back(ModifyRejected, reason);
}

std::ostream & operator << (std::ostream & out, const Order::StateChange & event)
{
    //out << "{";
    switch(event.state_)
    {
    case Order::Submitted:
        out << "Submitted";
        break;
    case Order::Rejected: 
        out << "Rejected"; 
        break;
    case Order::Accepted:
        out << "Accepted";
        break;
    case Order::ModifyRequested:
        out << "ModifyRequested";
        break;
    case Order::ModifyRejected:
        out << "ModifyRejected";
        break;
    case Order::Modified:
        out << "Modified";
        break;
    case Order::PartialFilled:
        out << "PartialFilled";
        break;
    case Order::Filled: 
        out << "Filled"; 
        break;
    case Order::CancelRequested:
        out << "CancelRequested";
        break;
    case Order::CancelRejected:
        out << "CancelRejected";
        break;
    case Order::Cancelled: 
        out << "Cancelled"; 
        break;
    case Order::Unknown:
        out << "Unknown";
        break;
    }
    //out << event.description_;
    //out << "}";
    return out;
}

std::ostream & operator << (std::ostream & out, const Order & order)
{
    out.precision(8);
    out << std::fixed;
    out << "{\"order_id\": \"" << order.extern_order_id() << "\""; 
    out << ", \"side\": \"" << (order.is_buy() ? "BUY" : "SELL") << "\"";
    out << ", \"quantity\": \"" << order.order_qty() << "\"";
    out << ", \"symbol\": \"" << order.symbol() << "\"";
    if(order.price() == 0)
    {
        out << ", \"price\": \"MKT\"";
    }
    else
    {
        out << ", \"price\": \""<< order.price() << "\"";
    }

    if(order.stop_price() != 0)
    {
        out << ", \"stop\": \""<< order.stop_price() << "\"";
    }

    if(order.all_or_none())
    {
        out << ", \"aon\": true";
    } else {
        out << ", \"aon\": false";
    }

    if(order.immediate_or_cancel())
    {
        out << ", \"ioc\": true";
    } else {
        out << ", \"ioc\": false";
    }

    auto onMarket = order.quantityOnMarket();
    if(onMarket != 0)
    {
        //out << " Open: " << onMarket;
        out << ", \"open\": \""<< onMarket << "\"";
    }

    auto filled = order.quantityFilled();
    if(filled != 0)
    {
        //out << " Filled: " << filled;
        out << ", \"filled\": \""<< filled << "\"";
    }

    auto cost = order.fillCost();
    if(cost != 0)
    {
        //out << " Cost: " << cost;
        out << ", \"cost\": \""<< cost << "\"";
    }

    out << ", \"status\": \""<< order.currentState() << "\"";
    //out << " Last Event:" << order.currentState();

    out << '}';
   
   return out;
}


}
