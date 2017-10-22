#include "TestCase.hpp"

#include "RyujinCore/CoreUtils/Timer.hpp"


namespace Ryujin
{
    void TestCase::Invoke()
    {
        if (test)
        {
            Timer timer(true);
            test->Invoke();
            timer.Stop();
            
            if (!test->HasFailed())
            {
                float elapsedTime = timer.ElapsedSeconds();
                if (elapsedTime > 1.0f)
                {
                    Debugf(Info, "%s took %.4f seconds", hash.ToString(), elapsedTime);
                }
            }
        }
    }
}
