

#include "../RyujinUnitTesting/RyujinUnitTesting.hpp"
#include "../RyujinUnitTesting/TestCase.hpp"

#include "RyujinCore/Threading/Fiber.hpp"
#include "RyujinCore/CoreUtils/SmartPointer.hpp"
#include "RyujinCore/OS/Application.hpp"

#include "RyujinCore/Serialization/TextFilePupper.hpp"
#include "RyujinCore/Serialization/BinaryFilePupper.hpp"
#include "RyujinExtensions/EntitySystem/Entity.hpp"
#include "RyujinExtensions/EntitySystem/TransformComponent.hpp"

#include "RyujinExtensions/Utils/SymbolLocator.hpp"
#include "Ryujin/Ryujin/testMaterial.hpp"


#include "RyujinExtensions/AssetManagement/AssetRegistry.hpp"
#include "RyujinExtensions/AssetManagement/AudioResource.hpp"
#include "RyujinCore/Threading/JobSystem.hpp"

#include "RyujinCore/CoreUtils/ConfigCache.hpp"
#include "RyujinCore/Allocators/MemoryPool.hpp"



namespace Ryujin
{
    TEST_CASE(ConfigCache2Test, "ConfigCache2Test")
    {
        ConfigCache cache;
        REQUIRE(cache.LoadConfig("options.ini"));
        
        int32 width, height;
        cache.GetInt("Viewport", "width", width, 1);
        cache.GetInt("Viewport", "height", height, 1);
        REQUIRE(width == 640 && height == 360);
    }
    
    TEST_CASE(MemoryPoolTest, "MemoryPoolTest")
    {
        struct PoolEntry
        {
            int32 i;
        };
        const int32 count = 5;
        MemoryPool<PoolEntry> pool;
        pool.SetPoolSize(count);
        PoolEntry* entries[count];
        
        REQUIRE(pool.GetPoolSizeInKb() == count * sizeof(PoolEntry) / 1024.0f);
        
        for (int32 i = 0; i < count; ++i)
        {
            entries[i] = pool.Allocate();
            REQUIRE(entries[i]);
            entries[i]->i = i;
        }
        
        pool.Free(entries[2]);
        entries[2] = pool.Allocate();
        REQUIRE(entries[2]);
    }
    
//    TEST_CASE(AssetRegistryBasicTest, "Basic test for asset registry")
//    {
//        AssetRegistry registry;
//        AssetDescriptor assetDesc;
//        AudioAssetDesc desc;
//        desc.filepath = "../../../Content/Audio/swvader03.wav";
//        assetDesc.Setup<AudioAssetDesc>("testAudio", AudioResource::GetFactoryId(), desc);
//        {
//        AssetPtr owner;
//        assetDesc.owner = &owner;
//        DynamicArray<AssetDescriptor> descs { assetDesc };
//        JobSystem jobSystem;
//        
//        jobSystem.Initialize(Threading::NumConcurrentThreadsSupported() - 1);
//        
//        JobId jobId = registry.LoadAssetsAsync(&jobSystem, descs);
//        jobSystem.Schedule(jobId);
//        jobSystem.Wait(jobId);
//        AudioResource* r = owner.Cast<AudioResource>();
//        
//        jobSystem.Shutdown();
//        }
//        
//        Threading::Sleep(ResourceBase::MAX_INACTIVE_DURATION + 100);
//        registry.Tick(ResourceBase::MAX_INACTIVE_DURATION + 100);
//    }
    
    TEST_CASE(RyujinComponentBinarySerialization, "Test for binary serialization of Ryujin components")
    {
        RyujinEntity entity;
        Transform t;
        uint32 tagIndex = entity.AddComponent(RYUJIN_NEW TagComponent("test tag"));
        uint32 transformIndex = entity.AddComponent(RYUJIN_NEW TransformComponent(t));
        
        String testDataFile(String::Printf("%stest.dat", AppInfo::GetDevCookedContentDir()));
        
        FileHandle file = File::Open(*testDataFile, FileMode::FM_Write, true);
        REQUIRE(file != nullptr);
        UniquePtr<BinaryFilePupper> p(RYUJIN_NEW BinaryFilePupper(file, Pupper::IoMode::Write));
        VarInfo v("entity");
        Pup(p, entity, v);
        File::Close(file);
        
        FileHandle file2 = File::Open(*testDataFile, FileMode::FM_Read, true);
        REQUIRE(file2 != nullptr);
        UniquePtr<BinaryFilePupper> p2(RYUJIN_NEW BinaryFilePupper(file, Pupper::IoMode::Read));
        RyujinEntity e2;
        Pup(p2, e2, v);
        File::Close(file2);
        
        REQUIRE(e2.GetComponentCount() == 2);
        TagComponent* tag = e2.GetComponent<TagComponent>(tagIndex);
        TransformComponent* transform = e2.GetComponent<TransformComponent>(transformIndex);
        REQUIRE(tag != nullptr);
        REQUIRE(transform != nullptr);
        
        String testTag("test tag");
        REQUIRE(testTag == tag->GetTag());
        
        Vector3 translation = t.GetTranslation();
        Vector3 translationFromDisk = transform->GetTransform().GetTranslation();
        
        Vector3 scale = t.GetScale();
        Vector3 scaleFromDisk = transform->GetTransform().GetScale();
        
        Quaternion q = t.GetRotation();
        Quaternion quatFromDisk = transform->GetTransform().GetRotation();
        for (int32 i = 0; i < 3; ++i)
        {
            REQUIRE(MathUtils::Abs(translation[i] - translationFromDisk[i]) <= 0.0001f);
            REQUIRE(MathUtils::Abs(scale[i] - scaleFromDisk[i]) <= 0.0001f);
            REQUIRE(MathUtils::Abs(q.v[i] - quatFromDisk.v[i]) <= 0.0001f);
        }
        
        REQUIRE(MathUtils::Abs(q.w - quatFromDisk.w) <= 0.0001f);
    }
    
    TEST_CASE(RyujinComponentTextSerialization, "Test for text serialization of Ryujin components")
    {
        RyujinEntity entity;
        Transform t(Vector3(10.0f, 1.0, -10.0f));
        uint32 tagIndex = entity.AddComponent(RYUJIN_NEW TagComponent("test tag"));
        uint32 transformIndex = entity.AddComponent(RYUJIN_NEW TransformComponent(t));
        String testDataFile(String::Printf("%stest.txt", AppInfo::GetDevCookedContentDir()));
        
        FileHandle file = File::Open(*testDataFile, FileMode::FM_Write);
        REQUIRE(file != nullptr);
        UniquePtr<TextFilePupper> p(RYUJIN_NEW TextFilePupper(file, Pupper::IoMode::Write));
        VarInfo v("entity");
        Pup(p, entity, v);
        File::Close(file);
        
        FileHandle file2 = File::Open(*testDataFile, FileMode::FM_Read);
        REQUIRE(file2 != nullptr);
        UniquePtr<TextFilePupper> p2(RYUJIN_NEW TextFilePupper(file, Pupper::IoMode::Read));
        RyujinEntity e2;
        Pup(p2, e2, v);
        File::Close(file2);
        
        REQUIRE(e2.GetComponentCount() == 2);
        TagComponent* tag = e2.GetComponent<TagComponent>(tagIndex);
        TransformComponent* transform = e2.GetComponent<TransformComponent>(transformIndex);
        REQUIRE(tag != nullptr);
        REQUIRE(transform != nullptr);
        
        String testTag("test tag");
        REQUIRE(testTag == tag->GetTag());
        
        Vector3 translation = t.GetTranslation();
        Vector3 translationFromDisk = transform->GetTransform().GetTranslation();
        
        Vector3 scale = t.GetScale();
        Vector3 scaleFromDisk = transform->GetTransform().GetScale();
        
        Quaternion q = t.GetRotation();
        Quaternion quatFromDisk = transform->GetTransform().GetRotation();
        for (int32 i = 0; i < 3; ++i)
        {
            REQUIRE(MathUtils::Abs(translation[i] - translationFromDisk[i]) <= 0.0001f);
            REQUIRE(MathUtils::Abs(scale[i] - scaleFromDisk[i]) <= 0.0001f);
            REQUIRE(MathUtils::Abs(q.v[i] - quatFromDisk.v[i]) <= 0.0001f);
        }
        
        REQUIRE(MathUtils::Abs(q.w - quatFromDisk.w) <= 0.0001f);
    }
    
    TEST_CASE(LookupSymbolTest, "Test for lookup symbol")
    {
        TableOfContent toc;
        REQUIRE(toc.InitializeFromFile("../../../Content/DataCompiler/testMaterial.dat"));
        TestMaterial* test = toc.LookupSymbol<TestMaterial>("red");
        REQUIRE(test != nullptr);
        
        REQUIRE(test->albedo[0] == 1.0f);
        REQUIRE(test->albedo[1] == 0.0f);
        REQUIRE(test->albedo[2] == 0.0f);
        String name(test->name);
        REQUIRE(name == "test1");
        
        SAFE_DELETE(test);
    }
    
    class FiberTestClass : public Fiber
    {
    protected:
        void Run() OVERRIDE FINAL
        {
            count = 0;
            for (int32 i = 0; i < 5; ++i)
            {
//                Debugf(Info, "FiberTestClass %d", i);
                FiberYield();
                ++count;
            }
        }
        
    public:
        persist int32 count;
    };
    
    struct StacklessCoroutineExample : public StacklessCoroutine
    {
        int32 i;
        StacklessCoroutineExample() : i(0) {}
        
        void Main() OVERRIDE FINAL
        {
            STACKLESS_COROUTINE_START();
            ++i;
            STACKLESS_COROUTINE_YIELD();
            ++i;
            STACKLESS_COROUTINE_YIELD();
            ++i;
            STACKLESS_COROUTINE_END();
        }
    };
    
    TEST_CASE(StacklessCoroutineTest, "Stackless coroutine test")
    {
        StacklessCoroutineExample c;
        int32 i = 0;
        while (c.done == false)
        {
            c.Main();
            ++i;
            REQUIRE(c.i == i);
        }
    }
    
    TEST_CASE(FiberTest, "Test for fibers")
    {
        Fiber::EnableFibersInCurrentThread();
        FiberTestClass fiberTest;
        while (fiberTest.Resume())
        {
            REQUIRE(fiberTest.count != 5);
        }
        Fiber::DisableFibersInCurrentThread();
    }
}
