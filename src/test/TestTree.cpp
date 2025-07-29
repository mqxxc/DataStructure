#include "TestTree.h"
#include "../Tree/BTree.hpp"

#include <iostream>
#include <chrono>
using namespace std::chrono;

using namespace std;

void Test::Test_BTree()
{
	BTree<int> tree;

    int keys_to_insert[] = {
    10,    // 插入第一个 key
    20,    // 同一节点插入不分裂
    5,     // 插入最左侧，不分裂
    6,     // 触发叶子节点分裂（4个key）
    12,    // 插入另一侧叶子，检查正常插入
    30,    // 继续向右插入
    7,     // 插入中间位置
    17,    // 插入中间叶子，接近再次分裂
    3,     // 插入最左，可能导致根被提升
    1,     // 插入最左叶子再分裂，观察分裂传播
    8,     // 插入中间，使中间分裂
    9,     // 引发中间节点的复杂分裂
    4,     // 深层插入
    2,     // 补充完整最左侧
    11,    // 插入已有中间节点
    13,    // 插入，准备触发新的内部分裂
    14,    // 内部节点刚好满，测试是否分裂正确
    15,    // 触发内层递归分裂
    16,    // 检查是否正确分裂、树高是否增长
    18     // 最后一个插入，确保树结构平衡
    };


    //auto end = high_resolution_clock::now();

	for (int i = 0; i < sizeof(keys_to_insert) / sizeof(int); i++)
		tree.Insert(keys_to_insert[i]);

    

	/*auto duration = duration_cast<nanoseconds>(end - start);
	std::cout << "耗时: " << duration.count() << " ns" << std::endl;*/

	//tree.Printf();

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
