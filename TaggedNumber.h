#pragma once
#include <stdint.h>
#include <stdexcept>
#include <sstream>
#include <string>

class TaggedNumber
{
public:
    using bool_t = bool;
    using int_t = int64_t;
    using float_t = double;

    union Value
    {
        bool_t boolValue;
        int_t intValue;
        float_t floatValue;

        Value();
        ~Value();
    };

    enum class Type
    {
        Invalid,
        Boolean,
        Integer,
        Float
    };

protected:
    Value value;
    Type type;

public:
    static TaggedNumber InvalidNumber;

public:

    TaggedNumber(Type type);
    TaggedNumber(bool value);
    TaggedNumber(int value);
    TaggedNumber(int64_t value);
    TaggedNumber(float value);
    TaggedNumber(double value);
    TaggedNumber(const std::string& value);

    // タイプ取得
    Type getType() const;

    // メモリ取得
    bool_t& getBoolRaw();
    int_t& getIntRaw();
    float_t& getFloatRaw();

    // 値取得
    bool_t asBool() const;
    int_t asInt() const;
    float_t asFloat() const;

    // デバッグ用表示
    std::string toString() const;
    std::string getTypeString() const;
    static std::string getTypeString(Type type);

    template <typename T>
    static bool tryParse(const std::string& str, T& result)
    {
        std::istringstream iss(str);
        iss >> std::noskipws >> result;
        return iss.eof() && !iss.fail();
    }

public:
    // https://learn.microsoft.com/ja-jp/cpp/cpp/cpp-built-in-operators-precedence-and-associativity?view=msvc-170
    // 2
    //operator++(int)       a++
    //operator--(int)       a--
    // 3
    //operator++()          ++a
    //operator--()          --a
    //std::bit_not          ~a
    //std::logical_not      !a
    //std::negate           -a
    // 5
    //std::multiplies       a * b
    //std::divides          a / b
    //std::modulus          a % b
    // 6
    //std::plus             a + b
    //std::minus            a - b
    // 7
    //operaot>>(int b)      a >> b
    //operaot<<(int b)      a << b
    // 8
    //std::greater          a > b
    //std::greater_equal    a >= b
    //std::less             a < b
    //std::less_equal       a <= b
    // 9
    //std::equal_to         a == b
    //std::not_equal_to     a != b
    // 10
    //std::bit_and          a & b
    // 11
    //std::bit_xor          a ^ b
    // 12
    //std::bit_or           a | b
    // 13
    //std::logical_and      a && b
    // 14
    //std::logical_or       a || b
    // 15
    // operator*=           a *= b
    // operator/=           a /= b
    // operator%=           a %= b
    // operator+=           a += b
    // operator-=           a -= b
    // operator<<=          a <<= b
    // operator>>=          a >>= b
    // operator&=           a &= b
    // operator|=           a |= b
    // operator^=           a ^= b

    void operator++(int);
    void operator--(int);
    void operator++();
    void operator--();
    TaggedNumber operator~();
    TaggedNumber operator!();
    TaggedNumber operator-();
    TaggedNumber operator*(const TaggedNumber& other) const;
    TaggedNumber operator/(const TaggedNumber& other) const;
    TaggedNumber operator%(const TaggedNumber& other) const;
    TaggedNumber operator+(const TaggedNumber& other) const;
    TaggedNumber operator-(const TaggedNumber& other) const;
    TaggedNumber operator>>(const TaggedNumber& other) const;
    TaggedNumber operator<<(const TaggedNumber& other) const;
    TaggedNumber operator>(const TaggedNumber& other) const;
    TaggedNumber operator>=(const TaggedNumber& other) const;
    TaggedNumber operator<(const TaggedNumber& other) const;
    TaggedNumber operator<=(const TaggedNumber& other) const;
    TaggedNumber operator==(const TaggedNumber& other) const;
    TaggedNumber operator!=(const TaggedNumber& other) const;
    TaggedNumber operator&(const TaggedNumber& other) const;
    TaggedNumber operator^(const TaggedNumber& other) const;
    TaggedNumber operator|(const TaggedNumber& other) const;
    TaggedNumber operator&&(const TaggedNumber& other) const;
    TaggedNumber operator||(const TaggedNumber& other) const;
    TaggedNumber& operator*=(const TaggedNumber& other);
    TaggedNumber& operator/=(const TaggedNumber& other);
    TaggedNumber& operator%=(const TaggedNumber& other);
    TaggedNumber& operator+=(const TaggedNumber& other);
    TaggedNumber& operator-=(const TaggedNumber& other);
    TaggedNumber& operator<<=(const TaggedNumber& other);
    TaggedNumber& operator>>=(const TaggedNumber& other);
    TaggedNumber& operator&=(const TaggedNumber& other);
    TaggedNumber& operator|=(const TaggedNumber& other);
    TaggedNumber& operator^=(const TaggedNumber& other);
};
