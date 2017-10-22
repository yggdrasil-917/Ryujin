#pragma once


#include "RyujinCore/CoreUtils/StringHash.hpp"

#include "Macros.hpp"

namespace Ryujin
{
    class ITestMethod
    {
    protected:
        bool bFailed = false;
        class TestCase* owner;
        
    public:
        ITestMethod() : owner(nullptr) {}
        ITestMethod(class TestCase* inOwner) : owner(inOwner) {}
        VIRTUAL ~ITestMethod() { }
        VIRTUAL void Invoke() = 0;
        
        FORCEINLINE bool HasFailed() const { return bFailed; }
        FORCEINLINE void SetFailedFlag(bool value) { bFailed = value; }
    };
    
    class TestCase
    {
    public:
        StringHash hash;
        String desc;
        ITestMethod* test;
        
        ~TestCase() { SAFE_DELETE(test); }
        
        void Invoke();
    };
    
}
