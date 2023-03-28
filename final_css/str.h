
#pragma once

#include <cstring>
#include <iostream>
#include <cctype>

#include "Vector.h"

class str {
    char* data_;
    size_t length_;
public:
    str() : data_(new char[1]), length_(0)
    {
        data_[0] = '\0';
    }

    str(const char* cstr) : data_(nullptr), length_(0)
    {
        if (cstr) {
            const size_t len = strlen(cstr);
            char* strData = new char[len + 1];

            for (size_t i = 0, j = 0; i < len; i++) {
                if (cstr[i] != '\n') {
                    strData[j++] = cstr[i];
                    length_++;
                }
            }

            strData[length_] = '\0';
            data_ = strData;
        }
        else {
            data_ = new char[1];
            data_[0] = '\0';
        }
    }

    str& operator=(const str& other) {
        if (this != &other) {
            delete[] data_;
            length_ = other.length_;
            data_ = new char[length_ + 1];
            memcpy(data_, other.data_, length_ + 1);
        }
        return *this;
    }

    char back() const
    {
        return data_[length_ - 1];
    }

    char front() const
    {
        return data_[0];
    }

    str(const str& other) : data_(nullptr), length_(other.length_) {
        if (other.data_) {
            data_ = new char[length_ + 1];
            memcpy(data_, other.data_, length_ + 1);
        }
    }

    // Destructor
    ~str() {
        delete[] data_;
    }

    int find(const char c) const
    {
        for (size_t i = 0; i < length_; i++)
        {
            if (data_[i] == c) return i;
        }
        return -1;
    }

    int find(const char c, const size_t start) const
    {
        for (size_t i = start; i < length_; i++)
        {
            if (data_[i] == c) return i;
        }
        return -1;
    }

    str substr(const int start, const int length) const {
        if (start < 0 || start > length_ || length < 0 || start + length > length_) {
            // invalid substring parameters
            return str("");
        }
        char* substrData = new char[length + 1];
        strncpy(substrData, data_ + start, length);
        substrData[length] = '\0';
        str substr(substrData);
        delete[] substrData;
        return substr;
    }

    str substr(const int start) const {
        if (start < 0 || start >= length_) {
            return str("");
        }

        return str(data_ + start);
    }

    bool operator==(const str& other) const
    {
        if (length_ != other.length_) return false;
        for (size_t i = 0; i < length_; i++)
        {
            if (data_[i] != other.data_[i]) return false;
        }
        return true;
    }

    const char& operator[](const int index) const {
        if (index < 0 || index >= length_) throw "Out of bounds";
        return data_[index];
    }

    bool is_whitespace(char c) {
        return c == ' ' || c == '\t' || c == '\n' || c == '\r' || c == '\f' || c == '\v';
    }

    void trim() {
        if (!data_) {
            return; // exit early if data_ is null
        }
        // Determine the new length of the string after trimming whitespace from the end
        size_t new_length = length_;
        while (new_length > 0 && std::isspace(data_[new_length - 1])) {
            new_length--;
        }

        // Determine the new length of the string after trimming whitespace from the beginning
        size_t start_index = 0;
        while (start_index < new_length && std::isspace(data_[start_index])) {
            start_index++;
        }

        // Allocate a new buffer for the trimmed string
        char* new_data = new char[new_length - start_index + 1];

        // Copy the trimmed string into the new buffer
        std::memcpy(new_data, data_ + start_index, new_length - start_index);
        new_data[new_length - start_index] = '\0';

        // Free the old buffer and update the object's data and length
        delete[] data_;
        data_ = new_data;
        length_ = new_length - start_index;
    }

    char* c_str() const {
        if (this == nullptr || data_ == nullptr) {
            return nullptr;
        }
        return data_;
    }

    size_t length() const {
        return length_;
    }

    void pop_back() {
        if (length_ > 0) {
            data_[length_ - 1] = '\0';
            length_--;
        }
    }

    Vector<str> split(const char delimiter)
    {
        Vector<str> result;
        int start = 0;
        int pos = find(delimiter);

        while (pos != -1)
        {
            auto element = substr(start, pos - start);
            if (element.length() != 0) result.push_back(element);
            start = pos + 1;
            pos = find(delimiter, start);
        }

        auto element = substr(start);
        if (element.length() != 0) result.push_back(element);
        return result;
    }

    friend std::ostream& operator<<(std::ostream& os, const str& s) {
        os << s.data_;
        return os;
    }

    bool operator==(const char* str) const
    {
        if (this == nullptr || data_ == nullptr) return false;  // Add null pointer check for this and data_
        if (length_ != strlen(str)) return false;
        for (size_t i = 0; i < length_; i++)
        {
            if (data_[i] != str[i]) return false;
        }
        return true;
    }
};