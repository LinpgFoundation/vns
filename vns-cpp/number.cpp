#include <stdexcept>
#include "number.hpp"
#include "operation.hpp"

void Number::add(const Number &o)
{
    // If either of the values is a float, perform float addition.
    if (std::holds_alternative<float>(value) || std::holds_alternative<float>(o.value))
    {
        value = (std::holds_alternative<float>(value) ? std::get<float>(value) : static_cast<float>(std::get<int>(
                value))) +
                (std::holds_alternative<float>(o.value) ? std::get<float>(o.value) : static_cast<float>(std::get<int>(
                        o.value)));
    } else
    {
        // If both values are ints, perform int addition.
        value = std::get<int>(value) + std::get<int>(o.value);
    }
}

void Number::subtract(const Number &o)
{
    // If either of the values is a float, perform float subtraction.
    if (std::holds_alternative<float>(value) || std::holds_alternative<float>(o.value))
    {
        value = (std::holds_alternative<float>(value) ? std::get<float>(value) : static_cast<float>(std::get<int>(
                value))) -
                (std::holds_alternative<float>(o.value) ? std::get<float>(o.value) : static_cast<float>(std::get<int>(
                        value)));
    } else
    {
        // If both values are ints, perform int subtraction.
        value = std::get<int>(value) - std::get<int>(o.value);
    }
}

void Number::multiply(const Number &o)
{
    // If either of the values is a float, perform float multiplication.
    if (std::holds_alternative<float>(value) || std::holds_alternative<float>(o.value))
    {
        value = (std::holds_alternative<float>(value) ? std::get<float>(value) : static_cast<float>(std::get<int>(
                value))) *
                (std::holds_alternative<float>(o.value) ? std::get<float>(o.value) : static_cast<float>(std::get<int>(
                        value)));
    } else
    {
        // If both values are ints, perform int multiplication.
        value = std::get<int>(value) * std::get<int>(o.value);
    }
}

void Number::divide(const Number &o)
{
    // If either of the values is a float, perform float division.
    if (std::holds_alternative<float>(value) || std::holds_alternative<float>(o.value))
    {
        value = (std::holds_alternative<float>(value) ? std::get<float>(value) : static_cast<float>(std::get<int>(
                value))) /
                (std::holds_alternative<float>(o.value) ? std::get<float>(o.value) : static_cast<float>(std::get<int>(
                        value)));
    } else
    {
        // Check if the division results in a non-integer value when both are ints
        const int dividend = std::get<int>(value);
        if (const int divisor = std::get<int>(o.value); dividend % divisor == 0) {
            // Division results in an integer value
            value = dividend / divisor;
        } else
        {
            // Division results in a non-integer value, perform the division as float
            value = static_cast<float>(dividend) / static_cast<float>(divisor);
        }
    }
}

void Number::mod(const Number &o)
{
    // If either of the values is a float, throw error
    if (std::holds_alternative<float>(value) || std::holds_alternative<float>(o.value))
    {
        throw std::runtime_error("You can only perform modulus operation on two integers");
    }
    // If both values are ints, perform int mod.
    value = std::get<int>(value) % std::get<int>(o.value);
}

void Number::operate(const std::string &operation_name, const Number &o)
{
    if (operation_name == operation::add)
    {
        add(o);
    } else if (operation_name == operation::subtract)
    {
        subtract(o);
    } else if (operation_name == operation::multiply)
    {
        multiply(o);
    } else if (operation_name == operation::divide)
    {
        divide(o);
    } else if (operation_name == operation::mod)
    {
        mod(o);
    } else
    {
        throw std::runtime_error("Invalid operation " + operation_name);
    }
}

bool Number::is_int() const
{
    return std::holds_alternative<int>(value);
}

int Number::get_int() const
{
    return std::get<int>(value);
}

float Number::get_float() const
{
    return std::get<float>(value);
}