
#pragma once

#include <cstring>
#include <iostream>
#include <cctype>

#include "Vector.h"

class str
{
	char* buf_ = nullptr;
	unsigned int size_ = 0;

public:
	str() = default;

	explicit str(const char* buffer)
	{
		size_ = std::strlen(buffer);
		buf_ = new char[size_ + 1];
		strncpy_s(buf_, size_ + 1, buffer, size_);
		buf_[size_] = '\0';
	}

	str(const str& other)
	{
		size_ = other.size_;
		buf_ = new char[size_ + 1];
		strncpy_s(buf_, size_ + 1, other.buf_, size_);
	}

	str& operator=(const str& other)
	{
		cleanup();

		size_ = other.size_;
		buf_ = new char[size_ + 1];
		strncpy_s(buf_, size_ + 1, other.buf_, size_);
		buf_[size_] = '\0';
		return *this;
	}

	str (str&& old) noexcept
	{
		cleanup();
		size_ = old.size_;
		buf_ = old.buf_;

		delete[] old.buf_;
	}

	~str() { cleanup(); }

	str& operator=(str&& old) noexcept
	{
		cleanup();
		size_ = old.size_;
		buf_ = old.buf_;

		delete[] old.buf_;
		return *this;
	}

	str operator+(const str& other) const
	{
		str s;
		s.size_ = this->size_ + other.size_;
		s.buf_ = new char[s.size_ + 1];

		strncpy_s(s.buf_, this->size_ + 1, this->buf_, this->size_);
		strncpy_s(s.buf_ + this->size_, other.size_ + 1, other.buf_, other.size_);

		s.buf_[s.size_] = '\0';

		return s;
	}

	str& operator+=(const char c)
	{
		const unsigned int new_size = size_ + 1;
		char* new_buf = new char[new_size + 1];

		if (buf_)
		{
			memcpy(new_buf, buf_, size_);
			delete[] buf_;
		}

		new_buf[size_] = c;
		new_buf[new_size] = '\0';

		buf_ = new_buf;
		size_ = new_size;

		buf_[size_] = '\0';

		return *this;
	}

	bool empty() const { return size_ == 0; }

	bool readline()
	{
		this->cleanup();
		char c;

		while (std::cin.get(c))
		{
			if (c == '\n') return true;

			if (c < ' ') continue;

			*this += c;
		}
		buf_[size_] = '\0';
		return !this->empty();
	}

	bool operator==(const str& other) const
	{
		if (size_ != other.size_) return false;
		for (int i = 0; i < size_; i++)
		{
			if (buf_[i] != other.buf_[i]) return false;
		}
		return true;
	}

	bool operator==(const char* other) const
	{
		if (size_ != std::strlen(other)) return false;
		return std::strcmp(buf_, other) == 0;
	}

	friend std::ostream& operator<<(std::ostream& out, const str& s)
	{
		out << s.c_str();
		return out;
	}

	[[nodiscard]] char back() const
	{
		if (size_ == 0) throw std::out_of_range("Out of range");
		return buf_[size_ - 1];
	}

	[[nodiscard]] char front() const
	{
		if (size_ == 0) throw std::out_of_range("Out of range");
		return buf_[0];
	}

	void pop_back()
	{
		if (size_ > 0)
		{
			--size_;
			buf_[size_] = '\0';
		}
	}

	void trim()
	{
		// Remove leading whitespaces
		int i = 0;
		while (isspace(buf_[i])) {
			++i;
		}

		if (i > 0) {
			size_ -= i;
			memmove(buf_, buf_ + i, size_ + 1);
		}

		// Remove trailing whitespaces
		i = size_ - 1;
		while (i >= 0 && isspace(buf_[i])) {
			--i;
		}

		size_ = i + 1;
		buf_[size_] = '\0';
	}

	[[nodiscard]] int find(const char c) const
	{
		for (int i = 0; i < size_; i++)
		{
			if (buf_[i] == c) return i;
		}
		return -1;
	}

	[[nodiscard]] int find(const char c, const int start) const
	{
		for (int i = start; i < size_; i++)
		{
			if (buf_[i] == c) return i;
		}
		return -1;
	}

	[[nodiscard]] Vector<str> split (const char c) const
	{
		Vector<str> result;
		int start = 0;
		int pos = find(c);

		while (pos != -1)
		{
			auto element = substr(start, pos - start);
			element.trim();
			if (element.length() != 0) result.push_back(element);
			start = pos + 1;
			pos = find(c, start);
		}

		auto element = substr(start);
		element.trim();
		if (element.length() != 0) result.push_back(element);
		return  result;
	}

	[[nodiscard]] str substr(const int start, const int len) const
	{
		int max_len = size_ - start;
		int sub_len = (len > 0 && len <= max_len) ? len : max_len;
		char* sub_buf = new char[sub_len + 1];

		strncpy_s(sub_buf, sub_len + 1, buf_ + start, sub_len);

		str sub_str(sub_buf);
		sub_str.trim();
		delete[] sub_buf;

		return sub_str;
	}

	[[nodiscard]] str substr(const int start) const
	{
		if (start < 0 || start >= size_) return str("");
		return str(buf_ + start);
	}

	[[nodiscard]] unsigned int length() const { return size_; }

	[[nodiscard]] const char* c_str() const { return buf_; }

private:
	void cleanup()
	{
		delete[] buf_;
		buf_ = nullptr;
		size_ = 0;
	}
};