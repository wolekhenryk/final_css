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

	void erase(const int index)
	{
		if (index < 0 || index >= size_) return;

		for (int i = index + 1; i < size_; i++) data_[i - 1] = data_[i];

		size_--;
	}

	class reverse_iterator
	{
		T* ptr_;

	public:
		explicit reverse_iterator(T* ptr) : ptr_(ptr) {}

		T& operator*() const { return *ptr_; }
		T* operator->() const { return  ptr_; }

		bool operator==(const reverse_iterator& other) const { return ptr_ == other.ptr_; }
		bool operator!=(const reverse_iterator& other) const { return ptr_ != other.ptr_; }

		reverse_iterator& operator++()
		{
			--ptr_;
			return *this;
		}
	};

	reverse_iterator rbegin() { return reverse_iterator(data_ + size_ - 1); }
	reverse_iterator rend() { return reverse_iterator(data_ - 1); }

	class iterator {
		T* current_;

	public:
		explicit iterator(T* ptr) : current_(ptr) {}
		T& operator*() const { return *current_; }
		T* operator->() const { return current_; }
		iterator& operator++() { ++current_; return *this; }
		iterator operator++(int) { iterator temp(current_); ++current_; return temp; }
		iterator& operator--() { --current_; return *this; }
		iterator operator--(int) { iterator temp(current_); --current_; return temp; }
		bool operator==(const iterator& other) const { return current_ == other.current_; }
		bool operator!=(const iterator& other) const { return !(*this == other); }
	};

	iterator begin() const { return iterator(data_); }
	iterator end() const { return iterator(data_ + size_); }

	void erase(iterator it)
	{
		if (it == end()) return;

		iterator next_it = it;
		++next_it;

		while (next_it != end())
		{
			*it = *next_it;
			++it;
			++next_it;
		}

		--size_;
	}
};