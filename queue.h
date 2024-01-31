#ifndef QUEUE_H
#define QUEUE_H

#include <iostream>
#include <functional>
#include <mutex>
#include <stdexcept>

template <typename T>
class Queue
{
private:
	size_t             size;
	mutable std::mutex mutex;
	bool               is_thread_safe;

	struct Node
	{
		T data;
		Node* next;

		Node(const T& data) : data(data), next(nullptr) { }
	};

	Node* front_node;
	Node* back_node;

public:
	std::function<T()>            pop;
	std::function<T()>            front;
	std::function<T()>            back;
	std::function<void(const T&)> push;
	std::function<T(int)>         at;
	std::function<size_t()>       get_size;
	std::function<size_t()>       get_length;

public:
	Queue(bool is_thread_safe);
	~Queue();

	const T& operator[](int index) const;

private:
	void initialize_functions();
	bool is_empty();

	T thread_safe_pop();
	T no_thread_safe_pop();

	T thread_safe_front();
	T no_thread_safe_front();

	T thread_safe_back();
	T no_thread_safe_back();

	void thread_safe_push(const T& element);
	void no_thread_safe_push(const T& element);

	T thread_safe_at(int index);
	T no_thread_safe_at(int index);

	size_t thread_safe_get_size();
	size_t no_thread_safe_get_size();

	size_t thread_safe_get_length();
	size_t no_thread_safe_get_length();
};

template <typename T>
Queue<T>::Queue(bool is_thread_safe)
	: size(0), is_thread_safe(is_thread_safe)
{
	initialize_functions();
}

template <typename T>
Queue<T>::~Queue()
{
	while (!is_empty())
	{
		pop();
	}
}

template <typename T>
const T& Queue<T>::operator[](int index) const
{
	if (is_thread_safe)
	{
		std::lock_guard<std::mutex> lock(mutex);

		Node* current = front_node;
		for (size_t i = 0; i < index; i++)
			current = current->next;

		return current->data;
	}

	Node* current = front_node;
	for (size_t i = 0; i < index; i++)
		current = current->next;

	return current->data;
	
}

template <typename T>
void Queue<T>::initialize_functions()
{
	if (is_thread_safe)
	{
		pop = [this]() { return this->thread_safe_pop(); };
		front = [this]() { return this->thread_safe_front(); };
		back = [this]() { return this->thread_safe_back(); };
		push = [this](const T& element) { this->thread_safe_push(element); };
		at = [this](int index) { return this->thread_safe_at(index); };
		get_size = [this]() { return this->thread_safe_get_size(); };
		get_length = [this]() { return this->thread_safe_get_length(); };
	}
	else
	{
		pop = [this]() { return this->no_thread_safe_pop(); };
		front = [this]() { return this->no_thread_safe_front(); };
		back = [this]() { return this->no_thread_safe_back(); };
		push = [this](const T& element) { this->no_thread_safe_push(element); };
		at = [this](int index) { return this->no_thread_safe_at(index); };
		get_size = [this]() { return this->no_thread_safe_get_size(); };
		get_length = [this]() { return this->no_thread_safe_get_length(); };
	}
}

template <typename T>
bool Queue<T>::is_empty()
{
	return (size == 0);
}

template <typename T>
T Queue<T>::thread_safe_pop()
{
	std::lock_guard<std::mutex> lock(mutex);

	if (is_empty())
		throw std::out_of_range("Index out of range");

	Node* temp = front_node;
	front_node = front_node->next;

	T popped_element = temp->data;

	delete temp;

	size--;
	if (is_empty())
	{
		front_node = nullptr;
		back_node = nullptr;
	}

	return popped_element;
}

template <typename T>
T Queue<T>::no_thread_safe_pop()
{
	if (is_empty())
		throw std::out_of_range("Index out of range");

	Node* temp = front_node;
	front_node = front_node->next;

	T popped_element = temp->data;

	delete temp;

	size--;
	if (is_empty())
	{
		front_node = nullptr;
		back_node = nullptr;
	}

	return popped_element;
}

template <typename T>
T Queue<T>::thread_safe_front()
{
	std::lock_guard<std::mutex> lock(mutex);

	if (is_empty())
		throw std::out_of_range("Index out of range");

	return front_node->data;
}

template <typename T>
T Queue<T>::no_thread_safe_front()
{
	if (is_empty())
		throw std::out_of_range("Index out of range");

	return front_node->data;
}

template <typename T>
T Queue<T>::thread_safe_back()
{
	std::lock_guard<std::mutex> lock(mutex);

	if (is_empty())
		throw std::out_of_range("Index out of range");

	return back_node->data;
}

template <typename T>
T Queue<T>::no_thread_safe_back()
{
	if (is_empty())
		throw std::out_of_range("Index out of range");

	return back_node->data;
}

template <typename T>
void Queue<T>::thread_safe_push(const T& element)
{
	std::lock_guard<std::mutex> lock(mutex);

	Node* node = new Node(element);

	if (is_empty())
	{
		front_node = back_node = node;
	}
	else
	{
		back_node->next = node;
		back_node = node;
	}

	size++;
}

template <typename T>
void Queue<T>::no_thread_safe_push(const T& element)
{
	Node* node = new Node(element);

	if (is_empty())
	{
		front_node = back_node = node;
	}
	else
	{
		back_node->next = node;
		back_node = node;
	}

	size++;
}

template <typename T>
T Queue<T>::thread_safe_at(int index)
{
	std::lock_guard<std::mutex> lock(mutex);
	
	if (index < 0 || index >= size)
		throw std::out_of_range("Index out of range");

	if (index == 0)
		return front_node->data;

	else if (index == size - 1)
		return back_node->data;

	Node* current = front_node;
	for (size_t i = 0; i < index; i++)
		current = current->next;

	return current->data;
}

template <typename T>
T Queue<T>::no_thread_safe_at(int index)
{
	if (index < 0 || index >= size)
		throw std::out_of_range("Index out of range");

	if (index == 0)
		return front_node->data;

	else if (index == size - 1)
		return back_node->data;

	Node* current = front_node;
	for (size_t i = 0; i < index; i++)
		current = current->next;

	return current->data;
}

template <typename T>
size_t Queue<T>::thread_safe_get_size()
{
	std::lock_guard<std::mutex> lock(mutex);

	return size * sizeof(T);
}

template <typename T>
size_t Queue<T>::no_thread_safe_get_size()
{
	return size * sizeof(T);
}

template <typename T>
size_t Queue<T>::thread_safe_get_length()
{
	std::lock_guard<std::mutex> lock(mutex);

	return size;
}

template <typename T>
size_t Queue<T>::no_thread_safe_get_length()
{
	return size;
}

#endif