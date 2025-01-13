#pragma once
#include "testQueue.h"

#include "../queue/AnnularQueue.hpp"
void Test::test_annularQueue()
{
    AnnularQueue<int> queue;
    queue.Enque(1);
    queue.Enque(2);
    queue.Enque(3);
    std::cout << "Front:" << queue.Front() << std::endl;
    std::cout << "Rear:" << queue.Rear() << std::endl;
    //std::cout << "deque:" << queue.Deque() << std::endl;

    queue.Enque(15);

    std::cout << "Front:" << queue.Front() << std::endl;
    std::cout << "maxsize:" << queue.Size() << std::endl;

    std::cout << "size:" << queue.Count() << std::endl;
    std::cout << "isEmpty" << queue.Empty() << std::endl;
    std::cout << "isFull" << queue.Full() << std::endl;


    queue.Print_test();
}

#include "../queue/Queue.hpp"
int Test::test_Queue()
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

#include "../queue/TwoWayQueue.hpp"
int Test::Test_TwoWayQueue()
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

#include "../queue/PriorityQueue.hpp"
int Test::Test_PriorityQueue()
{
    PriorityQueue<int> queue(5, PriorityQueue<int>::eIncremental);
    queue.Enque(12);
    queue.Enque(2);
    queue.Enque(60);
    queue.Enque(8);
    queue.Enque(18);
    int op = queue.Deque();
    queue.Enque(20);
    queue.Enque(17);
    queue.Enque(3);
    queue.Enque(13);
    std::cout << "quenue size is " << queue.Size() << std::endl;
    std::cout << "quenue count is " << queue.Count() << std::endl;
    std::cout << "quenue front is " << queue.Top() << std::endl;
    queue.Print_test();
    return 0;
}

