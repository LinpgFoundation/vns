#ifndef OPERATION_HPP
#define OPERATION_HPP

#include <string>

struct operation
{
    constexpr static std::string_view add = "add";
    constexpr static std::string_view subtract = "subtract";
    constexpr static std::string_view multiply = "multiply";
    constexpr static std::string_view divide = "divide";
    constexpr static std::string_view mod = "mod";
    constexpr static std::string_view set = "set";

    static std::string_view get(const char symbol) {
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

    static bool has(const char symbol) {
        return symbol == '+' || symbol == '-' || symbol == '*' || symbol == '/' || symbol == '%';
    }
};

#endif