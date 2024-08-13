#include <iostream>
#include "List/List.hpp"

void Print(List<int>* temp)
{
    int nCout = temp->Size();
    for (int i = 0; i < nCout; ++i)
    {
        std::cout << (*temp)[i] << "  ";
    }
    std::cout << std::endl;
}

int main()
{
    List<int> list;
    list.Append(1);
    list.Append(12);
    list.Append(13);
    Print(&list);
    list.Insert(1, 90);
    Print(&list);
    list.Remove(2);
    Print(&list);
    std::cout << list.IndexOf(90)<<std::endl;
    std::cout << list.IndexOf(80) << std::endl;
    return 0;
}