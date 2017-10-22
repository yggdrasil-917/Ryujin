#include "Mouse.hpp"


namespace Ryujin
{
    Mouse* Mouse::Instance()
    {
        static Mouse mouse;
        return &mouse;
    }
}
