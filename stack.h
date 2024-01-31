#ifndef STACK_H
#define STACK_H

#include <functional>
#include <mutex>
#include <stdexcept>

template <typename T>
class Stack
{
private:
	size_t     capacity;
	size_t     size;
	T*         elements;
	std::mutex mutex;

public:
	std::function<T()>            pop;
	std::function<T()>            top;
	std::function<void(const T&)> push;
	std::function<T(int)>         at;
	std::function<size_t()>       get_size;
	std::function<size_t()>       get_length;
	std::function<size_t()>       get_capacity;

public:
	Stack(bool is_thread_safe);
	Stack(size_t capacity, bool is_thread_safe);
	~Stack();

	const T& operator[](int index) const;

private:
	void initialize_functions(bool is_thread_safe);

	bool is_full();
	bool is_empty();

	T thread_safe_pop();
	T no_thread_safe_pop();

	T thread_safe_top();
	T no_thread_safe_top();

	void thread_safe_push(const T& element);
	void no_thread_safe_push(const T& element);

	T thread_safe_at(int index);
	T no_thread_safe_at(int index);

	size_t thread_safe_get_size();
	size_t no_thread_safe_get_size();

	size_t thread_safe_get_length();
	size_t no_thread_safe_get_length();

	size_t thread_safe_get_capacity();
	size_t no_thread_safe_get_capacity();
};

template <typename T>
Stack<T>::Stack(bool is_thread_safe)
	: capacity(10), size(0), elements(new T[capacity])
{
	initialize_functions(is_thread_safe);
}

template <typename T>
Stack<T>::Stack(size_t capacity, bool is_thread_safe)
	: capacity(capacity), size(0), elements(new T[capacity])
{
	initialize_functions(is_thread_safe);
}

template <typename T>
Stack<T>::~Stack()
{
	delete[] elements;
}

template <typename T>
const T& Stack<T>::operator[](int index) const
{
	return at(index);
}

template <typename T>
void Stack<T>::initialize_functions(bool is_thread_safe)
{
	if (is_thread_safe)
	{
		pop          = [this]() { return this->thread_safe_pop(); };
		top          = [this]() { return this->thread_safe_top(); };
		push         = [this](const T& element) { this->thread_safe_push(element); };
		at           = [this](int index) { return this->thread_safe_at(index); };
		get_size     = [this]() { return this->thread_safe_get_size(); };
		get_length   = [this]() { return this->thread_safe_get_length(); };
		get_capacity = [this]() { return this->thread_safe_get_capacity(); };
	}
	else
	{
		pop          = [this]() { return this->no_thread_safe_pop(); };
		top          = [this]() { return this->no_thread_safe_top(); };
		push         = [this](const T& element) { this->no_thread_safe_push(element); };
		at           = [this](int index) { return this->no_thread_safe_at(index); };
		get_size     = [this]() { return this->no_thread_safe_get_size(); };
		get_length   = [this]() { return this->no_thread_safe_get_length(); };
		get_capacity = [this]() { return this->no_thread_safe_get_capacity(); };
	}
}

template <typename T>
T Stack<T>::thread_safe_pop()
{
	std::lock_guard<std::mutex> lock(mutex);

	if (is_empty())
		throw std::out_of_range("Index out of range");

	size--;
	T popped_element = elements[size];
	elements[size] = T();

	return popped_element;
}

template <typename T>
T Stack<T>::no_thread_safe_pop()
{
	if (is_empty())
		throw std::out_of_range("Index out of range");

	size--;
	T popped_element = elements[size];
	elements[size] = T();

	return popped_element;
}

template <typename T>
T Stack<T>::thread_safe_top()
{
	std::lock_guard<std::mutex> lock(mutex);

	if (is_empty())
		throw std::out_of_range("Index out of range");

	return elements[size - 1];
}

template <typename T>
T Stack<T>::no_thread_safe_top()
{
	if (is_empty())
		throw std::out_of_range("Index out of range");

	return elements[size - 1];
}

template <typename T>
void Stack<T>::thread_safe_push(const T& element)
{
	std::lock_guard<std::mutex> lock(mutex);

	if (is_full())
		return;

	elements[size++] = element;
}

template <typename T>
void Stack<T>::no_thread_safe_push(const T& element)
{
	if (is_full())
		return;

	elements[size++] = element;
}

template <typename T>
T Stack<T>::thread_safe_at(int index)
{
	std::lock_guard<std::mutex> lock(mutex);

	if (index >= 0 && index < size)
		return elements[index];
	else
		throw std::out_of_range("Index out of range");
}

template <typename T>
T Stack<T>::no_thread_safe_at(int index)
{
	if (index >= 0 && index < size)
		return elements[index];
	else
		throw std::out_of_range("Index out of range");
}

template <typename T>
bool Stack<T>::is_full()
{
	return (size + 1 == capacity);
}

template <typename T>
bool Stack<T>::is_empty()
{
	return (size == 0);
}

template <typename T>
size_t Stack<T>::thread_safe_get_size()
{
	std::lock_guard<std::mutex> lock(mutex);

	return size * sizeof(T);
}

template <typename T>
size_t Stack<T>::no_thread_safe_get_size()
{
	return size * sizeof(T);
}

template <typename T>
size_t Stack<T>::thread_safe_get_length()
{
	std::lock_guard<std::mutex> lock(mutex);

	return size;
}

template <typename T>
size_t Stack<T>::no_thread_safe_get_length()
{
	return size;
}

template <typename T>
size_t Stack<T>::thread_safe_get_capacity()
{
	std::lock_guard<std::mutex> lock(mutex);

	return capacity;
}

template <typename T>
size_t Stack<T>::no_thread_safe_get_capacity()
{
	return capacity;
}

#endif