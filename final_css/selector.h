#pragma once
#include "str.h"

class selector
{
	str name_;

public:
	explicit selector(const str& name) { this->name_ = name; }

	const str& get_name() const { return this->name_; }
};
