#include <iostream>
#include "vector.h"
#include "stack.h"
#include "queue.h"
#include "doublylinkedlist.h"

void test_vector();
void test_stack();
void test_queue();
void test_doublylinkedlist();

int main() 
{
    test_vector();
    test_stack();
    test_queue();
    test_doublylinkedlist();

    return 0;
}

void test_vector()
{
    std::cout << "\n---------------------------------\nVector\n";

    Vector<int> vec(false);
    vec.push_back(1);
    vec.push_back(2);
    vec.push_back(3);
    vec.push_back(4);
    vec.push_back(5);
    vec.push_back(6);
    vec.push_back(7);
    vec.push_back(8);
    vec.push_back(9);
    vec.push_back(10);
    vec.push_back(11);

    std::cout << "Elements: ";
    for (int i = 0; i < vec.get_length(); i++)
        std::cout << vec[i] << '\t';

    std::cout << '\n';

    std::cout << "Capacity: " << vec.get_capacity() << '\n';
    std::cout << "Size: " << vec.get_size() << '\n';
    std::cout << "Length: " << vec.get_length() << '\n';
    std::cout << "---------------------------------\n";
}

void test_stack()
{
    std::cout << "\n---------------------------------\nStack\n";

    Stack<char> stack(true);
    stack.push('a');
    stack.push('b');
    stack.push('c');
    stack.push('d');
    stack.push('e');
    stack.push('f');
    stack.pop();

    std::cout << "Top: " << stack.top() << '\n';

    std::cout << "Elements: ";
    for (int i = 0; i < stack.get_length(); i++)
        std::cout << stack[i] << '\t';

    std::cout << '\n';

    std::cout << "Capacity: " << stack.get_capacity() << '\n';
    std::cout << "Size: " << stack.get_size() << '\n';
    std::cout << "Length: " << stack.get_length() << '\n';
    std::cout << "---------------------------------\n";
}

void test_queue()
{
    std::cout << "\n---------------------------------\nQueue\n";

    Queue<std::string> queue(true);
    queue.push("hello!");
    queue.push("how are you?");
    queue.push("what are you doing?");
    queue.push("where are you?");
    queue.push("When are you coming?");
    queue.push("When are you leaving?");
    queue.pop();

    std::cout << "Front: " << queue.front() << '\n';
    std::cout << "Back: " << queue.back() << '\n';
    std::cout << "First: " << queue.at(0) << '\n';

    std::cout << "Elements: ";
    for (int i = 0; i < queue.get_length(); i++)
        std::cout << queue[i] << '\t';

    std::cout << '\n';

    std::cout << "Size: " << queue.get_size() << '\n';
    std::cout << "Length: " << queue.get_length() << '\n';
    std::cout << "---------------------------------\n";
}

void test_doublylinkedlist()
{
    std::cout << "\n---------------------------------\nDoubly Linked List\n";

    DoublyLinkedList<double> list(false);

    list.push_front(0.2);
    list.push_front(0.1);
    list.push_back(0.4);
    list.push_back(0.5);
    list.push_at(0.3, 2);
    list.push_back(0.6);
    list.push_front(0.0);
    list.pop_front();
    list.pop_back();
    list.pop_at(2);

    std::cout << "Head: " << list.get_head() << '\n';
    std::cout << "Tail: " << list.get_tail() << '\n';
    std::cout << "First: " << list.at(0) << '\n';

    std::cout << "Elements: ";
    for (int i = 0; i < list.get_length(); i++)
        std::cout << list[i] << '\t';

    std::cout << '\n';

    std::cout << "Size: " << list.get_size() << '\n';
    std::cout << "Length: " << list.get_length() << '\n';
    std::cout << "---------------------------------\n";
}