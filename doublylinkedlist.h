#ifndef DOUBLYLINKEDLIST_H
#define DOUBLYLINKEDLIST_H

#include <iostream>
#include <functional>
#include <mutex>
#include <stdexcept>

template <typename T>
class DoublyLinkedList
{
private:
	size_t             size;
	mutable std::mutex mutex;
	bool               is_thread_safe;

	struct Node
	{
		T     data;
		Node* next;
		Node* previous;

		Node(const T& data) : data(data), next(nullptr), previous(nullptr) { }
	};

	Node* head;
	Node* tail;

public:
	std::function<void(const T&)>      push_front;
	std::function<void(const T&)>      push_back;
	std::function<void(const T&, int)> push_at;
	std::function<T()>                 pop_front;
	std::function<T()>                 pop_back;
	std::function<T(int)>              pop_at;
	std::function<void()>              clear;
	std::function<T()>                 get_head;
	std::function<T()>                 get_tail;
	std::function<T(int)>              at;
	std::function<size_t()>            get_size;
	std::function<size_t()>            get_length;

public:
	DoublyLinkedList(bool is_thread_safe);
	~DoublyLinkedList();

	const T& operator[](int index) const;

private:
	void initialize_functions();
	bool is_empty();

	void thread_safe_push_front(const T& element);
	void no_thread_safe_push_front(const T& element);

	void thread_safe_push_back(const T& element);
	void no_thread_safe_push_back(const T& element);

	void thread_safe_push_at(const T& element, int index);
	void no_thread_safe_push_at(const T& element, int index);

	T thread_safe_pop_front();
	T no_thread_safe_pop_front();
	
	T thread_safe_pop_back();
	T no_thread_safe_pop_back();

	T thread_safe_pop_at(int index);
	T no_thread_safe_pop_at(int index);

	void thread_safe_clear();
	void no_thread_safe_clear();

	T thread_safe_get_head();
	T no_thread_safe_get_head();

	T thread_safe_get_tail();
	T no_thread_safe_get_tail();

	T thread_safe_at(int index);
	T no_thread_safe_at(int index);

	size_t thread_safe_get_size();
	size_t no_thread_safe_get_size();

	size_t thread_safe_get_length();
	size_t no_thread_safe_get_length();
};

template <typename T>
DoublyLinkedList<T>::DoublyLinkedList(bool is_thread_safe)
	: size(0), is_thread_safe(is_thread_safe)
{
	initialize_functions();
}

template <typename T>
DoublyLinkedList<T>::~DoublyLinkedList()
{
	if (!is_empty())
		clear();
}

template <typename T>
const T& DoublyLinkedList<T>::operator[](int index) const
{
	if (is_thread_safe)
	{
		std::lock_guard<std::mutex> lock(mutex);

		if (index == 0)
			return head->data;

		else if (index == size - 1)
			return tail->data;

		bool is_close_to_head = (index <= (size - index));
		Node* current = is_close_to_head ? head : tail;

		if (is_close_to_head)
		{
			for (int i = 1; i <= index; i++)
			{
				current = current->next;
			}

			return current->data;
		}
		else
		{
			for (int i = size - 2; i >= index; i--)
			{
				current = current->previous;
			}

			return current->data;
		}
	}
	
	if (index == 0)
		return head->data;

	else if (index == size - 1)
		return tail->data;

	bool is_close_to_head = (index <= (size - index));
	Node* current = is_close_to_head ? head : tail;

	if (is_close_to_head)
	{
		for (int i = 1; i <= index; i++)
		{
			current = current->next;
		}

		return current->data;
	}
	else
	{
		for (int i = size - 2; i >= index; i--)
		{
			current = current->previous;
		}

		return current->data;
	}
}

template <typename T>
void DoublyLinkedList<T>::initialize_functions()
{
	if (is_thread_safe)
	{
		push_front = [this](const T& element) { thread_safe_push_front(element); };
		push_back = [this](const T& element) { thread_safe_push_back(element); };
		push_at = [this](const T& element, int index) { thread_safe_push_at(element, index); };
		
		pop_front  = [this]() { return this->thread_safe_pop_front(); };
		pop_back   = [this]() { return this->thread_safe_pop_back(); };
		pop_at     = [this](int index) { return this->thread_safe_pop_at(index); };

		clear      = [this]() { thread_safe_clear(); };
		
		get_head   = [this]() { return this->thread_safe_get_head(); };
		get_tail   = [this]() { return this->thread_safe_get_tail(); };
		at         = [this](int index) { return this->thread_safe_at(index); };

		get_size   = [this]() { return this->thread_safe_get_size(); };
		get_length = [this]() { return this->thread_safe_get_length(); };
	}
	else
	{
		push_front = [this](const T& element) { no_thread_safe_push_front(element); };
		push_back = [this](const T& element) { no_thread_safe_push_back(element); };
		push_at = [this](const T& element, int index) { no_thread_safe_push_at(element, index); };

		pop_front  = [this]() { return this->no_thread_safe_pop_front(); };
		pop_back   = [this]() { return this->no_thread_safe_pop_back(); };
		pop_at     = [this](int index) { return this->no_thread_safe_pop_at(index); };

		clear      = [this]() { no_thread_safe_clear(); };

		get_head   = [this]() { return this->no_thread_safe_get_head(); };
		get_tail   = [this]() { return this->no_thread_safe_get_tail(); };
		at         = [this](int index) { return this->no_thread_safe_at(index); };

		get_size   = [this]() { return this->no_thread_safe_get_size(); };
		get_length = [this]() { return this->no_thread_safe_get_length(); };
	}
}

template <typename T>
bool DoublyLinkedList<T>::is_empty()
{
	return (size == 0);
}

template <typename T>
void DoublyLinkedList<T>::thread_safe_push_front(const T& element)
{
	std::lock_guard<std::mutex> lock(mutex);

	Node* new_node = new Node(element);

	if (is_empty())
	{
		head = tail = new_node;
	}
	else
	{
		new_node->next = head;
		head->previous = new_node;
		head = new_node;
	}

	size++;
}

template <typename T>
void DoublyLinkedList<T>::no_thread_safe_push_front(const T& element)
{
	Node* new_node = new Node(element);

	if (is_empty())
	{
		head = tail = new_node;
	}
	else
	{
		new_node->next = head;
		head->previous = new_node;
		head = new_node;
	}

	size++;
}

template <typename T>
void DoublyLinkedList<T>::thread_safe_push_back(const T& element)
{
	std::lock_guard<std::mutex> lock(mutex);

	Node* new_node = new Node(element);

	if (is_empty())
	{
		tail = head = new_node;
	}
	else
	{
		new_node->previous = tail;
		tail->next = new_node;
		tail = new_node;
	}

	size++;
}

template <typename T>
void DoublyLinkedList<T>::no_thread_safe_push_back(const T& element)
{
	Node* new_node = new Node(element);

	if (is_empty())
	{
		tail = head = new_node;
	}
	else
	{
		new_node->previous = tail;
		tail->next = new_node;
		tail = new_node;
	}

	size++;
}

template <typename T>
void DoublyLinkedList<T>::thread_safe_push_at(const T& element, int index)
{
	std::lock_guard<std::mutex> lock(mutex);

	if (is_empty())
		throw std::out_of_range("Index out of range");

	if (index <= 0 || index >= size - 1)
		throw std::out_of_range("Index out of range");

	Node* new_node = new Node(element);

	bool is_close_to_head = (index <= (size - index));
	Node* current_node = is_close_to_head ? head : tail;
	Node* next_node;

	if (is_close_to_head)
	{
		for (int i = 1; i < index; i++)
		{
			current_node = current_node->next;
		}

		next_node = current_node->next;
		
		current_node->next = new_node;
		new_node->previous = current_node;
		new_node->next = next_node;
		next_node->previous = new_node;
	}
	else
	{
		for (int i = size - 2; i > index; i--)
		{
			current_node = current_node->previous;
		}

		next_node = current_node->next;

		current_node->next = new_node;
		new_node->previous = current_node;
		new_node->next = next_node;
		next_node->previous = new_node;
	}
	
	size++;
}

template <typename T>
void DoublyLinkedList<T>::no_thread_safe_push_at(const T& element, int index)
{
	if (is_empty())
		throw std::out_of_range("Index out of range");

	if (index <= 0 || index >= size - 1)
		throw std::out_of_range("Index out of range");

	Node* new_node = new Node(element);

	bool is_close_to_head = (index <= (size - index));
	Node* current_node = is_close_to_head ? head : tail;
	Node* next_node;

	if (is_close_to_head)
	{
		for (int i = 1; i < index; i++)
		{
			current_node = current_node->next;
		}

		next_node = current_node->next;

		current_node->next = new_node;
		new_node->previous = current_node;
		new_node->next = next_node;
		next_node->previous = new_node;
	}
	else
	{
		for (int i = size - 2; i > index; i--)
		{
			current_node = current_node->previous;
		}

		next_node = current_node->next;

		current_node->next = new_node;
		new_node->previous = current_node;
		new_node->next = next_node;
		next_node->previous = new_node;
	}

	size++;
}

template <typename T>
T DoublyLinkedList<T>::thread_safe_pop_front()
{
	std::lock_guard<std::mutex> lock(mutex);

	if (is_empty())
		throw std::out_of_range("Index out of range");

	head = head->next;
	head->previous = nullptr;

	size--;
}

template <typename T>
T DoublyLinkedList<T>::no_thread_safe_pop_front()
{
	if (is_empty())
		throw std::out_of_range("Index out of range");

	head = head->next;
	head->previous = nullptr;

	size--;
}

template <typename T>
T DoublyLinkedList<T>::thread_safe_pop_back()
{
	std::lock_guard<std::mutex> lock(mutex);

	if (is_empty())
		throw std::out_of_range("Index out of range");

	tail = tail->previous;
	tail->next = nullptr;

	size--;
}

template <typename T>
T DoublyLinkedList<T>::no_thread_safe_pop_back()
{
	if (is_empty())
		throw std::out_of_range("Index out of range");

	tail = tail->previous;
	tail->next = nullptr;

	size--;
}

template <typename T>
T DoublyLinkedList<T>::thread_safe_pop_at(int index)
{
	std::lock_guard<std::mutex> lock(mutex);

	if (is_empty())
		throw std::out_of_range("Index out of range");

	if (index <= 0 || index >= size - 1)
		throw std::out_of_range("Index out of range");

	bool is_close_to_head = (index <= (size - index));
	Node* current_node = is_close_to_head ? head : tail;
	Node* next_node;
	Node* previous_node;

	if (is_close_to_head)
	{
		for (int i = 1; i <= index; i++)
		{
			current_node = current_node->next;
		}

		previous_node = current_node->previous;
		next_node = current_node->next;

		previous_node->next = next_node;
		next_node->previous = previous_node;

		current_node = nullptr;
	}
	else
	{
		for (int i = 1; i <= index; i++)
		{
			current_node = current_node->next;
		}

		previous_node = current_node->previous;
		next_node = current_node->next;

		previous_node->next = next_node;
		next_node->previous = previous_node;

		current_node = nullptr;
	}

	size--;
}

template <typename T>
T DoublyLinkedList<T>::no_thread_safe_pop_at(int index)
{
	if (is_empty())
		throw std::out_of_range("Index out of range");

	if (index <= 0 || index >= size - 1)
		throw std::out_of_range("Index out of range");

	bool is_close_to_head = (index <= (size - index));
	Node* current_node = is_close_to_head ? head : tail;
	Node* next_node;
	Node* previous_node;

	if (is_close_to_head)
	{
		for (int i = 1; i <= index; i++)
		{
			current_node = current_node->next;
		}

		previous_node = current_node->previous;
		next_node = current_node->next;

		previous_node->next = next_node;
		next_node->previous = previous_node;

		current_node = nullptr;
	}
	else
	{
		for (int i = 1; i <= index; i++)
		{
			current_node = current_node->next;
		}

		previous_node = current_node->previous;
		next_node = current_node->next;

		previous_node->next = next_node;
		next_node->previous = previous_node;

		current_node = nullptr;
	}

	size--;
}

template <typename T>
void DoublyLinkedList<T>::thread_safe_clear()
{
	std::lock_guard<std::mutex> lock(mutex);

	for (Node* node = head; node; )
	{
		node = head->next;
		delete head;
		head = node;
	}

	size = 0;
}

template <typename T>
void DoublyLinkedList<T>::no_thread_safe_clear()
{
	for (Node* node = head; node; )
	{
		node = head->next;
		delete head;
		head = node;
	}

	size = 0;
}

template <typename T>
T DoublyLinkedList<T>::thread_safe_get_head()
{
	std::lock_guard<std::mutex> lock(mutex);

	return head->data;
}

template <typename T>
T DoublyLinkedList<T>::no_thread_safe_get_head()
{
	return head->data;
}

template <typename T>
T DoublyLinkedList<T>::thread_safe_get_tail()
{
	std::lock_guard<std::mutex> lock(mutex);

	return tail->data;
}

template <typename T>
T DoublyLinkedList<T>::no_thread_safe_get_tail()
{
	return tail->data;
}

template <typename T>
T DoublyLinkedList<T>::thread_safe_at(int index)
{
	std::lock_guard<std::mutex> lock(mutex);

	if (is_empty())
		throw std::out_of_range("Index out of range");

	else if (index == 0)
		return head->data;

	else if (index == size - 1)
		return tail->data;

	else if (index < 0 || index >= size)
		throw std::out_of_range("Index out of range");

	bool is_close_to_head = (index <= (size - index));
	Node* current = is_close_to_head ? head : tail;

	if (is_close_to_head)
	{
		for (int i = 1; i <= index; i++)
		{
			current = current->next;
		}

		return current->data;
	}
	else
	{
		for (int i = size - 2; i >= index; i--)
		{
			current = current->previous;
		}

		return current->data;
	}
}

template <typename T>
T DoublyLinkedList<T>::no_thread_safe_at(int index)
{
	if (is_empty())
		throw std::out_of_range("Index out of range");

	else if (index == 0)
		return head->data;

	else if (index == size - 1)
		return tail->data;

	else if (index < 0 || index >= size)
		throw std::out_of_range("Index out of range");

	bool is_close_to_head = (index <= (size - index));
	Node* current = is_close_to_head ? head : tail;

	if (is_close_to_head)
	{
		for (int i = 1; i <= index; i++)
		{
			current = current->next;
		}

		return current->data;
	}
	else
	{
		for (int i = size - 2; i >= index; i--)
		{
			current = current->previous;
		}

		return current->data;
	}
}

template <typename T>
size_t DoublyLinkedList<T>::thread_safe_get_size()
{
	std::lock_guard<std::mutex> lock(mutex);

	return size * sizeof(T);
}

template <typename T>
size_t DoublyLinkedList<T>::no_thread_safe_get_size()
{
	return size * sizeof(T);
}

template <typename T>
size_t DoublyLinkedList<T>::thread_safe_get_length()
{
	std::lock_guard<std::mutex> lock(mutex);

	return size;
}

template <typename T>
size_t DoublyLinkedList<T>::no_thread_safe_get_length()
{
	return size;
}

#endif