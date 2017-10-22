#ifndef BITFIELD_HPP
#define BITFIELD_HPP


namespace Ryujin
{
	template<typename T>
	class Bitfield
	{
	public:
		using Value = T;

	protected:
		Value bitfield;

	public:
		Bitfield()
			: bitfield(static_cast<Value>(0))
		{}

		Bitfield(Value bitmask)
			: bitfield(bitmask)
		{}

		template<typename Y>
		FORCEINLINE void EnableBits(Y mask)
		{
			Value maski = static_cast<Value>(mask);
			bitfield |= maski;
		}

		template<typename Y>
		FORCEINLINE void DisableBits(Y mask)
		{
			Value maski = static_cast<Value>(mask);
			bitfield &= ~maski;
		}

		template<typename Y>
		FORCEINLINE void SwitchBits(Y mask)
		{
			Value maski = static_cast<Value>(mask);
			bitfield ^= maski;
		}

		template<typename Y>
		FORCEINLINE bool BitsEnabled(Y mask) const
		{
			Value maski = static_cast<Value>(mask);
			return (bitfield & maski) == maski;
		}

		template<typename Y>
		FORCEINLINE bool AnyBitsEnabled(Y mask) const
		{
			Value maski = static_cast<Value>(mask);
			return (bitfield & maski) != static_cast<Value>(0);
		}

		FORCEINLINE Value GetBitmask() const
		{
			return bitfield;
		}
	};
}


#endif