

#include "../RyujinUnitTesting/RyujinUnitTesting.hpp"
#include "../RyujinUnitTesting/TestCase.hpp"

#include "RyujinCore/Containers/Bitfield.hpp"



namespace Ryujin
{
    TEST_CASE(BitfieldEnableBitsTest, "Test for enabling bits")
    {
        Bitfield<uint32> field;
        uint32 bits = 1;
        field.EnableBits(bits);
        REQUIRE(field.BitsEnabled(bits));
    }
    
    TEST_CASE(BitfieldDisableBitsTest, "Test for disabling bits")
    {
        Bitfield<uint32> field;
        uint32 bits = (uint32)-1;
        field.DisableBits(bits);
        REQUIRE(field.BitsEnabled(0) && !field.AnyBitsEnabled(0));
    }
    
    TEST_CASE(BitfieldSwitchBitsTest, "Test for switching bits")
    {
        Bitfield<uint32> field((uint32)-1);
        uint32 bits = (uint32)-1;
        field.SwitchBits(bits);
        REQUIRE(field.BitsEnabled(0));
    }
}
