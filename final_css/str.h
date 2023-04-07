#pragma once

#include <cstring>
#include <iostream>
#include <cctype>

#include "Vector.h"

#define _CRT_SECURE_NO_WARNINGS

class str {

    char* buf_ = nullptr;
    unsigned int size_ = 0;

    void assign(const char* s) {
        if (s == nullptr) {
            delete[] buf_;
            buf_ = nullptr;
            size_ = 0;
        }
        else {
            size_t new_size = strlen(s);
            char* new_buf = new char[new_size + 1];
            strcpy(new_buf, s);
            delete[] buf_;
            buf_ = new_buf;
            buf_[new_size] = '\0';
            size_ = new_size;
        }
    }

    void assign(const char* s, const unsigned int len) {
        if (s == nullptr || len == 0) {
            clear();
        }
        else {
            char* new_buf = new char[len + 1];
            memcpy(new_buf, s, len);
            new_buf[len] = '\0';
            delete[] buf_;
            buf_ = new_buf;
            size_ = len + 1;
        }
    }

    // Helper method to free the buffer
    void clear() {
        delete[] buf_;
        buf_ = nullptr;
        size_ = 0;
    }

public:
    // Default constructor
    str() {}

    // Constructor with a const char* argument
    explicit str(const char* s) {
        assign(s);
    }

    // Copy constructor
    str(const str& other) {
        assign(other.buf_);
    }

    // Move constructor, it might be unnecessary!
    str(str&& other) noexcept {
        buf_ = other.buf_;
        size_ = other.size_;
        other.buf_ = nullptr;
        other.size_ = 0;
    }

    str(const char* s, unsigned int len) : buf_(nullptr), size_(0)
    {
        assign(s, len);
    }

    // Destructor
    ~str() {
        clear();
    }

    [[nodiscard]] str& operator=(const char c) {
        // replace the current string with a new string consisting only of the given char
        char new_buf[2] = { c, '\0' };
        assign(new_buf);
        return *this;
    }

    [[nodiscard]] str& operator=(const char* s) {
        // replace the current string with the new string specified by the const char*
        assign(s);
        return *this;
    }

    str& operator=(const str& other) {
        if (this != &other) {
            assign(other.buf_);
        }
        return *this;
    }

    str& operator+=(const char c) {
        const unsigned int new_size = size_ + 2;
        char* new_buf = new char[new_size];
        if (new_buf == nullptr) {
            throw std::bad_alloc();
        }
        if (buf_) {
            std::memcpy(new_buf, buf_, size_);
            delete[] buf_;
        }
        new_buf[size_] = c;
        new_buf[size_ + 1] = '\0';
        buf_ = new_buf;
        size_ = new_size - 1;
        return *this;
    }

    [[nodiscard]] bool empty() const { return size_ == 0; }

    bool operator==(const str& other) const
    {
        if (this->size_ != other.size_) return false;
        for (int i = 0; i < other.size_; i++)
        {
            if (this->buf_[i] != other.buf_[i]) return false;
        }
        return true;
    }

    bool operator==(const char* other) const
    {
        if (strlen(other) != this->size_) return false;
        for (int i = 0; i < this->size_;+ i++)
        {
            if (this->buf_[i] != other[i]) return false;
        }
        return true;
    }

    [[nodiscard]] const char* c_str() const { return buf_; }

    friend std::ostream& operator<<(std::ostream& out, const str& s)
    {
        out << s.c_str();
        return out;
    }

    [[nodiscard]] char back() const
    {
        if (this->buf_ == nullptr || this->size_ == 0)
            throw std::out_of_range("Back function failed");

        return this->c_str()[this->size_ - 1];
    }

    [[nodiscard]] char front() const
    {
        if (this->buf_ == nullptr || this->size_ == 0)
            throw std::out_of_range("Back function failed");

        return this->c_str()[0];
    }

    void pop_back()
    {
        if (this->buf_ == nullptr || this->size_ == 0) return;

        --this->size_;
        this->buf_[size_] = '\0';
    }

    void trim() {
        if (buf_ == nullptr) {
            return;
        }

        char* start = buf_;
        char* end = buf_ + size_ - 1;

        while (isspace(*start)) {
            start++;
        }

        while (end > start && isspace(*end)) {
            end--;
        }

        const unsigned int new_size = end - start + 1;
        char* new_buf = new char[new_size + 1];
        strncpy(new_buf, start, new_size);
        new_buf[new_size] = '\0';

        delete[] buf_;
        buf_ = new_buf;
        size_ = new_size;
    }



    [[nodiscard]] int find(const char c) const
    {
        if (this->buf_ == nullptr || this->size_ == 0) return -1;

        for (int i = 0; i < this->size_; i++)
        {
            if (this->c_str()[i] == c) return i;
        }

        return -1;
    }

    [[nodiscard]] int find(const char c, const int start) const
    {
        if (this->buf_ == nullptr || this->size_ == 0) return -1;

        for (int i = start; i < this->size_; i++)
        {
            if (this->c_str()[i] == c) return i;
        }

        return -1;
    }

    [[nodiscard]] Vector<str> split(const char delimiter) const
    {
        Vector<str> result;

        if (buf_ == nullptr) return result;

        const char* start = buf_;
        const char* end = buf_ + size_ - 1;

        while (start <= end)
        {
            const char* p = start;
            while (p <= end && *p != delimiter) ++p;

            const unsigned int len = p - start;
            if (len > 0) result.push_back(str(start, len));

            start = p + 1;
        }

        return Vector(result);
    }


    [[nodiscard]] str substr(const int start, const int len) const {
        if (start < 0 || start >= size_) {
            // start index out of bounds
            return str();
        }
        int substring_length = size_ - start;
        if (substring_length > len) {
            substring_length = len;
        }
        return str(c_str() + start, substring_length);
    }

    [[nodiscard]] str substr(const int start) const {
        if (start < 0 || start > this->size_) {
            throw std::out_of_range("Invalid start index");
        }

        const int new_length = this->size_ - start;
        char* new_data = new char[new_length + 1];

        for (int i = 0; i < new_length; i++) {
            new_data[i] = this->c_str()[start + i];
        }

        new_data[new_length] = '\0';

        return str(new_data);
    }

    [[nodiscard]] unsigned int length() const { return size_; }

private:
    // Helper method to initialize the buffer from a const char*


};
