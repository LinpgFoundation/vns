#ifndef OPERATION_HPP
#define OPERATION_HPP

#include <string>

struct operation
{
    constexpr static const std::string add = "add";
    constexpr static const std::string subtract = "subtract";
    constexpr static const std::string multiply = "multiply";
    constexpr static const std::string divide = "divide";
    constexpr static const std::string mod = "mod";

    static std::string get(char symbol)
    {
        switch (symbol)
        {
            case '+':
                return add;
            case '-':
                return subtract;
            case '*':
                return multiply;
            case '/':
                return divide;
            case '%':
                return mod;
            default:
                throw std::runtime_error(std::string(symbol, 1) + " is not supported!");
        }
    }

    static bool has(char symbol)
    {
        return symbol == '+' || symbol == '-' || symbol == '*' || symbol == '/' || symbol == '%';
    }
};

#endif