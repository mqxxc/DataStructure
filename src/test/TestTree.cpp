#include "TestTree.h"
#include "../Tree/BTree.hpp"
#include "../tree/BPlusTree.hpp"

#include <iostream>
#include <chrono>
using namespace std::chrono;

using namespace std;

void Test::Test_BTree()
{
	BTree<int, 6> tree;

    int keys_to_insert[] = {
    10,
    20,
    5,
    6,
    12,
    30,
    7,
    17,
    3,
    1,
    8,
    9,
    4,
    2,
    11,
    13,
    14,
    15,
    16,
    18
    };


    //auto end = high_resolution_clock::now();

	for (int i = 0; i < sizeof(keys_to_insert) / sizeof(int); i++)
		tree.Insert(keys_to_insert[i]);



	/*auto duration = duration_cast<nanoseconds>(end - start);
	std::cout << "耗时: " << duration.count() << " ns" << std::endl;*/

	tree.Printf();

    // 简单删除，不触发重平衡
    tree.DeleteAt(8);

    tree.DeleteAt(11);

    tree.DeleteAt(30);

    tree.Insert(11);
    tree.DeleteAt(11);

    tree.DeleteAt(13);

    tree.DeleteAt(5);
    tree.DeleteAt(4);

    tree.DeleteAt(14);

    tree.DeleteAt(10);

    tree.DeleteAt(3);
    tree.DeleteAt(6);
    tree.DeleteAt(12);
    tree.DeleteAt(16);
    tree.DeleteAt(20);

    tree.Printf();


	//cout << "find 6:" << (tree.Find(6) ? "find!!" : "no find!!");
    //cout << "find 0:" << (tree.Find(0) ? "find!!" : "no find!!");
}

void Test::Test_BPlusTree()
{
    BPlusTree<int, 6> tree;

    int keys_to_insert[] = {
        10,
        20,
        5,
        6,
        12, 
        30,
        7,
        17,
        3,
        1,
        8,
        9, 
        4,
        2,
        11,
        13,
        14,
        15,
        16,
        18
    };


    //auto end = high_resolution_clock::now();

    for (int i = 0; i < sizeof(keys_to_insert) / sizeof(int); i++)
        tree.Insert(keys_to_insert[i]);

    /* tree.Printf();

     tree.PrintfForLeaf();*/

    //return;
    /*auto duration = duration_cast<nanoseconds>(end - start);
    std::cout << "耗时: " << duration.count() << " ns" << std::endl;*/

    // 简单删除，不触发重平衡
    tree.DeleteAt(8);

    tree.DeleteAt(11);

    tree.DeleteAt(30);

    tree.Insert(11);
    tree.DeleteAt(11);

    tree.DeleteAt(13);

    tree.DeleteAt(5);

    tree.DeleteAt(4);

    tree.DeleteAt(14);

    tree.DeleteAt(10);

    tree.DeleteAt(3);

    tree.DeleteAt(6);

    tree.DeleteAt(12);

    tree.DeleteAt(16);

    tree.DeleteAt(20);

    tree.Printf();

    tree.PrintfForLeaf();

    //cout << "find 6:" << (tree.Find(6) ? "find!!" : "no find!!");
    //cout << "find 0:" << (tree.Find(0) ? "find!!" : "no find!!");
}
