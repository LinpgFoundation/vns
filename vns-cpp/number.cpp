#include <stdexcept>
#include "number.hpp"

Number Number::add(const Number &o) const
{
    // If either of the values is a float, perform float addition.
    if (std::holds_alternative<float>(value) || std::holds_alternative<float>(o.value))
    {
        float result = (std::holds_alternative<float>(value) ? std::get<float>(value) : std::get<int>(value)) +
                       (std::holds_alternative<float>(o.value) ? std::get<float>(o.value) : std::get<int>(o.value));
        return Number(result);
    } else
    {
        // If both values are ints, perform int addition.
        int result = std::get<int>(value) + std::get<int>(o.value);
        return Number(result);
    }
}

Number Number::subtract(const Number &o) const
{
    // If either of the values is a float, perform float subtraction.
    if (std::holds_alternative<float>(value) || std::holds_alternative<float>(o.value))
    {
        float result = (std::holds_alternative<float>(value) ? std::get<float>(value) : std::get<int>(value)) -
                       (std::holds_alternative<float>(o.value) ? std::get<float>(o.value) : std::get<int>(o.value));
        return Number(result);
    } else
    {
        // If both values are ints, perform int addition.
        int result = std::get<int>(value) - std::get<int>(o.value);
        return Number(result);
    }
}

Number Number::multiply(const Number &o) const
{
    // If either of the values is a float, perform float multiplication.
    if (std::holds_alternative<float>(value) || std::holds_alternative<float>(o.value))
    {
        float result = (std::holds_alternative<float>(value) ? std::get<float>(value) : std::get<int>(value)) *
                       (std::holds_alternative<float>(o.value) ? std::get<float>(o.value) : std::get<int>(o.value));
        return Number(result);
    } else
    {
        // If both values are ints, perform int addition.
        int result = std::get<int>(value) * std::get<int>(o.value);
        return Number(result);
    }
}

Number Number::divide(const Number &o) const
{
    // If either of the values is a float, perform float division.
    if (std::holds_alternative<float>(value) || std::holds_alternative<float>(o.value))
    {
        float result = (std::holds_alternative<float>(value) ? std::get<float>(value) : std::get<int>(value)) /
                       (std::holds_alternative<float>(o.value) ? std::get<float>(o.value) : std::get<int>(o.value));
        return Number(result);
    } else
    {
        // Check if the division results in a non-integer value when both are ints
        int dividend = std::get<int>(value);
        int divisor = std::get<int>(o.value);
        if (dividend % divisor == 0)
        {
            // Division results in an integer value
            return Number(dividend / divisor);
        } else
        {
            // Division results in a non-integer value, perform the division as float
            return Number(static_cast<float>(dividend) / static_cast<float>(divisor));
        }
    }
}

Number Number::operate(const std::string &operation_name, const Number &o) const
{
    if (operation_name == "add")
    {
        return add(o);
    } else if (operation_name == "subtract")
    {
        return subtract(o);
    } else if (operation_name == "multiply")
    {
        return multiply(o);
    } else if (operation_name == "divide")
    {
        return divide(o);
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