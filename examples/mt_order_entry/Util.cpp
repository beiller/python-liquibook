// Copyright (c) 2017 Object Computing, Inc.
// All rights reserved.
// See the file license.txt for licensing information.
#include "Util.h"
#include <iostream>
#include <algorithm>
#include <functional>
#include <cctype>

namespace orderentry
{
std::string nextToken(const std::vector<std::string> & tokens, size_t & pos)
{
    if(pos < tokens.size())
    {
        return tokens[pos++];
    }
    return "";
}

liquibook::book::Quantity toNative(const std::string & input)
{
    char * end;
    liquibook::book::Quantity value = strtod(input.c_str(), &end);
    if(*end != '\0')
    {
        value = INVALID_UNSIGNED;
    }
    return value;
}

liquibook::book::QuantityDelta toNativeSigned(const std::string & input)
{
    char * end;
    liquibook::book::QuantityDelta value = strtod(input.c_str(), &end);
    if(*end != '\0')
    {
        value = INVALID_SIGNED;
    }
    return value;
}


liquibook::book::Price stringToPrice(const std::string & str)
{
    if(str == "MARKET" || str == "MKT")
    {
        return 0;
    } else
    {
        return toNative(str);
    }
}

// trim from start (in place)
std::string &  ltrim(std::string &s) {
    s.erase(s.begin(), std::find_if(s.begin(), s.end(), 
        std::not1(std::ptr_fun<int, int>(std::isspace))));
    return s;
}

// trim from end (in place)
std::string &  rtrim(std::string &s) {
    s.erase(std::find_if(s.rbegin(), s.rend(), 
        std::not1(std::ptr_fun<int, int>(std::isspace))).base(), s.end());
    return s;
}

// trim from both ends (in place)
std::string &  trim(std::string &s) 
{
    return ltrim(rtrim(s));
}

// trim from start (copying)
std::string ltrimmed(std::string s) 
{
    ltrim(s);
    return s;
}

// trim from end (copying)
std::string rtrimmed(std::string s) 
{
    rtrim(s);
    return s;
}

// trim from both ends (copying)
std::string trimmed(std::string s) 
{
    trim(s);
    return s;
}

}

