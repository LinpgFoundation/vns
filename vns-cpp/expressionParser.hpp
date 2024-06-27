#ifndef EXPRESSION_PARSER_HPP
#define EXPRESSION_PARSER_HPP

#include <iostream>
#include <string>
#include <cctype>
#include <cmath>
#include <utility>
#include <functional>
#include "event.hpp"
#include "number.hpp"
#include "functions.hpp"

class ExpressionParser
{
public:
    ExpressionParser(
            const std::string &expression, const std::function<bool(std::string)> &contains_variable,
            const std::function<event_data_t(std::string)> &get_variable) : index(0),
                                                                            buffer(remove_whitespace(expression)),
                                                                            contains_variable_(contains_variable),
                                                                            get_variable_(get_variable)
    {
    }

    // Parse and evaluate the expression
    Number parse();

private:
    size_t index;
    const std::string buffer;
    const std::function<bool(std::string)> &contains_variable_;
    const std::function<event_data_t(std::string)> &get_variable_;

    // Parse an expression
    Number parseExpression();

    // Parse a term
    Number parseTerm();

    // Parse a factor
    Number parseFactor();

    // Parse a number
    Number parseNumber();
};

#endif
