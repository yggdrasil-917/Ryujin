#include "PlayerController.hpp"

#include "RyujinCore/CoreUtils/ConfigCache.hpp"


namespace Ryujin
{
    bool PlayerController::Initialize(int32 playerIndex)
    {
        ConfigCache input;
        if (input.LoadConfig("defaultInput.ini") == false)
            return false;
        
        int32 count = int32(ControllerAction::CA_Count);
        String controllerString = String::Printf("Controller%d", playerIndex);
        for (int32 i = 0; i < count; ++i)
        {
            int32 controllerKey = 0;
            if (input.GetInt(*controllerString, controllerActionNames[i], controllerKey))
            {
                for (int32 j = 0; j < RYUJIN_NUM_SCANCODES; ++j)
                {
                    RyujinScanCode code = (RyujinScanCode)j;
                    if (Keyboard::Instance()->GetKeyMap(code) == controllerKey)
                    {
                        keyToAction.Add((uint32)j, ControllerAction(uint8(ControllerAction::CA_Forward) + i));
                        break;
                    }
                }
            }
        }
        
        return true;
    }
    
    void PlayerController::Update(float dt)
    {
        const uint8* keyboardState = Keyboard::Instance()->GetKeyboardState();
        RYUJIN_ASSERT(keyboardState, "Null keyboard state");
        
        HashTable<uint32, ControllerAction>::Iterator it(keyToAction);
        int32 yaw = 0;
        int32 pitch = 0;
        Mouse::Instance()->GetCursorVector(yaw, pitch);
        if (yaw != 0 || pitch != 0)
        {
            OnRotationActionPerformed(dt, (float)yaw, (float)pitch, 0.0f);
        }
        
        while (it)
        {
            if (keyboardState[it->key] == KEY_PRESSED)
            {
                OnActionPerformed(it->value, dt);
            }
            
            ++it;
        }
    }
}
