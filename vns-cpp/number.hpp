#ifndef NUMBER_HPP
#define NUMBER_HPP

#include <variant>
#include <string>

using number = std::variant<int, float>;

struct Number
{

    explicit Number(const number &data) : value(data)
    {
    }

    [[nodiscard]] Number add(const Number &) const;

    [[nodiscard]] Number subtract(const Number &) const;

    [[nodiscard]] Number multiply(const Number &) const;

    [[nodiscard]] Number divide(const Number &) const;

    [[nodiscard]] Number operate(const std::string &, const Number &) const;

    [[nodiscard]] bool is_int() const;

    [[nodiscard]] int get_int() const;

    [[nodiscard]] float get_float() const;

    const std::variant<int, float> value;
};


#endif
