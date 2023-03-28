#pragma once
#include "str.h"

class attribute {
    str name_;
    str value_;
public:
    attribute() = default;
    attribute(const str& name, const str& value)
        : name_(name), value_(value) {}

    // getters
    str get_name() const {
        return name_;
    }
    str get_value() const {
        return value_;
    }

    // setters
    void set_name(const str& name) {
        name_ = name;
    }
    void set_value(const str& value) {
        value_ = value;
    }

    bool operator==(const attribute& other) const
    {
        return (this->name_ == other.name_);
    }
};