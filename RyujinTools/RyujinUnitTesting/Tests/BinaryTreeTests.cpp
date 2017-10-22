
#include "../RyujinUnitTesting/RyujinUnitTesting.hpp"
#include "../RyujinUnitTesting/TestCase.hpp"

#include "RyujinCore/Containers/BinaryTree.hpp"


namespace Ryujin
{
    TEST_CASE(BinaryTreeHeightTest, "BinaryTreeHeightTest")
    {
        BinarySearchTree<int32> tree;
        tree.Add(0);
        tree.Add(1);
        tree.Add(2);
        tree.Add(0);
        tree.Add(6);
        tree.Add(-2);
        tree.Add(4);
        tree.Add(5);
        
        REQUIRE(tree.GetHeight() == 3);
        REQUIRE(tree.IsValidBST());
    }
    
    TEST_CASE(BinarySearchTreeFindTest, "BinarySearchTreeFindTest")
    {
        BinarySearchTree<int32> tree;
        tree.Add(0);
        tree.Add(1);
        tree.Add(2);
        tree.Add(0);
        tree.Add(6);
        tree.Add(-2);
        tree.Add(4);
        tree.Add(5);
        
        typename BinaryTreeBase<int32>::Element* element = tree.Find(1);
        REQUIRE(element);
        REQUIRE(element->value == 1);
        REQUIRE(tree.IsValidBST());
    }
    
    TEST_CASE(BinarySearchTreeRemoveTest, "BinarySearchTreeRemoveTest")
    {
        BinarySearchTree<int32> tree;
        tree.Add(0);
        tree.Add(1);
        tree.Add(2);
        tree.Add(0);
        tree.Add(6);
        tree.Add(-2);
        tree.Add(4);
        tree.Add(5);
        
        tree.Remove(0);
        tree.Remove(-2);
        tree.Remove(5);
//                const BinaryTreeBase<int32>::Element* data = tree.GetData();
//                for (int32 i = 0; i < tree.GetMaxNodeCount(); ++i)
//                {
//                    Debugf(Info, "%d", data[i].bValid ? data[i].value : 99999);
//                }
        REQUIRE(tree.IsValidBST());
    }
}
