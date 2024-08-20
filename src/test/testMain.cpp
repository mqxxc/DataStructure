#pragma once
#include <iostream>


#include "../List/List.hpp"

void Print(List<int>* temp)
{
    int nCout = temp->Size();
    for (int i = 0; i < nCout; ++i)
    {
        std::cout << (*temp)[i] << "  ";
    }
    std::cout << std::endl;
}

int main_list()
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

#include "../queue/TwoWayQueue.hpp"
int main_TwoWayQueue()
{
    TwoWayQueue<int> queue;
    queue.Push(12, true);
    queue.Push(20, false);
    queue.Push(145, true);
    std::cout << "quenue size is " << queue.Size() << std::endl;
    std::cout << "quenue front is " << queue.peekFront() << std::endl;
    std::cout << "quenue rear is " << queue.peekRear() << std::endl;
    std::cout << "pop:" << queue.Pop(true) << std::endl;
    std::cout << "pop:" << queue.Pop(false) << std::endl;

    return 0;
}

#include "../queue/Queue.hpp"
int test_Queue()
{
    Queue<int> queue;

    queue.Push(12);
    queue.Push(25);
    queue.Push(35);

    std::cout << "queue size is :" << queue.Size() << std::endl;

    std::cout << "queue top unit is :" << queue.Peek() << std::endl;

    queue.Pop();
    std::cout << "queue size is :" << queue.Size() << std::endl;

    return 0; 
}


int main()
{
    return 0;
}