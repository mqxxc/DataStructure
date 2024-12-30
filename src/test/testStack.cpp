#pragma once
#include "testStack.h"
#include <iostream>

#include "../stack/MonotoneStack.hpp"
void Test::Test_MonotoneStack()
{
	MonotoneStack<int> stack(MonotoneStack<int>::eIncremental);

	std::cout << "is empty:" << stack.IsEmpty() << std::endl;
	stack.Push(31);
	stack.Push(1);
	stack.Push(100);
	stack.Push(2);
	stack.Push(7);
	
	std::cout << "is empty:" << stack.IsEmpty() << std::endl;
	std::cout << "size is:" << stack.Size() << std::endl;
	stack.TestOutPut();
	std::cout << std::endl;

	stack.Pop();

	stack.TestOutPut();
	std::cout << std::endl;
}