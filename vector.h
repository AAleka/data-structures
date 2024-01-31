#ifndef VECTOR_H
#define VECTOR_H

#include <functional>
#include <mutex>
#include <stdexcept>

template <typename T>
class Vector
{
private:
    size_t     capacity;
    size_t     size;
    int        increase;
    T*         elements;
    std::mutex mutex;

public:
    std::function<void(const T&)> push_back;
    std::function<T()>            pop;
    std::function<T(int)>         at;
    std::function<size_t()>       get_size;
    std::function<size_t()>       get_length;
    std::function<size_t()>       get_capacity;
    std::function<int()>       get_increase;

public:
    Vector(bool is_thread_safe);
    Vector(size_t capacity, bool is_thread_safe);
    Vector(size_t capacity, int increase, bool is_thread_safe);
    ~Vector();

    const T& operator[](int index) const;

private:
    void initialize_functions(bool is_thread_safe);
    void increase_capacity();

    bool is_full();
    bool is_empty();

    void thread_safe_push_back(const T& element);
    void no_thread_safe_push_back(const T& element);

    T thread_safe_pop();
    T no_thread_safe_pop();

    T thread_safe_at(int index);
    T no_thread_safe_at(int index);

    size_t thread_safe_size();
    size_t no_thread_safe_size();

    size_t thread_safe_length();
    size_t no_thread_safe_length();

    size_t thread_safe_capacity();
    size_t no_thread_safe_capacity();

    int thread_safe_increase();
    int no_thread_safe_increase();
};

template <typename T>
Vector<T>::Vector(bool is_thread_safe)
    : capacity(10), size(0), increase(capacity / 2), elements(new T[capacity])
{
    initialize_functions(is_thread_safe);
}

template <typename T>
Vector<T>::Vector(size_t capacity, bool is_thread_safe)
    : capacity(capacity), size(0), increase(capacity / 2), elements(new T[capacity])
{
    initialize_functions(is_thread_safe);
}

template <typename T>
Vector<T>::Vector(size_t capacity, int increase, bool is_thread_safe)
    : capacity(capacity), size(0), increase(increase), elements(new T[capacity])
{
    initialize_functions(is_thread_safe);
}

template <typename T>
Vector<T>::~Vector()
{
    delete[] elements;
}

template <typename T>
void Vector<T>::initialize_functions(bool is_thread_safe)
{
    if (is_thread_safe)
    {
        push_back    = [this](const T& element) { this->thread_safe_push_back(element); };
        pop          = [this]() { return this->thread_safe_pop(); };
        at           = [this](int index) { return this->thread_safe_at(index); };
        get_size     = [this]() { return this->thread_safe_size(); };
        get_length   = [this]() { return this->thread_safe_length(); };
        get_capacity = [this]() { return this->thread_safe_capacity(); };
    }
    else
    {
        push_back    = [this](const T& element) { this->no_thread_safe_push_back(element); };
        pop          = [this]() { return this->no_thread_safe_pop(); };
        at           = [this](int index) { return this->no_thread_safe_at(index); };
        get_size     = [this]() { return this->no_thread_safe_size(); };
        get_length   = [this]() { return this->no_thread_safe_length(); };
        get_capacity = [this]() { return this->no_thread_safe_capacity(); };
    }
}

template <typename T>
bool Vector<T>::is_full()
{
    return (size + 1 == capacity);
}

template <typename T>
bool Vector<T>::is_empty()
{
    return (size == 0);
}

template <typename T>
void Vector<T>::thread_safe_push_back(const T& element)
{
    std::lock_guard<std::mutex> lock(mutex);

    if (is_full())
        increase_capacity();

    elements[size++] = element;
}

template <typename T>
void Vector<T>::no_thread_safe_push_back(const T& element)
{
    if (is_full())
        increase_capacity();

    elements[size++] = element;
}

template <typename T>
T Vector<T>::thread_safe_pop()
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
T Vector<T>::no_thread_safe_pop()
{
    if (is_empty())
        throw std::out_of_range("Index out of range");

    size--;
    T popped_element = elements[size];
    elements[size] = T();

    return popped_element;
}

template <typename T>
T Vector<T>::thread_safe_at(int index)
{
    std::lock_guard<std::mutex> lock(mutex);

    if (index >= 0 && index < size)
        return elements[index];
    else
        throw std::out_of_range("Index out of range");
}

template <typename T>
T Vector<T>::no_thread_safe_at(int index)
{
    if (index >= 0 && index < size)
        return elements[index];
    else
        throw std::out_of_range("Index out of range");
}

template <typename T>
size_t Vector<T>::thread_safe_size()
{
    std::lock_guard<std::mutex> lock(mutex);

    return size * sizeof(T);
}

template <typename T>
size_t Vector<T>::no_thread_safe_size()
{
    return size * sizeof(T);
}

template <typename T>
size_t Vector<T>::thread_safe_capacity()
{
    std::lock_guard<std::mutex> lock(mutex);

    return capacity;
}

template <typename T>
size_t Vector<T>::no_thread_safe_capacity()
{
    return capacity;
}

template <typename T>
size_t Vector<T>::thread_safe_length()
{
    std::lock_guard<std::mutex> lock(mutex);

    return size;
}

template <typename T>
size_t Vector<T>::no_thread_safe_length()
{
    return size;
}

template <typename T>
int Vector<T>::thread_safe_increase()
{
    std::lock_guard<std::mutex> lock(mutex);

    return increase;
}

template <typename T>
int Vector<T>::no_thread_safe_increase()
{
    return increase;
}

template <typename T>
void Vector<T>::increase_capacity()
{
    capacity += increase;

    T* buffer_elements = new T[capacity];

    for (size_t i = 0; i < size; ++i)
        buffer_elements[i] = elements[i];

    delete[] elements;

    elements = buffer_elements;
}

template <typename T>
const T& Vector<T>::operator[](int index) const
{
    return at(index);
}

#endif