#ifndef EXPRESSION_PARSER_HPP
#define EXPRESSION_PARSER_HPP

#include <iostream>
#include <string>
#include <cctype>
#include <cmath>
#include <utility>
#include <functional>
#include "Event.hpp"

class ExpressionParser
{
public:
    ExpressionParser(
            std::string buffer, const std::function<bool(std::string)> &contains_variable,
            const std::function<EventValueType(std::string)> &get_variable) : index(0), buffer(std::move(buffer)),
                                                                              contains_variable_(contains_variable),
                                                                              get_variable_(get_variable)
    {
    }

    // Parse and evaluate the expression
    float parse();

private:
    size_t index;
    const std::string buffer;
    const std::function<bool(std::string)> &contains_variable_;
    const std::function<EventValueType(std::string)> &get_variable_;

    // Parse an expression
    float parseExpression();

    // Parse a term
    float parseTerm();

    // Parse a factor
    float parseFactor();

    // Parse a number
    float parseNumber();
};

#endif
