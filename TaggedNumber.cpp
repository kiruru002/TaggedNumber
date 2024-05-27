#include "TaggedNumber.h"
#include <functional>

TaggedNumber TaggedNumber::InvalidNumber = TaggedNumber(Type::Invalid);

TaggedNumber::Value::Value()
    : intValue(0)
{

}
TaggedNumber::Value::~Value()
{

}

TaggedNumber::TaggedNumber(Type type) : type(type), value()
{
}
TaggedNumber::TaggedNumber(bool value) : type(Type::Boolean)
{
    this->value.boolValue = value;
}
TaggedNumber::TaggedNumber(int value) : type(Type::Integer)
{
    this->value.intValue = static_cast<int64_t>(value);
}
TaggedNumber::TaggedNumber(int64_t value) : type(Type::Integer)
{
    this->value.intValue = value;
}
TaggedNumber::TaggedNumber(float value) : type(Type::Float)
{
    this->value.floatValue = static_cast<double>(value);
}
TaggedNumber::TaggedNumber(double value) : type(Type::Float)
{
    this->value.floatValue = value;
}
TaggedNumber::TaggedNumber(const std::string& value)
{
    if (tryParse(value, this->value.intValue))
    {
        type = Type::Integer;
    }
    else if (tryParse(value, this->value.floatValue))
    {
        type = Type::Float;
    }
    else if (value == std::string("true") || value == std::string("True") || value == std::string("TRUE"))
    {
        this->value.boolValue = true;
        type = Type::Boolean;
    }
    else if (value == std::string("false") || value == std::string("False") || value == std::string("FALSE"))
    {
        this->value.boolValue = false;
        type = Type::Boolean;
    }
    else
    {
        type = Type::Invalid;
    }
}

TaggedNumber::Type TaggedNumber::getType() const
{
    return this->type;
}

TaggedNumber::bool_t& TaggedNumber::getBoolRaw()
{
    return value.boolValue;
}
TaggedNumber::int_t& TaggedNumber::getIntRaw()
{
    return value.intValue;
}
TaggedNumber::float_t& TaggedNumber::getFloatRaw()
{
    return value.floatValue;
}

bool TaggedNumber::asBool() const
{
    switch (type)
    {
    case Type::Boolean:
        return value.boolValue;
    case Type::Integer:
        return value.intValue != 0;
    case Type::Float:
        return value.floatValue != 0.0;
    default:
        return false;
    }
}
int64_t TaggedNumber::asInt() const
{
    switch (type)
    {
    case Type::Boolean:
        return value.boolValue ? 1 : 0;
    case Type::Integer:
        return value.intValue;
    case Type::Float:
        return static_cast<int64_t>(value.floatValue);
    default:
        return 0;
    }
}
double TaggedNumber::asFloat() const
{
    switch (type)
    {
    case Type::Boolean:
        return value.boolValue ? 1.0 : 0.0;
    case Type::Integer:
        return static_cast<double>(value.intValue);
    case Type::Float:
        return value.floatValue;
    default:
        return 0.0;
    }
}

std::string TaggedNumber::toString() const
{
    switch (type)
    {
    case Type::Boolean:
        return value.boolValue ? std::string("true") : std::string("false");
    case Type::Integer:
        return std::to_string(value.intValue);
    case Type::Float:
        return std::to_string(value.floatValue);
    default:
        return "Unknown";
    }
}

std::string TaggedNumber::getTypeString() const
{
    return getTypeString(getType());
}
std::string TaggedNumber::getTypeString(Type type)
{
    switch (type)
    {
    case Type::Boolean:
        return std::string("boolean");
    case Type::Integer:
        return std::string("integer");
    case Type::Float:
        return std::string("float");
    default:
        return std::string("invalid type");
    }
}

template <template<class> class Op>
TaggedNumber calcArithmetic(TaggedNumber& a)
{
    switch (a.getType())
    {
    case TaggedNumber::Type::Boolean:
        return Op<TaggedNumber::bool_t>()(a.getBoolRaw());
    case TaggedNumber::Type::Integer:
        return Op<TaggedNumber::int_t>()(a.getIntRaw());
    case TaggedNumber::Type::Float:
        return Op<TaggedNumber::float_t>()(a.getFloatRaw());
    default:
        return TaggedNumber::InvalidNumber;
    }
}

template <template<class> class Op>
TaggedNumber calcArithmeticBoolean(TaggedNumber& a)
{
    return TaggedNumber(Op<TaggedNumber::bool_t>()(a.asBool()));
}

template <template<class> class Op>
TaggedNumber calcArithmeticInteger(TaggedNumber& a)
{
    return TaggedNumber(Op<TaggedNumber::int_t>()(a.asInt()));
}

template <template<class> class Op>
TaggedNumber calcArithmeticNumeric(TaggedNumber& a)
{
    switch (a.getType())
    {
    case TaggedNumber::Type::Boolean:
        return TaggedNumber::InvalidNumber;
    case TaggedNumber::Type::Integer:
        return TaggedNumber(Op<TaggedNumber::int_t>()(a.getIntRaw()));
    case TaggedNumber::Type::Float:
        return TaggedNumber(Op<TaggedNumber::float_t>()(a.getFloatRaw()));
    default:
        return TaggedNumber::InvalidNumber;
    }
}

template <template<class> class Op>
TaggedNumber calcArithmetic2(const TaggedNumber& a, const TaggedNumber& b)
{
    if (a.getType() == b.getType())
    {
        // Œ^‚ª“¯‚¶ê‡A‚»‚Ì‚Ü‚ÜŒvZ
        switch (a.getType())
        {
        case TaggedNumber::Type::Boolean:
            return TaggedNumber(Op<TaggedNumber::bool_t>()(a.asBool(), b.asBool()));
        case TaggedNumber::Type::Integer:
            return TaggedNumber(Op<TaggedNumber::int_t>()(a.asInt(), b.asInt()));
        case TaggedNumber::Type::Float:
            return TaggedNumber(Op<TaggedNumber::float_t>()(a.asFloat(), b.asFloat()));
        default:
            return TaggedNumber::InvalidNumber;
            break;
        }
    }
    else
    {
        // Œ^‚ªˆÙ‚È‚éê‡A•\Œ»‚ª‚æ‚èL‚¢•û‚ÉƒLƒƒƒXƒg‚µ‚ÄŒvZ
        if (a.getType() == TaggedNumber::Type::Float || b.getType() == TaggedNumber::Type::Float)
        {
            return TaggedNumber(Op<TaggedNumber::float_t>()(a.asFloat(), b.asFloat()));
        }
        else if (a.getType() == TaggedNumber::Type::Integer || b.getType() == TaggedNumber::Type::Integer)
        {
            return TaggedNumber(Op<TaggedNumber::int_t>()(a.asInt(), b.asInt()));
        }
        else
        {
            return TaggedNumber::InvalidNumber;
        }
    }
}

template <template<class> class Op>
TaggedNumber calcArithmetic2Boolean(const TaggedNumber& a, const TaggedNumber& b)
{
    return TaggedNumber(Op<TaggedNumber::bool_t>()(a.asBool(), b.asBool()));
}

template <template<class> class Op>
TaggedNumber calcArithmetic2Integer(const TaggedNumber& a, const TaggedNumber& b)
{
    return TaggedNumber(Op<TaggedNumber::int_t>()(a.asInt(), b.asInt()));
}

template <template<class> class Op>
TaggedNumber calcArithmetic2Numeric(const TaggedNumber& a, const TaggedNumber& b)
{
    if (a.getType() == b.getType())
    {
        switch (a.getType())
        {
        case TaggedNumber::Type::Integer:
            return TaggedNumber(Op<TaggedNumber::int_t>()(a.asInt(), b.asInt()));
        case TaggedNumber::Type::Float:
            return TaggedNumber(Op<TaggedNumber::float_t>()(a.asFloat(), b.asFloat()));
        case TaggedNumber::Type::Boolean:
        default:
            return TaggedNumber::InvalidNumber;
            break;
        }
    }
    else
    {
        if (a.getType() == TaggedNumber::Type::Boolean || b.getType() == TaggedNumber::Type::Boolean)
        {
            return TaggedNumber::InvalidNumber;
        }
        else if (a.getType() == TaggedNumber::Type::Float || b.getType() == TaggedNumber::Type::Float)
        {
            return TaggedNumber(Op<TaggedNumber::float_t>()(a.asFloat(), b.asFloat()));
        }
        else if (a.getType() == TaggedNumber::Type::Integer || b.getType() == TaggedNumber::Type::Integer)
        {
            return TaggedNumber(Op<TaggedNumber::int_t>()(a.asInt(), b.asInt()));
        }
        else
        {
            return TaggedNumber::InvalidNumber;
        }
    }
}


void TaggedNumber::operator++(int)
{
    switch (type)
    {
    case Type::Boolean:
        break;
    case Type::Integer:
        this->value.intValue += 1;
        break;
    case Type::Float:
        this->value.floatValue += 1.0;;
        break;
    default:
        break;
    }
    return;
}
void TaggedNumber::operator--(int)
{
    switch (type)
    {
    case Type::Boolean:
        break;
    case Type::Integer:
        this->value.intValue -= 1;
        break;
    case Type::Float:
        this->value.floatValue -= 1.0;;
        break;
    default:
        break;
    }
    return;
}
void TaggedNumber::operator++()
{
    return (*this)++;
}
void TaggedNumber::operator--()
{
    return (*this)--;
}
TaggedNumber TaggedNumber::operator~()
{
    return calcArithmeticInteger<std::bit_not>(*this);
}
TaggedNumber TaggedNumber::operator!()
{
    return calcArithmetic<std::logical_not>(*this);
}
TaggedNumber TaggedNumber::operator-()
{
    return calcArithmeticNumeric<std::negate>(*this);
}
TaggedNumber TaggedNumber::operator*(const TaggedNumber& other) const
{
    return calcArithmetic2<std::multiplies>(*this, other);
}
TaggedNumber TaggedNumber::operator/(const TaggedNumber& other) const
{
    return calcArithmetic2Numeric<std::divides>(*this, other);
}
TaggedNumber TaggedNumber::operator%(const TaggedNumber& other) const
{
    return calcArithmetic2Integer<std::modulus>(*this, other);
}
TaggedNumber TaggedNumber::operator+(const TaggedNumber& other) const
{
    return calcArithmetic2<std::plus>(*this, other);
}
TaggedNumber TaggedNumber::operator-(const TaggedNumber& other) const
{
    return calcArithmetic2<std::minus>(*this, other);
}
TaggedNumber TaggedNumber::operator>>(const TaggedNumber& other) const
{
    return TaggedNumber(asInt() >> other.asInt());
}
TaggedNumber TaggedNumber::operator<<(const TaggedNumber& other) const
{
    return TaggedNumber(asInt() << other.asInt());
}
TaggedNumber TaggedNumber::operator>(const TaggedNumber& other) const
{
    return calcArithmetic2<std::greater>(*this, other);
}
TaggedNumber TaggedNumber::operator>=(const TaggedNumber& other) const
{
    return calcArithmetic2<std::greater_equal>(*this, other);
}
TaggedNumber TaggedNumber::operator<(const TaggedNumber& other) const
{
    return calcArithmetic2<std::less>(*this, other);
}
TaggedNumber TaggedNumber::operator<=(const TaggedNumber& other) const
{
    return calcArithmetic2<std::less_equal>(*this, other);
}
TaggedNumber TaggedNumber::operator==(const TaggedNumber& other) const
{
    return calcArithmetic2<std::equal_to>(*this, other);
}
TaggedNumber TaggedNumber::operator!=(const TaggedNumber& other) const
{
    return calcArithmetic2<std::not_equal_to>(*this, other);
}
TaggedNumber TaggedNumber::operator&(const TaggedNumber& other) const
{
    return calcArithmetic2Integer<std::bit_and>(*this, other);
}
TaggedNumber TaggedNumber::operator^(const TaggedNumber& other) const
{
    return calcArithmetic2Integer<std::bit_xor>(*this, other);
}
TaggedNumber TaggedNumber::operator|(const TaggedNumber& other) const
{
    return calcArithmetic2Integer<std::bit_or>(*this, other);
}
TaggedNumber TaggedNumber::operator&&(const TaggedNumber& other) const
{
    return calcArithmetic2<std::logical_and>(*this, other);
}
TaggedNumber TaggedNumber::operator||(const TaggedNumber& other) const
{
    return calcArithmetic2<std::logical_or>(*this, other);
}
TaggedNumber& TaggedNumber::operator*=(const TaggedNumber& other)
{
    return (*this = *this * other);
}
TaggedNumber& TaggedNumber::operator/=(const TaggedNumber& other)
{
    return (*this = *this / other);
}
TaggedNumber& TaggedNumber::operator%=(const TaggedNumber& other)
{
    return (*this = *this % other);
}
TaggedNumber& TaggedNumber::operator+=(const TaggedNumber& other)
{
    return (*this = *this + other);
}
TaggedNumber& TaggedNumber::operator-=(const TaggedNumber& other)
{
    return (*this = *this - other);
}
TaggedNumber& TaggedNumber::operator<<=(const TaggedNumber& other)
{
    return (*this = *this << other);
}
TaggedNumber& TaggedNumber::operator>>=(const TaggedNumber& other)
{
    return (*this = *this >> other);
}
TaggedNumber& TaggedNumber::operator&=(const TaggedNumber& other)
{
    return (*this = *this & other);
}
TaggedNumber& TaggedNumber::operator|=(const TaggedNumber& other)
{
    return (*this = *this | other);
}
TaggedNumber& TaggedNumber::operator^=(const TaggedNumber& other)
{
    return (*this = *this ^ other);
}
