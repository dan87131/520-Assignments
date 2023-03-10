#ifndef TYPED_ARRAY
#define TYPED_ARRAY

#include <assert.h>
#include <iostream>
#include <stdexcept>
#include <algorithm>


template <typename ElementType>
class TypedArray {
public:
    TypedArray();
    TypedArray(const TypedArray& other);

    // Copy constructor
    TypedArray& operator=(const TypedArray& other);

    // Destructor
    ~TypedArray();

    // Getters
    ElementType &get(int index);
    ElementType &safe_get(int index) const;
    int size() const;

    // Setters
    void set(int index, ElementType value);
    void push(const ElementType& value);
    void push_front(const ElementType& value);
    void pop();
    void pop_front();
    TypedArray<ElementType>& reverse();
    TypedArray<ElementType> operator+(const TypedArray<ElementType>& other) const;
    TypedArray<ElementType> concat(const TypedArray<ElementType>& other) const;

private:
    int capacity, origin, end;
    ElementType * buffer;
    const int INITIAL_CAPACITY = 100;
    int index_to_offset(int index) const;
    int offset_to_index(int offset) const;
    bool out_of_buffer(int offset) const;
    void extend_buffer(void);
};

template <typename ElementType>
TypedArray<ElementType>::TypedArray() {
    buffer = new ElementType[INITIAL_CAPACITY]();
    capacity = INITIAL_CAPACITY;
    origin = capacity / 2;
    end = origin;
}

// Copy constructor
template <typename ElementType>
TypedArray<ElementType>::TypedArray(const TypedArray& other) : TypedArray() {
    *this = other;
}

// Assignment operator
template <typename ElementType>
TypedArray<ElementType>& TypedArray<ElementType>::operator=(const TypedArray<ElementType>& other) {
    if (this != &other) {
        delete[] buffer;
        buffer = new ElementType[other.capacity]();
        capacity = other.capacity;
        origin = other.origin;
        end = origin;
        for (int i = 0; i < other.size(); i++) {
            set(i, other.safe_get(i));
        }
    }
    return *this;
}

// Destructor
template <typename ElementType>
TypedArray<ElementType>::~TypedArray() {
    delete[] buffer;
}

// Getters
template <typename ElementType>
ElementType &TypedArray<ElementType>::get(int index) {
    if (index < 0) {
        throw std::range_error("Out of range");
    }
    if (index >= size()) {
        ElementType x;
        set(index, x);
    }
    return buffer[index_to_offset(index)];
}

template <typename ElementType>
ElementType &TypedArray<ElementType>::safe_get(int index) const {
    assert(index >= 0 && index < size());
    return buffer[index_to_offset(index)];
}

template <typename ElementType>
int TypedArray<ElementType>::size() const {
    assert(origin <= end);
    return end - origin;
}


// Setters
template <typename ElementType>
void TypedArray<ElementType>::set(int index, ElementType value) {
    if (index < 0) {
        throw std::range_error("Negative index");
    }
    while (out_of_buffer(index_to_offset(index))) {
        extend_buffer();
    }
    buffer[index_to_offset(index)] = value;
    if (index >= size()) {
        end = index_to_offset(index + 1);
    }
}

template <typename ElementType>
std::ostream &operator<<(std::ostream &os, TypedArray<ElementType> &array)
{
    os << '[';
    for (int i = 0; i < array.size(); i++) {
        os << array.get(i);
        if (i < array.size() - 1) {
            os << ",";
        }
    }
    os << ']';
    return os;
}

// Private methods
template <typename ElementType>
int TypedArray<ElementType>::index_to_offset ( int index ) const {
    return index + origin;
}

/* The element's location in the buffer at position 'offset'*/
template <typename ElementType>
int TypedArray<ElementType>::offset_to_index ( int offset ) const  {
    return offset - origin;
}

template <typename ElementType>
bool TypedArray<ElementType>::out_of_buffer ( int offset ) const {
    return offset < 0 || offset >= capacity;
}

/* Creates a new buffer twice the size of the existing one, replicates the old information into the new one, then deletes the old one. */
template <typename ElementType>
void TypedArray<ElementType>::extend_buffer() {

    auto temp = new ElementType[2 * capacity]();
    int new_origin = capacity - (end - origin)/2,
           new_end = new_origin + (end - origin);

    for ( int i=0; i<size(); i++ ) {
        temp[new_origin+i] = get(i);
    }

    delete[] buffer;
    buffer = temp;

    capacity = 2 * capacity;
    origin = new_origin;
    end = new_end;

    return;

}


template <typename ElementType>
void TypedArray<ElementType>::push(const ElementType& value) {
    set(size(), value);
}

template <typename ElementType>
void TypedArray<ElementType>::push_front(const ElementType& value) {
    if (origin == 0) {
        extend_buffer();
    }
    buffer[--origin] = value;
}


template <typename ElementType>
void TypedArray<ElementType>::pop() {
    if (size() == 0) {
        throw std::range_error("Cannot pop from an empty array");
    }
    end--;
}

template <typename ElementType>
void TypedArray<ElementType>::pop_front() {
    if (size() == 0) {
        throw std::range_error("Cannot pop from an empty array");
    }
    origin++;
}

template <typename T>
TypedArray<T> TypedArray<T>::concat(const TypedArray<T>& other) const {
    TypedArray<T> result(this->size() + other.size());
    int index = 0;

    // Copy elements from the first array
    for (int i = 0; i < this->size(); i++) {
        result.set(index++, this->get(i));
    }

    // Copy elements from the second array
    for (int i = 0; i < other.size(); i++) {
        result.set(index++, other.get(i));
    }

    return result;
}


template <typename T>
TypedArray<T>& TypedArray<T>::reverse() {
    std::reverse(buffer + origin, buffer + end);
    return *this;
}

template <typename ElementType>
TypedArray<ElementType> TypedArray<ElementType>::operator+(const TypedArray<ElementType>& other) const {
    return this->concat(other);
}

#endif

