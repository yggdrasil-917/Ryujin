#pragma once


namespace Ryujin
{
#define TEST_CASE(testName, description) \
    class testName : public ITestMethod \
    { \
        public: \
            testName() \
            {\
                StringHash hash(#testName);\
                TestCase* testCase = RYUJIN_NEW TestCase();\
                testCase->hash = hash;\
                testCase->desc = description; \
                testCase->test = RYUJIN_NEW testName(testCase);\
                UnitTestRegistry::Register(testCase); \
            } \
            testName(TestCase* inOwner) : ITestMethod(inOwner) {  } \
            void Invoke() OVERRIDE FINAL; \
    } var##testName; \
    void testName::Invoke()
    
    
    
#define REQUIRE(exp) \
    if (!(exp)) \
    { \
        bFailed = true; \
        Debugf(Info, "%s failed %s at line %d", owner->hash.ToString(), #exp, __LINE__); \
        return; \
    }
    
    
}
