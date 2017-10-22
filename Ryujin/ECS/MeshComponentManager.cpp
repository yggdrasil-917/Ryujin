

#include "MeshComponentManager.hpp"


namespace Ryujin
{
    const uint32 MeshComponentManager::INVALID_INDEX = NumericLimit<uint32>::Max();
    const MeshComponentManager::Instance MeshComponentManager::INVALID_INSTANCE = { INVALID_INDEX };
}