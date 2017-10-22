#pragma once


#include "../Allocators/GlobalAllocators.hpp"
#include "../Math/MathUtils.hpp"


namespace Ryujin
{
    template<typename T>
    class BinaryTreeBase
    {
    public:
        struct Element
        {
            T value;
            bool bValid;
            
            Element() : bValid(false) {}
        };
        
    protected:
        Element* flatTree;
        int32 treeCapacity;
        int32 nodeCount;
        
        void Resize(int32 desiredHeight)
        {
            // Resize enough to include an additional height in the tree
            int32 count = (1 << desiredHeight) - 1;
            Element* data = RYUJIN_NEW Element[count];
            Memory::MemCpy(data, flatTree, sizeof(Element) * treeCapacity);
            delete[] flatTree;
            flatTree = data;
            treeCapacity = count;
        }
        
    public:
        BinaryTreeBase()
        {
            treeCapacity = 7;
            nodeCount = 0;
            flatTree = RYUJIN_NEW Element[7];
        }
        
        BinaryTreeBase(const BinaryTreeBase<T>& other)
        {
            if (other.treeCapacity > treeCapacity)
            {
                SAFE_DELETE_ARRAY(flatTree);
                treeCapacity = other.treeCapacity;
                flatTree = RYUJIN_NEW Element[treeCapacity];
            }
            nodeCount = other.nodeCount;
            Memory::MemCpy(flatTree, other.flatTree, sizeof(Element) * other.nodeCount);
        }
        
        BinaryTreeBase(BinaryTreeBase<T>&& other)
        {
            if (other.treeCapacity > treeCapacity)
            {
                SAFE_DELETE_ARRAY(flatTree);
                treeCapacity = other.treeCapacity;
                flatTree = RYUJIN_NEW Element[treeCapacity];
            }
            nodeCount = other.nodeCount;
            flatTree = Move(other.flatTree);
            other.flatTree = nullptr;
        }
        
        VIRTUAL ~BinaryTreeBase()
        {
            SAFE_DELETE_ARRAY(flatTree);
            treeCapacity = 0;
            nodeCount = 0;
        }
        
        BinaryTreeBase<T>* operator=(const BinaryTreeBase<T>& other)
        {
            if (other.treeCapacity > treeCapacity)
            {
                SAFE_DELETE_ARRAY(flatTree);
                treeCapacity = other.treeCapacity;
                flatTree = RYUJIN_NEW Element[treeCapacity];
            }
            nodeCount = other.nodeCount;
            Memory::MemCpy(flatTree, other.flatTree, sizeof(Element) * other.nodeCount);
            return *this;
        }
        
        BinaryTreeBase<T>* operator=(const BinaryTreeBase<T>&& other)
        {
            if (other.treeCapacity > treeCapacity)
            {
                SAFE_DELETE_ARRAY(flatTree);
                treeCapacity = other.treeCapacity;
                flatTree = RYUJIN_NEW Element[treeCapacity];
            }
            nodeCount = other.nodeCount;
            flatTree = Move(other.flatTree);
            other.flatTree = nullptr;
            return *this;
        }
        
        FORCEINLINE const Element* GetData() const { return flatTree; }
        
        FORCEINLINE int32 GetHeight() const
        {
            if (treeCapacity == 0)
                return 0;
            
            return MathUtils::Log2(treeCapacity) - 1;
        }
        
        FORCEINLINE int32 GetParentIndex(int32 nodeIndex) const
        {
            return nodeIndex <= 0 ? 0 : (nodeIndex-1)/2;
        }
        
        FORCEINLINE int32 GetNodeCount() const { return nodeCount; }
        FORCEINLINE int32 GetMaxNodeCount() const { return treeCapacity; }
    };
    
    
    template<typename T>
    class BinarySearchTree : public BinaryTreeBase<T>
    {
    protected:
        void UpdateSubtreeAfterAdd(int32 childIndex, int32 grandChildIndex, int32 value)
        {
            T temp = this->flatTree[childIndex].value;
            this->flatTree[childIndex].value = value;
            if (temp <= value)
            {
                if (grandChildIndex % 2 == 0)
                    grandChildIndex--; // make it go down left tree
            }
            else
            {
                if (grandChildIndex % 2 != 0)
                    grandChildIndex++; // make it go down right tree
            }
            while (grandChildIndex < this->treeCapacity)
            {
                if (this->flatTree[grandChildIndex].bValid == false)
                {
                    this->flatTree[grandChildIndex].bValid = true;
                    this->flatTree[grandChildIndex].value = temp;
                    ++this->nodeCount;
                    return;
                }
                else
                {
                    T temp2 = this->flatTree[grandChildIndex].value;
                    this->flatTree[grandChildIndex].value = temp;
                    temp = temp2;
                    
                    if (temp <= this->flatTree[grandChildIndex].value)
                    {
                        // Proceed down left subtree
                        grandChildIndex = grandChildIndex * 2 + 1;
                    }
                    else
                    {
                        // Proceed down right subtree
                        grandChildIndex = grandChildIndex * 2 + 2;
                    }
                    if (grandChildIndex >= this->treeCapacity)
                    {
                        this->Resize(MathUtils::Log2(grandChildIndex) + 1);
                    }
                }
            }
        }
        
        bool IsValidBST_Internal(int32 nodeIndex, T*& prevNode) const
        {
            if (nodeIndex >= this->treeCapacity)
                return true;
            
            if (this->flatTree[nodeIndex].bValid == false)
                return true;
            
            if (!IsValidBST_Internal(nodeIndex * 2 + 1, prevNode))
                return false;
            
            if (prevNode && this->flatTree[nodeIndex].value < *prevNode)
                return false;
            
            prevNode = &this->flatTree[nodeIndex].value;
            
            return IsValidBST_Internal(nodeIndex * 2 + 2, prevNode);
        }
        
    public:
        BinarySearchTree()
        : BinaryTreeBase<T>()
        {
            
        }
        
        BinarySearchTree(const BinarySearchTree<T>& other)
        : BinaryTreeBase<T>(other)
        {
            
        }
        
        BinarySearchTree(BinarySearchTree<T>&& other)
        : BinaryTreeBase<T>(other)
        {
            
        }
        
        ~BinarySearchTree()
        {
            
        }
        
        BinarySearchTree<T>* operator=(const BinarySearchTree<T>& other)
        {
            BinaryTreeBase<T>::operator=(other);
            return *this;
        }
        
        BinarySearchTree<T>* operator=(const BinarySearchTree<T>&& other)
        {
            BinaryTreeBase<T>::operator=(other);
            return *this;
        }
        
        void Add(T value)
        {
            if (this->nodeCount == 0)
            {
                this->flatTree[0].value = value;
                this->flatTree[0].bValid = true;
                ++this->nodeCount;
                return;
            }
            
            if (this->nodeCount == this->treeCapacity)
            {
                this->Resize(this->GetHeight() + 1);
            }
            
            int32 i = 0;
            while (i < this->treeCapacity)
            {
                int32 childIndex = 2 * i + 1; // Assume we are going down left subtree unless the next check passes
                if (value > this->flatTree[i].value)
                {
                    // go down right subtree
                    childIndex++;
                }
                
                if (this->flatTree[childIndex].bValid == false)
                {
                    this->flatTree[childIndex].value = value;
                    this->flatTree[childIndex].bValid = true;
                    ++this->nodeCount;
                    return;
                }
                else
                {
                    int32 grandChild = childIndex * 2 + 1;
                    if (grandChild >= this->treeCapacity)
                    {
                        this->Resize(this->GetHeight() + 2);
                    }
                    
                    if (value <= this->flatTree[childIndex].value)
                    {
                        // Rotate left
                    }
                    else
                    {
                        // Rotate right
                        ++grandChild;
                    }
                    UpdateSubtreeAfterAdd(childIndex, grandChild, value);
                    
                    return;
                }
                
                i = childIndex;
            }
        }
        
        void Remove(T value)
        {
            int32 i = 0;
            bool bFoundValue = false;
            while (i < this->treeCapacity)
            {
                if (this->flatTree[i].value == value)
                {
                    bFoundValue = true;
                    break;
                }
                
                if (value <= this->flatTree[i].value)
                {
                    i = i * 2 + 1;
                }
                else
                {
                    i = i * 2 + 2;
                }
            }
            if (!bFoundValue)
                return;
            
            int32 child = i * 2 + 1;
            if (child >= this->treeCapacity)
                return;
            
            if (this->flatTree[child].bValid && this->flatTree[child].value <= value)
            {
                int32 indexOfValueToSwap = FindLargest(child);
                this->flatTree[i].value = this->flatTree[indexOfValueToSwap].value;
                this->flatTree[indexOfValueToSwap].bValid = false;
                --this->nodeCount;
            }
            else
            {
                ++child;
                int32 indexOfValueToSwap = FindSmallest(child);
                this->flatTree[i].value = this->flatTree[indexOfValueToSwap].value;
                this->flatTree[indexOfValueToSwap].bValid = false;
                --this->nodeCount;
            }
        }
        
        int32 FindLargest(int32 root) const
        {
            int32 index = root;
            int32 grandChild = index * 2 + 2;
            while (grandChild < this->treeCapacity)
            {
                if (this->flatTree[grandChild].bValid == false)
                {
                    return index;
                }
                index = grandChild;
                grandChild = grandChild * 2 + 2;
            }
            
            return index;
        }
        
        int32 FindSmallest(int32 root) const
        {
            int32 index = root;
            int32 grandChild = index * 2 + 1;
            while (grandChild < this->treeCapacity)
            {
                if (this->flatTree[grandChild].bValid == false)
                {
                    return index;
                }
                index = grandChild;
                grandChild = grandChild * 2 + 1;
            }
            
            return index;
        }
        
        bool IsValidBST() const
        {
            T* node = nullptr;
            return IsValidBST_Internal(0, node);
        }
        
        typename BinaryTreeBase<T>::Element* Find(T value) const
        {
            int32 i = 0;
            while (i < this->treeCapacity)
            {
                if (this->flatTree[i].value == value)
                {
                    return &this->flatTree[i];
                }
                
                if (value <= this->flatTree[i].value)
                {
                    i = i * 2 + 1;
                }
                else
                {
                    i = i * 2 + 2;
                }
            }
            return nullptr;
        }
    };
}
