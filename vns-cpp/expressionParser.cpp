#include "expressionParser.hpp"

// Parse and evaluate the expression
Number ExpressionParser::parse()
{
    index = 0;
    return parseExpression();
}


// Parse an expression
Number ExpressionParser::parseExpression()
{
    Number result = parseTerm();
    while (index < buffer.size())
    {
        if (buffer[index] == '+')
        {
            ++index;
            result.add(parseTerm());
        } else if (buffer[index] == '-')
        {
            ++index;
            result.subtract(parseTerm());
        } else
        {
            break;
        }
    }
    return result;
}

// Parse a term
Number ExpressionParser::parseTerm()
{
    Number result = parseFactor();
    while (index < buffer.size())
    {
        if (buffer[index] == '*')
        {
            ++index;
            result.multiply(parseFactor());
        } else if (buffer[index] == '/')
        {
            ++index;
            result.divide(parseFactor());
        } else if (buffer[index] == '%')
        {
            ++index;
            result.mod(parseFactor());
        } else
        {
            break;
        }
    }
    return result;
}

// Parse a factor
Number ExpressionParser::parseFactor()
{
    if (buffer[index] == '(')
    {
        ++index; // Consume '('
        Number result = parseExpression();
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
Number ExpressionParser::parseNumber()
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
    number number_t;
    // if the number extracted is the name of a variable
    if (contains_variable_(data_s))
    {
        EventValueType value = get_variable_(data_s);
        if (std::holds_alternative<int>(value))
        {
            number_t = std::get<int>(value);
        } else if (std::holds_alternative<float>(value))
        {
            number_t = std::get<float>(value);
        } else
        {
            throw std::runtime_error("Cannot add " + data_s + " because it is not a number");
        }
    } else
    {
        size_t pos;
        // it is a float?
        if (data_s.find('.') != std::string::npos)
        {
            number_t = std::stof(data_s, &pos);
        }
            // it is an integer?
        else
        {
            number_t = std::stoi(data_s, &pos);
        }
        // Not all characters were processed, throw an error
        if (pos != data_s.size())
        {
            throw std::runtime_error("String contains non-integer characters");
        }
    }

    return Number(number_t);
}
