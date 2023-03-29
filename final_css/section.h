#pragma once
#include "attribute.h"
#include "LinkedList.h"
#include "selector.h"

class section
{
	LinkedList<selector> selectors_;
	LinkedList<attribute> attributes_;
	bool is_global_ = false;

public:
	section() = default;

	explicit section(const LinkedList<selector>& s, const LinkedList<attribute>& attr)
		: selectors_(s), attributes_(attr) {}

	section(const section& other)
	{

		for (auto& s : other.selectors_) this->selectors_.push_back(s);
		for (auto& a : other.attributes_) this->attributes_.push_back(a);
	}

	LinkedList<selector>& get_selectors() { return this->selectors_; }
	const LinkedList<selector>& get_selectors() const { return this->selectors_; }

	LinkedList<attribute>& get_attributes() { return this->attributes_; }
	const LinkedList<attribute>& get_attributes() const { return this->attributes_; }

	void add_selector(const selector& s) { selectors_.push_back(s); }
	void add_attribute(const attribute& a) { attributes_.push_back(a); }

	bool has_attribute(const str& a) const {
		for (const auto& key : attributes_)
		{
			if (key.get_name() == a) return true;
		}
		return false;
	}

	bool has_selector(const str& s) const
	{
		for (const auto& sel_name : selectors_)
		{
			if (sel_name.get_name() == s) return true;
		}
		return false;
	}

	void overwrite_attribute(const str& attr_name, const str& attr_val) const
	{
		for (auto& attr : attributes_)
		{
			if (attr.get_name() == attr_name) attr.set_value(attr_val);
		}
	}

	void set_to_global() { this->is_global_ = true; }

	bool is_global() const { return this->is_global_; }
};