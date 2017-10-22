
#pragma once

#include "../Logger/Logger.hpp"
#include "../Threading/Thread.hpp"


namespace Ryujin
{
    template<typename T>
    class UniquePtr
    {
    private:
        T* obj;
        
    public:
        UniquePtr()
        : obj(nullptr)
        {
            
        }
        
        explicit UniquePtr(T* inObj)
        : obj(inObj)
        {
            
        }

		UniquePtr(const UniquePtr<T>& other)
		{
			SAFE_DELETE(obj);
			obj = RYUJIN_NEW T(*other);
		}
        
        UniquePtr(UniquePtr<T>&& other)
        {
            obj = Move(other.obj);
            other.obj = nullptr;
        }
        
        UniquePtr<T>& operator=(UniquePtr<T>&& other)
        {
            SAFE_DELETE(obj);
            obj = Move(other.obj);
            other.obj = nullptr;
            
            return *this;
        }

		UniquePtr<T>& operator=(UniquePtr<T>& right)
		{
			SAFE_DELETE(obj);
			obj = RYUJIN_NEW T(right->obj);

			return *this;
		}
        
        UniquePtr<T>& operator=(T* right)
        {
            SAFE_DELETE(obj);
            obj = right;
            
            return *this;
        }
        
        ~UniquePtr()
        {
            SAFE_DELETE(obj);
        }
        
        T* Release()
        {
            T* ptr = obj;
            obj = nullptr;
            return ptr;
        }
        
        FORCEINLINE T* Get() const
        {
            return obj;
        }

		FORCEINLINE T* const* GetAddressOf() const
		{
			return &obj;
		}

		FORCEINLINE T** GetAddressOf()
		{
			return &obj;
		}
        
        FORCEINLINE operator bool() const { return obj != nullptr; }
        FORCEINLINE T* operator->() const { return obj; }
		FORCEINLINE T& operator*() const { return *obj; }
		FORCEINLINE operator T*() const { return obj; }
    };
    
    class SmartPtrConcurrentCounterPolicy
    {
    private:
        AtomicVar<uint32> counter;
        
    public:
        SmartPtrConcurrentCounterPolicy()
        : counter(1)
        {
            
        }
        
        FORCEINLINE void Increment()
        {
            ++counter;
        }
        
        FORCEINLINE void Decrement()
        {
            --counter;
        }
        
        FORCEINLINE uint32 Get() const
        {
            return counter.GetValue();
        }
    };
    
    class SmartPtrCounterPolicy
    {
    private:
        uint32 counter;
        
    public:
        SmartPtrCounterPolicy()
        : counter(1)
        {
            
        }
        
        FORCEINLINE void Increment()
        {
            ++counter;
        }
        
        FORCEINLINE void Decrement()
        {
            RYUJIN_ASSERT(counter != 0, "Decrementing counter with 0 value");
            --counter;
        }
        
        FORCEINLINE uint32 Get() const
        {
            return counter;
        }
    };
    
    template<typename T, class CounterPolicy, class Deleter>
    class WeakPointer;
    
    class DefaultSmartPtrDeleter
    {
    public:
        template<typename T>
        static void Delete(T*& obj)
        {
            SAFE_DELETE(obj);
        }
    };
    
    template<typename T, class CounterPolicy, class Deleter = DefaultSmartPtrDeleter>
    class SharedPointer
    {
        friend class WeakPointer<T, CounterPolicy, Deleter>;
    private:
        T* obj;
        CounterPolicy* refCount;
        
    public:
        explicit SharedPointer(T* inObj = nullptr)
        : obj(inObj)
        {
            if (obj)
            {
                refCount = RYUJIN_NEW CounterPolicy();
            }
            else
            {
                refCount = nullptr;
            }
        }
        
        ~SharedPointer()
        {
            if (refCount)
            {
                refCount->Decrement();
                if (refCount->Get() == 0)
                {
                    Deleter::Delete(obj);
                    obj = nullptr;
                    SAFE_DELETE(refCount);
                }
            }
        }
        
        explicit SharedPointer(UniquePtr<T>&& ptr)
        {
            if (ptr)
            {
                refCount = RYUJIN_NEW CounterPolicy();
            }
            obj = Move(ptr.Release());
        }
        
        SharedPointer(const SharedPointer<T, CounterPolicy, Deleter>& ptr)
        {
            obj = ptr.obj;
            if (ptr.refCount)
            {
                ptr.refCount->Increment();
            }
            refCount = ptr.refCount;
        }
        
        SharedPointer<T, CounterPolicy, Deleter>& operator=(const SharedPointer<T, CounterPolicy, Deleter>& ptr)
        {
            if (obj == ptr.obj)
                return *this;
            
            if (refCount)
            {
                refCount->Decrement();
                if (refCount->Get() == 0)
                {
                    Deleter::Delete(obj);
                }
            }
            
            obj = ptr.obj;
            if (ptr.refCount)
            {
                ptr.refCount->Increment();
            }
            refCount = ptr.refCount;
            
            return *this;
        }
        
        SharedPointer<T, CounterPolicy, Deleter>& operator=(T* other)
        {
            if (obj == other)
                return *this;
            
            if (refCount)
            {
                refCount->Decrement();
                if (refCount->Get() == 0)
                {
                    Deleter::Delete(obj);
                }
            }
            
            if (obj)
            {
                refCount = RYUJIN_NEW CounterPolicy();
            }
            else
            {
                refCount = nullptr;
            }
            
            obj = other;
            if (obj && !refCount)
            {
                refCount = RYUJIN_NEW CounterPolicy();
            }
            if (refCount)
            {
                refCount->Increment();
            }
            
            return *this;
        }
        
        FORCEINLINE void SafeRelease()
        {
            if (obj)
            {
                Deleter::Delete(obj);
                obj = nullptr;
            }
            if (refCount)
            {
                delete refCount;
                refCount = nullptr;
            }
		}

		FORCEINLINE T* const* GetAddressOf() const
		{
			return &obj;
		}

		FORCEINLINE T** GetAddressOf()
		{
			return &obj;
		}
        
        template<typename DerivedType>
        FORCEINLINE DerivedType* Cast()
        {
            return (DerivedType*)obj;
        }
        
        FORCEINLINE operator bool() const { return obj != nullptr; }
        FORCEINLINE T* operator->() const { return obj; }
        FORCEINLINE T& operator*() const { return *obj; }
        FORCEINLINE const T& operator*() { return *obj; }
		FORCEINLINE operator T*() const { return obj; }
    };
    
    template<typename T, class Deleter = DefaultSmartPtrDeleter>
    using SharedPtr = SharedPointer<T, SmartPtrCounterPolicy, Deleter>;
    
    template<typename T, class Deleter = DefaultSmartPtrDeleter>
    using SharedPtrConcurrent = SharedPointer<T, SmartPtrConcurrentCounterPolicy, Deleter>;
    
    
    template<typename T, class CounterPolicy, class Deleter = DefaultSmartPtrDeleter>
    class WeakPointer
    {
    private:
        const SharedPointer<T, CounterPolicy, Deleter>* obj;
        
    public:
        WeakPointer()
        : obj(nullptr)
        {
            
        }
        
        WeakPointer(const WeakPointer<T, CounterPolicy, Deleter>& weakPtr)
        {
            obj = weakPtr.obj;
        }
        
        WeakPointer(const SharedPointer<T, CounterPolicy, Deleter>& sharedPtr)
        {
            obj = &sharedPtr;
        }
        
        WeakPointer& operator=(const SharedPointer<T, CounterPolicy, Deleter>& other)
        {
            obj = &other;
            return *this;
        }
        
        WeakPointer& operator=(const WeakPointer<T, CounterPolicy, Deleter>& other)
        {
            obj = other.obj;
            return *this;
        }
        
        FORCEINLINE bool Expired() const
        {
            if (obj == nullptr || obj->obj == nullptr)
            {
                return true;
            }
            return obj->refCount->Get() == 0;
        }
        
        FORCEINLINE SharedPointer<T, CounterPolicy, Deleter> Lock() const
        {
            if (Expired())
            {
                return SharedPointer<T, CounterPolicy, Deleter>(nullptr);
            }
            
            return *obj;
        }
    };
    template<typename T, class Deleter = DefaultSmartPtrDeleter>
    using WeakPtr = WeakPointer<T, SmartPtrCounterPolicy, Deleter>;
    
    template<typename T, class Deleter = DefaultSmartPtrDeleter>
    using WeakPtrConcurrent = WeakPointer<T, SmartPtrConcurrentCounterPolicy, Deleter>;
}


