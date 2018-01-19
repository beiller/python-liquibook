// Copyright (c) 2017 Object Computing, Inc.
// All rights reserved.
// See the file license.txt for licensing information.

/// @brief Command parsing helpers
#pragma once
#include <book/types.h>
#include <string>
#include <vector>

namespace orderentry
{
static const liquibook::book::Price INVALID_UNSIGNED = UINT64_MAX;
static const liquibook::book::Price INVALID_SIGNED   = INT64_MAX;

/// @brief Parse a string into tokens breaking on delimiters.
/// @param input The string to parse.
/// @param delimiter A set of delimiter characters.
/// @param[out] tokens  the tokens parsed from the string.
template<typename INPUT_STRING,typename DELIMITER_STRING,typename STRING_CONTAINER>
void split(const INPUT_STRING & input, const DELIMITER_STRING & delimiter, STRING_CONTAINER & tokens)
{
    size_t pos = 0;
    size_t end = input.length();
    while(pos < end)
    {
        auto last = input.find_first_of(delimiter, pos);
        if(last == std::string::npos)
        {
            last = end;
        }
        tokens.push_back(input.substr(pos, last - pos));
        pos = ++last;
    }
}

/// @brief Get the next token from a vector of tokens.
/// @param tokens The vector of tokens
/// @param[inout] pos the current position in the vector. Will be updated if a token is found.
/// @return the next token or an empty string if no more tokens.
std::string nextToken(const std::vector<std::string> & tokens, size_t & pos);

/// @brief Convert a string to unsigned number
/// @param input the input string.
/// @return the converted number or INVALID_UNSIGNED if the string ill-formed.
liquibook::book::Quantity toNative(const std::string & input);

/// @brief Convert a string to signed number
/// @param input the input string.
/// @return the converted number or INVALID_INT32 if the string ill-formed.
liquibook::book::QuantityDelta toNativeSigned(const std::string & input);

/// @brief Convert a string to Price
/// @param input the input string which may be a number or "MARKET", "MKT", etc.
/// @return the converted price (MARKET is 0) or INVALID_UNSIGNED if the string ill-formed.
liquibook::book::Price stringToPrice(const std::string & input);

/// @brief Remove whitespace from the start of a string (in place)
/// @param[inout] s is the string to be trimmed in place.
/// @returns a reference to the string.
std::string & ltrim(std::string &s);

/// @brief Remove whitespace from the end of a string (in place)
/// @param[inout] s is the string to be trimmed in place.
/// @returns a reference to the string.
std::string & rtrim(std::string &s);

/// @brief Remove whitespace from both ends of a string (in place)
/// @param[inout] s is the string to be trimmed in place.
/// @returns a reference to the string.
std::string & trim(std::string &s);

/// @brief Remove whitespace from the start of a constant string
/// @param s is the string to be trimmed.
/// @return the string after trimming.
std::string ltrimmed(std::string s);

/// @brief Remove whitespace from the end of a constant string
/// @param s is the string to be trimmed
/// @return the string after trimming.
std::string rtrimmed(std::string s);

/// @brief Remove whitespace from both ends of a constant string
/// @param s is the string to be trimmed.
/// @return the string after trimming.
std::string trimmed(std::string s);

}

