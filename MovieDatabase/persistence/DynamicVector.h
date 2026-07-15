#pragma once

template <typename T>
class DynamicVector {
private:
	int capacity;
	int length;
	T* elems; 

public:
	DynamicVector(int capacity = 10);

	DynamicVector(const DynamicVector<T>& other);

	DynamicVector<T>& operator=(const DynamicVector<T>& other);

	~DynamicVector();

	void add(const T& e);
	void remove(int pos);
	void resize();

	int getLength() const { return length; }
	T& get(int pos) { return elems[pos]; }

	DynamicVector<T> operator+(const T& e) const;

	template <typename U>
	friend DynamicVector<U> operator+(const U& a, const DynamicVector<U>& v);
};

template <typename T>
DynamicVector<T>::DynamicVector(int capacity) {
	/*	 * Allocates internal storage with the specified capacity and sets	 
	* length to 0.	 
	* Parameters:	
	* - capacity: initial capacity for internal array	 */
	this->capacity = capacity;
	this->length = 0;
	this->elems = new T[capacity];
}

template <typename T>
DynamicVector<T>::~DynamicVector() {
	delete[] this->elems;
}

template <typename T>
DynamicVector<T>::DynamicVector(const DynamicVector<T>& other) {
	this->capacity = other.capacity;
	this->length = other.length;

	this->elems = new T[this->capacity];

	for (int i = 0; i < this->length; i++) {
		this->elems[i] = other.elems[i];
	}
}

template <typename T>
DynamicVector<T>& DynamicVector<T>::operator=(const DynamicVector<T>& other) {
	if (this == &other) {
		return *this;
	}

	delete[] this->elems;

	this->capacity = other.capacity;
	this->length = other.length;

	this->elems = new T[this->capacity];
	for (int i = 0; i < this->length; i++) {
		this->elems[i] = other.elems[i];
	}

	return *this;
}

template <typename T>
void DynamicVector<T>::resize() {
	/*	 * Doubles internal capacity, allocates a new array and copies existing elements preserving order.	 
	* Parameters:	
	* - none	
	* Errors:	
	* - may throw std::bad_alloc if memory allocation fails	 */
	this->capacity *= 2;
	T* newElems = new T[this->capacity];
	for (int i = 0; i < this->length; i++) {
		newElems[i] = this->elems[i];
	}
	delete[] this->elems;
	this->elems = newElems;
}

template <typename T>
void DynamicVector<T>::add(const T& e) {
	/*	 * Appends element `e` to the end of the vector. Resizes if full.	
	* Parameters:	 * - e: element to append (copied)	
	* Errors:	
	* - may throw std::bad_alloc indirectly via resize	 */
	if (this->length == this->capacity) {
		this->resize();
	}
	this->elems[this->length++] = e;
}

template <typename T>
void DynamicVector<T>::remove(int pos) {
	/*	 * Removes the element at index `pos` by shifting subsequent elements one position left and decrementing length.	
	* Parameters:	
	* - pos: index of the element to remove	 */
	if (pos < 0 || pos >= this->length) {
		return;
	}
	for (int i = pos; i < this->length - 1; i++) {
		this->elems[i] = this->elems[i + 1];
	}
	this->length--;
}

template <typename T>
DynamicVector<T> DynamicVector<T>::operator+(const T& e) const {
	DynamicVector<T> result = *this;
	result.add(e);
	return result;
}

template <typename U>
DynamicVector<U> operator+(const U& a, const DynamicVector<U>& v) {
	DynamicVector<U> result = v;
	result.add(a);
	return result;
}