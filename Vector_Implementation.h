#ifndef VECTOR_IMPLEMENTATION_H_DEFINED
#define VECTOR_IMPLEMENTATION_H_DEFINED
#include <iostream>
#include <fstream>
#include <string>
#include <stdexcept>

using namespace std;
template <typename T>
class Vector_Implementation {
    T* data;
    size_t size = 0;
    size_t capacity = 1;

    public:
        /** Vector_Implementation
         * @param None
         * @returns None
        */
        Vector_Implementation() {data = new T[this->capacity];}

        /** Vector_Implementation
         * @param None
         * @returns None
        */
        Vector_Implementation(size_t cap)
        {
            this->capacity = cap;
            data = new T[cap];
        }

        /** Vector_Implementation
         * @param None
         * @returns A destructor that deletes the memory space used by data
        */
        ~Vector_Implementation() {delete[] data;}

        /** Vector_Implementation
         * @param rhs the object from which to copy
         * @returns None
        */
        Vector_Implementation(const Vector_Implementation &rhs) 
        {
            this->data = rhs.data;
            this->capacity = rhs.capacity;
            this->size = rhs.size;
            if (size > 0) {
                this->data = new T[size];
                for (size_t i = 0; i < rhs.size; i++) {
                    data[i] = rhs.data[i];
                } 
            }
        }

        /** Vector_Implementation
         * @param rhs the object from which to copy
         * returns None
        */
        Vector_Implementation& operator=(const Vector_Implementation& rhs)
        {
            if (this != &rhs) {
                delete[] this->data;

                this->data = rhs.data;
                this->capacity = rhs.capacity;
                this->size = rhs.size;

                if (size > 0) {
                    this->data = new T[size];
                    for (size_t i = 0; i < rhs.size; i++) {
                        data[i] = rhs.data[i];
                    } 
                }
            }
            return *this;
        }

        /** size
         * @param None
         * @returns the current size of the vector
        */
        size_t get_size() {return this->size;}

        /** max_size
         * @param None
         * @returns the value of the largest ammount of data that can be stored of templated object type 
        */
        size_t max_size() {return (2^(32-get_size()))-1;}

        /** capacity
         * @param None
         * @returns the current capacity of the vector
        */
        size_t get_capacity() {return this->capacity;}

        /** resize
         * @param n the number to which the vector should be resized
         * @returns None
        */
        void resize(size_t n) 
        {
            T* temp = new T[n];
            if (n < this->size) {
                for (size_t i = 0; i < n; i++) {
                    temp[i] = data[i];
                }
            }
            else {
                for (size_t i = 0; i < n; i++) {
                    if (i > get_size()-1) temp[i] = T{};
                    else temp[i] = data[i];
                }
                if (n > this->capacity) this->capacity = n;
            }
            delete[] data;
            data = temp;
        }

        /** resize
         * @param n the number to which the vector should be resized
         * @param val the values used to initialize the data at each location
         * @returns None
        */
        void resize(size_t n, const T& val)
        {
            T* temp = new T[n];
            if (n < this->size) {
                for (size_t i = 0; i < n; i++) {
                    temp[i] = data[i];
                }
            }
            else {
                for (size_t i = 0; i < n; i++) {
                    if (i > get_size()-1) temp[i] = val;
                    else temp[i] = data[i];
                }
                if (n > this->capacity) this->capacity = n;
            }
            delete[] data;
            data = temp;
        }

        /** empty
         * @param None
         * @returns a true if the vector does not have any data, else false
        */
        bool empty() {return this->size == 0;}

        /** reserve
         * @param n the number for which to reserve memory
         * @returns None
        */
        void reserve(size_t n)
        {
            if (n > this->capacity)
                resize(n);
        }

        /** shrink_to_fit
         * @param None
         * @returns None
        */
        void shrink_to_fit() {resize(this->size);}

        /** operator[]
         * @param n the index at which to access the data
         * @returns the data at index n
        */
        T& operator[](size_t n) {return this->data[n];}

       /** at
        * @param n the index at which to access the data
        * @returns the data at index n 
       */
        T& at(size_t n)
        {
            if (n >= this->size) {
                string f_string = "Vector_Implementation - at(size_t) - Index Out of Bounds for size: " + to_string(this->size);
                throw range_error(f_string);
            }
            else return this->data[n];
        }

        /** front
         * @param None
         * @returns a reference to the first element in the vector
        */
        T& front() {return data[0];}

        /** back
         * @param None
         * @returns a reference to the last element in the vector
        */
        T& back() {return data[this->size-1];}

        /** data
         * @param None
         * @returns the pointer to the data array
        */
        T* get_data() {return this->data;}

        /** push_back
         * @param val the data to be added into the vector
         * @returns None
        */
        void push_back(const T& val)
        {
            if (size == capacity) resize(2*capacity);
            data[size] = val;
            size++;
        }

        /** pop_back
         * @param None
         * @returns None
        */
        void pop_back() 
        {
            data[size-1] = T{};
            size--;
        }

        /** swap
         * @param other a reference to the other vector from which to swap data
         * @returns None
        */
        void swap(Vector_Implementation& other)
        {
            size_t temp_size = this->size;
            this->size = other.size;
            other.size = temp_size;

            size_t temp_cap = this->capacity;
            this->capacity = other.capacity;
            other.capacity = temp_cap;

            T* temp_data = this->data;
            this->data = other.data;
            other.data = temp_data;
        }

        /** clear
         * @param None
         * @returns None
        */
        void clear()
        {
            for (size_t i = 0; i < this->size; i++) {
                data[i] = T{};
            }
            this->size = 0;
        }

       //DELETE: temp method used for debugging
       void display() const {
            for (size_t i = 0; i < size; ++i) {
                cout << data[i] << " ";
            }
            cout << endl;
            cout << "Capacity: " << this->capacity << endl;
            cout << "Size: " << this->size << endl;
        }
};

#endif/*VECTOR_IMPLEMENTATION_H_INCLUDED*/