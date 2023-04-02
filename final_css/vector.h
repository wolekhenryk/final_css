#pragma once

template<typename T>
class Vector
{
	T* data_;
	int size_;
	int capacity_;

public:
	Vector() : data_(nullptr), size_(0), capacity_(0) {}

	Vector(const Vector<T>& other) : data_(nullptr), size_(0), capacity_(0)
	{
		reserve(other.size_);

		for (int i = 0; i < other.size_; i++) {
			push_back(other[i]);
		}
	}

	~Vector() { clear(); }

	void push_back(const T& value)
	{
		if (size_ == capacity_) reserve(capacity_ == 0 ? 1 : capacity_ * 2);
		data_[size_++] = value;
	}

	void pop_back()
	{
		if (size_ > 0) size_--;
	}

	void reserve(int new_capacity)
	{
		if (new_capacity > capacity_)
		{
			T* new_data = new T[new_capacity];
			if (data_ != nullptr)
			{
				for (int i = 0; i < size_; i++) new_data[i] = data_[i];
				delete[] data_;
			}
			data_ = new_data;
			capacity_ = new_capacity;
		}
	}

	const T& operator[](int i) const { return data_[i]; }

	int size() const { return size_; }

	void clear()
	{
		delete[] data_;
		data_ = nullptr;
		size_ = 0;
		capacity_ = 0;
	}

	T* begin() const { return data_; }

	T* end() const { return data_ + size_; }
};