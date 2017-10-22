
#ifndef COMMAND_BUCKET_HPP
#define COMMAND_BUCKET_HPP


#include "RyujinCore/Allocators/LinearAllocator.hpp"

#include "RendererCommands.hpp"
#include "RenderCommandSortKey.hpp"

#include <algorithm>

namespace Ryujin
{
    
    namespace CommandPacketUtils
    {
        constexpr static const PtrSize OFFSET_NEXT_COMMAND_PACKET = 0u;
        constexpr static const PtrSize OFFSET_BACKEND_DISPATCH_FUNCTION = OFFSET_NEXT_COMMAND_PACKET + sizeof(CommandPacket);
        constexpr static const PtrSize OFFSET_COMMAND = OFFSET_BACKEND_DISPATCH_FUNCTION + sizeof(BackendDispatchFunction);
        
        
        template<class T>
        static PtrSize GetSize(PtrSize additionalMemoryToAllocate)
        {
            return OFFSET_COMMAND + sizeof(T) + additionalMemoryToAllocate;
        }
        
        template<class T>
        static CommandPacket Create(RyujinAllocator<LinearAllocator>* allocator, PtrSize additionalMemoryToAllocate)
        {
            return allocator->Allocate(GetSize<T>(additionalMemoryToAllocate));
        }
        
        static CommandPacket* GetNextCommandPacket(CommandPacket packet)
        {
            return reinterpret_cast<CommandPacket*>(reinterpret_cast<char*>(packet) + OFFSET_NEXT_COMMAND_PACKET);
        }
        
        template <typename T>
        static CommandPacket* GetNextCommandPacket(T* command)
        {
            return reinterpret_cast<CommandPacket*>(reinterpret_cast<char*>(command) - OFFSET_COMMAND + OFFSET_NEXT_COMMAND_PACKET);
        }
        
        static BackendDispatchFunction* GetBackendDispatchFunction(CommandPacket packet)
        {
            return reinterpret_cast<BackendDispatchFunction*>(reinterpret_cast<char*>(packet) + OFFSET_BACKEND_DISPATCH_FUNCTION);
        }
        
        template <typename T>
        static T* GetCommand(CommandPacket packet)
        {
            return reinterpret_cast<T*>(reinterpret_cast<char*>(packet) + OFFSET_COMMAND);
        }
        
        template <typename T>
        static char* GetAuxiliaryMemory(T* command)
        {
            return reinterpret_cast<char*>(command) + sizeof(T);
        }
        
        static void StoreNextCommandPacket(CommandPacket packet, CommandPacket nextPacket)
        {
            *GetNextCommandPacket(packet) = nextPacket;
        }
        
        template <typename T>
        static void StoreNextCommandPacket(T* command, CommandPacket nextPacket)
        {
            *GetNextCommandPacket<T>(command) = nextPacket;
        }
        
        static void StoreBackendDispatchFunction(CommandPacket packet, BackendDispatchFunction dispatchFunction)
        {
            *GetBackendDispatchFunction(packet) = dispatchFunction;
        }
        
        static const CommandPacket LoadNextCommandPacket(const CommandPacket packet)
        {
            return *GetNextCommandPacket(packet);
        }
        
        static const BackendDispatchFunction LoadBackendDispatchFunction(const CommandPacket packet)
        {
            return *GetBackendDispatchFunction(packet);
        }
        
        static const void* LoadCommand(const CommandPacket packet)
        {
            return reinterpret_cast<char*>(packet) + OFFSET_COMMAND;
        }
    }
    
    
    template<typename Key>
    class CommandBucket
    {
    private:
        DynamicArray<SortCommand<Key>, SceneRenderingAllocator> sortCommands;
        RyujinAllocator<LinearAllocator>* allocator;
        
        Renderer* renderer;
        Mutex mtx;
        
    public:
        CommandBucket(Renderer* inRenderer, PtrSize bytesToAllocate = Memory::ByteConstant::OneKiloByte, const char* bucketName = "CommandBucket");
        ~CommandBucket();
        
        void Submit();
        void Clear();
        
        template<class T>
        T* AddCommand(Key key, PtrSize extraMemoryToAllocate = 0);
        
        template<class U, class V>
        U* AppendCommand(V* command, PtrSize extraMemoryToAllocate = 0);
        
        template<class Compare>
        void Sort(Compare compare)
        {
            std::sort(sortCommands.begin(), sortCommands.end(), compare);
        }
        
        void Sort()
        {
            RadixSort<SortCommand<Key>, Key>(sortCommands.GetTypedData(), sortCommands.GetSize());
        }
    };
    
#include "CommandBucket.inl"
}


#endif
