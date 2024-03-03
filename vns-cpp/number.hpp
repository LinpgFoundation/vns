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

    void add(const Number &);

    void subtract(const Number &);

    void multiply(const Number &);

    void divide(const Number &);

    void mod(const Number &);

    void operate(const std::string &, const Number &);

    [[nodiscard]] bool is_int() const;

    [[nodiscard]] int get_int() const;

    [[nodiscard]] float get_float() const;

    std::variant<int, float> value;
};


#endif
