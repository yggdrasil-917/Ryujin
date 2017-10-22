#ifndef INPUT_HPP
#define INPUT_HPP


#include "Keyboard.hpp"
#include "Mouse.hpp"
#include "Events.hpp"

#if PLATFORM_MAC
#include "HIDManagerDarwin.hpp"
#elif PLATFORM_WINDOWS
#include "HIDManagerWindows.hpp"
#else
#include "HIDManagerLinux.hpp"
#endif

#endif
