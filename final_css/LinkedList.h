#pragma once

template<typename T>
class LinkedList {
public:
    LinkedList() : head(nullptr), tail(nullptr), size(0) {}
    ~LinkedList() {
        clear();
    }

    LinkedList(const LinkedList<T>& other) : head(nullptr), tail(nullptr), size(0) {
        // Iterate through other and create a new node for each element
        for (Node* node = other.head; node != nullptr; node = node->next) {
            push_back(node->value);
        }
    }

    void push_front(const T& value) {
        Node* new_node = new Node(value, nullptr, head);
        if (head != nullptr) {
            head->prev = new_node;
        }
        else {
            tail = new_node;
        }
        head = new_node;
        size++;
    }

    void push_back (T& value)
    {
        Node* new_node = new Node(value, tail, nullptr);
        if (tail != nullptr) {
            tail->next = new_node;
        }
        else {
            head = new_node;
        }
        tail = new_node;
        size++;
    }

    void push_back(const T& value) {
        Node* new_node = new Node(value, tail, nullptr);
        if (tail != nullptr) {
            tail->next = new_node;
        }
        else {
            head = new_node;
        }
        tail = new_node;
        size++;
    }

    void pop_front() {
        if (head != nullptr) {
            Node* old_head = head;
            head = head->next;
            if (head != nullptr) {
                head->prev = nullptr;
            }
            else {
                tail = nullptr;
            }
            delete old_head;
            size--;
        }
    }

    T& get(const int i) const {
        if (i < 0 || i >= size) {
            //throw std::out_of_range("Index out of range");
        }

        Node* current_node;
        int current_index;

        if (i <= size / 2) {
            // start from the head and move forward
            current_node = head;
            current_index = 0;
            while (current_index < i) {
                current_node = current_node->next;
                current_index++;
            }
        }
        else {
            // start from the tail and move backward
            current_node = tail;
            current_index = size - 1;
            while (current_index > i) {
                current_node = current_node->prev;
                current_index--;
            }
        }

        return current_node->value;
    }

    void pop_back() {
        if (tail != nullptr) {
            Node* old_tail = tail;
            tail = tail->prev;
            if (tail != nullptr) {
                tail->next = nullptr;
            }
            else {
                head = nullptr;
            }
            delete old_tail;
            size--;
        }
    }

    T& front() const {
        return head->value;
    }

    T& back() const {
        return tail->value;
    }

    bool empty() const {
        return size == 0;
    }

    int length() const {
        return size;
    }

    void clear() {
        Node* node = head;
        while (node != nullptr) {
            Node* next_node = node->next;
            delete node;
            node = next_node;
        }
        head = nullptr;
        tail = nullptr;
        size = 0;
    }


    void erase(const int i) {
        if (i < 0 || i >= size) {
            // throw std::out_of_range("Index out of range");
        }
        if (i == 0) {
            pop_front();
            return;
        }
        if (i == size - 1) {
            pop_back();
            return;
        }
        Node* current_node;
        int current_index;
        if (i <= size / 2) {
            // start from the head and move forward
            current_node = head;
            current_index = 0;
            while (current_index < i) {
                current_node = current_node->next;
                current_index++;
            }
        }
        else {
            // start from the tail and move backward
            current_node = tail;
            current_index = size - 1;
            while (current_index > i) {
                current_node = current_node->prev;
                current_index--;
            }
        }
        current_node->prev->next = current_node->next;
        current_node->next->prev = current_node->prev;
        delete current_node;
        size--;
    }


    class Node {
    public:
        Node(const T& value, Node* prev, Node* next) : value(value), prev(prev), next(next) {}

        T value;
        Node* prev;
        Node* next;
    };

    class LinkedListIterator {
    public:
        LinkedListIterator(Node* current_node) : current_node(current_node) {}

        T& operator*() const {
            return current_node->value;
        }

        T* operator->() const {
            return &current_node->value;
        }

        LinkedListIterator& operator++() {
            current_node = current_node->next;
            return *this;
        }

        bool operator==(const LinkedListIterator& other) const {
            return current_node == other.current_node;
        }

        bool operator!=(const LinkedListIterator& other) const {
            return !(*this == other);
        }
        Node* current_node;
    };

    LinkedListIterator begin() const {
        return LinkedListIterator(head);
    }

    LinkedListIterator end() const {
        return LinkedListIterator(nullptr);
    }



    void erase(const LinkedListIterator& it) {
        Node* current_node = it.current_node;
        if (current_node == nullptr) {
            // throw std::out_of_range("Iterator out of range");
            return;
        }

        if (current_node == head) {
            pop_front();
            return;
        }

        if (current_node == tail) {
            pop_back();
            return;
        }

        current_node->prev->next = current_node->next;
        current_node->next->prev = current_node->prev;
        delete current_node;
        size--;
    }



    class LinkedListReverseIterator {
    public:
        LinkedListReverseIterator(Node* current_node) : current_node(current_node) {}

        T& operator*() const {
            return current_node->value;
        }

        LinkedListReverseIterator& operator++() {
            current_node = current_node->prev;
            return *this;
        }

        bool operator==(const LinkedListReverseIterator& other) const {
            return current_node == other.current_node;
        }

        bool operator!=(const LinkedListReverseIterator& other) const {
            return !(*this == other);
        }

    private:
        Node* current_node;
    };

    LinkedListReverseIterator rbegin() const {
        return LinkedListReverseIterator(tail);
    }

    LinkedListReverseIterator rend() const {
        return LinkedListReverseIterator(nullptr);
    }


private:
    Node* head;
    Node* tail;
    int size;
};