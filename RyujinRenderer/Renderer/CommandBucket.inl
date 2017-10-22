
#include "CommandBucket.hpp"

#include <algorithm>

template<typename Key>
CommandBucket<Key>::CommandBucket(Renderer* inRenderer, PtrSize bytesToAllocate, const char* bucketName)
:
renderer(inRenderer)
{
    allocator = RYUJIN_NEW RyujinAllocator<LinearAllocator>(bytesToAllocate, bucketName);
}


template<typename Key>
CommandBucket<Key>::~CommandBucket()
{
    SAFE_DELETE(allocator);
}

template<typename Key>
void CommandBucket<Key>::Submit()
{
//    uint8* allocatorStart = allocator->GetStart();
    for (auto&& sortCommand : sortCommands)
    {
//        CommandPacket packet = (allocatorStart + sortCommand.offset);
        CommandPacket packet = sortCommand.command;
        while (packet)
        {
            const BackendDispatchFunction function = CommandPacketUtils::LoadBackendDispatchFunction(packet);
            const void* command = CommandPacketUtils::LoadCommand(packet);
            function(renderer, command);
            
            packet = CommandPacketUtils::LoadNextCommandPacket(packet);
        }
    }
    
    allocator->Reset();
    sortCommands.RemoveAll();
}

template<typename Key>
template<class T>
T* CommandBucket<Key>::AddCommand(Key key, PtrSize extraMemoryToAllocate)
{
    CommandPacket command;
    
    {
        //TODO: remove the mutex lock
        ScopedLock scopedLock(mtx);
        command = CommandPacketUtils::Create<T>(allocator, extraMemoryToAllocate);
    }
    
    SortCommand<Key> sortCommand;
    sortCommand.key = key;
//    sortCommand.offset = allocator->GetBytesAllocated();
    sortCommand.command = command;
    sortCommands.Add(sortCommand);
    
    
    CommandPacketUtils::StoreNextCommandPacket(command, nullptr);
    CommandPacketUtils::StoreBackendDispatchFunction(command, T::DispatchFunction);
    
    
    return CommandPacketUtils::GetCommand<T>(command);
}


template<typename Key>
template<class U, class V>
U* CommandBucket<Key>::AppendCommand(V* command, PtrSize extraMemoryToAllocate)
{
    CommandPacket packet;
    
    {
        //TODO: remove the mutex lock
        ScopedLock scopedLock(mtx);
        packet = CommandPacketUtils::Create<V>(allocator, extraMemoryToAllocate);
    }
    
    CommandPacketUtils::StoreNextCommandPacket<V>(command, packet);
    
    CommandPacketUtils::StoreNextCommandPacket(packet, nullptr);
    CommandPacketUtils::StoreBackendDispatchFunction(packet, U::DispatchFunction);
    
    return CommandPacketUtils::GetCommand<U>(packet);
}