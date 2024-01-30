#include "vector.h"

int main() 
{
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
        std::cout << vec[i] << ' ';

    std::cout << '\n';

    std::cout << "Capacity: " << vec.get_capacity() << '\n';
    std::cout << "Size: " << vec.get_size() << '\n';
    std::cout << "Length: " << vec.get_length() << '\n';

    return 0;
}