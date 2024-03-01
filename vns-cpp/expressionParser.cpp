#include "expressionParser.hpp"

// Parse and evaluate the expression
float ExpressionParser::parse()
{
    index = 0;
    return parseExpression();
}


// Parse an expression
float ExpressionParser::parseExpression()
{
    float result = parseTerm();
    while (index < buffer.size())
    {
        if (buffer[index] == '+')
        {
            ++index;
            result += parseTerm();
        } else if (buffer[index] == '-')
        {
            ++index;
            result -= parseTerm();
        } else
        {
            break;
        }
    }
    return result;
}

// Parse a term
float ExpressionParser::parseTerm()
{
    float result = parseFactor();
    while (index < buffer.size())
    {
        if (buffer[index] == '*')
        {
            ++index;
            result *= parseFactor();
        } else if (buffer[index] == '/')
        {
            ++index;
            float divisor = parseFactor();
            if (divisor != 0.0)
                result /= divisor;
            else
                throw std::runtime_error("Division by zero.");
        } else
        {
            break;
        }
    }
    return result;
}

// Parse a factor
float ExpressionParser::parseFactor()
{
    if (buffer[index] == '(')
    {
        ++index; // Consume '('
        float result = parseExpression();
        if (buffer[index] != ')')
        {
            throw std::runtime_error("Expected ')'.");
        }
        ++index; // Consume ')'
        return result;
    } else
    {
        return parseNumber();
    }
}

// Parse a number
float ExpressionParser::parseNumber()
{
    size_t start = index;
    // check the tag for global or persistent variable
    if (buffer[index] == '@' || buffer[index] == '&')
    {
        ++index;
    }
    while (index < buffer.size() &&
           (isdigit(buffer[index]) || isalpha(buffer[index]) || buffer[index] == '_' || buffer[index] == '.'))
    {
        ++index;
    }
    const std::string data_s = buffer.substr(start, index - start);
    float number;
    // if the number extracted is the name of a variable
    if (contains_variable_(data_s))
    {
        EventValueType value = get_variable_(data_s);
        if (std::holds_alternative<int>(value))
        {
            number = static_cast<float>(std::get<int>(value));
        } else if (std::holds_alternative<float>(value))
        {
            number = std::get<float>(value);
        } else
        {
            throw std::runtime_error("Cannot add " + data_s + " because it is not a number");
        }
    }
        // or if it is just a number
    else
    {
        number = std::stof(data_s);
    }
    return number;
}
