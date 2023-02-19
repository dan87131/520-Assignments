#ifndef TYPED_ARRAY
#define TYPED_ARRAY

#include <assert.h>
#include <iostream>
#include <stdexcept>

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


private:

    int capacity,
        origin,
        end;

    ElementType * buffer;   

    const int INITIAL_CAPACITY = 10;

    int index_to_offset(int index) const;
    int offset_to_index(int offset) const;
    bool out_of_buffer(int offset) const;
    void extend_buffer(void);    
    void push(const ElementType& value);
    void push_front(const ElementType& value);
    void pop();
    void pop_front();
    TypedArray<ElementType>& reverse();
    TypedArray<ElementType> concat(const TypedArray<ElementType>& other) const;

    

};

template <typename ElementType>
TypedArray<ElementType>::TypedArray() {
    buffer = new ElementType[INITIAL_CAPACITY]();
    capacity = INITIAL_CAPACITY;    
    origin = capacity / 2;
    end = origin;    
}

// Copy constructor: i.e TypedArray b(a) where a is a TypedArray
template <typename ElementType>
TypedArray<ElementType>::TypedArray(const TypedArray& other) : TypedArray() {
    *this = other;
}

// Assignment operator: i.e TypedArray b = a 
template <typename ElementType>
TypedArray<ElementType>& TypedArray<ElementType>::operator=(const TypedArray<ElementType>& other) {
    if ( this != &other) {
        delete[] buffer; // don't forget this or you'll get a memory leak!
        buffer = new ElementType[other.capacity]();
        capacity = other.capacity;
        origin = other.origin;
        end = origin;
        for ( int i=0; i<other.size(); i++) {
            set(i,other.safe_get(i));
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
        throw std::range_error("Out of range index in array");
    }
    if ( index >= size() ) {
        ElementType x;
        set(index, x);
    } 
    return buffer[index_to_offset(index)];
}

// Getters
template <typename ElementType>
ElementType &TypedArray<ElementType>::safe_get(int index) const {
    if (index < 0 || index >= size() ) {
        throw std::range_error("Out of range index in array");
    }
    return buffer[index_to_offset(index)];
}

template <typename ElementType>
int TypedArray<ElementType>::size() const {
    return end - origin;
}

// Setters
template <typename ElementType>
void TypedArray<ElementType>::set(int index, ElementType value) {
    if (index < 0) {
        throw std::range_error("Negative index in array");
    }
    while ( out_of_buffer(index_to_offset(index) ) ) {
        extend_buffer();
    }
    buffer[index_to_offset(index)] = value; // Should call the copy constructor
                                            // of ElementType.
    if ( index >= size() ) {
        end = index_to_offset(index+1);
    }
}

template <typename ElementType>
std::ostream &operator<<(std::ostream &os, TypedArray<ElementType> &array)
{
    os << '[';
    for (int i=0; i<array.size(); i++ ) {
        os << array.get(i);
        if ( i < array.size() - 1 ) {
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

/* Position of the element at buffer position 'offset' */
template <typename ElementType>
int TypedArray<ElementType>::offset_to_index ( int offset ) const  {
    return offset - origin;
}

/* Non-zero if and only if offset lies ouside the buffer */
template <typename ElementType>
bool TypedArray<ElementType>::out_of_buffer ( int offset ) const {
    return offset < 0 || offset >= capacity;
}

/* Makes a new buffer that is twice the size of the old buffer,
   copies the old information into the new buffer, and deletes
   the old buffer */
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
    buffer[origin++] = value;
}


template <typename ElementType>
void TypedArray<ElementType>::pop() {
    if (size() == 0) {
        throw std::range_error("Cannot pop from an empty array");
    }
    --end;
}

template <typename ElementType>
void TypedArray<ElementType>::pop_front() {
    if (size() == 0) {
        throw std::range_error("Cannot pop from an empty array");
    }
    ++origin;
}

//concat
template <typename T>
TypedArray<T> TypedArray<T>::concat(const TypedArray<T>& other) const {
    TypedArray<T> result(this->size() + other.size());

    if (this->size() > 0) {
        memcpy(result.get_pointer(), this->get_pointer(), this->size() * sizeof(T));
    }

    if (other.size() > 0) {
        memcpy(result.get_pointer() + this->size(), other.get_pointer(), other.size() * sizeof(T));
    }

    return result;
}



//reverse
template <typename T>
TypedArray<T>& TypedArray<T>::reverse() {
    for (int i = origin, j = end - 1; i < j; ++i, --j) {
        std::swap(buffer[i], buffer[j]);
    }
    return *this;
}


//+ operator
template <typename T>
TypedArray<T> operator+(const TypedArray<T>& a, const TypedArray<T>& b) {
  TypedArray<T> result(a.size() + b.size());
  for (size_t i = 0; i < a.size(); i++) {
    result.set(i, a.get(i));
  }
  for (size_t i = 0; i < b.size(); i++) {
    result.set(a.size() + i, b.get(i));
  }
  return result;
}

#endif