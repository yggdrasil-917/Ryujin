#ifndef STRING_HPP
#define STRING_HPP

#include "DynamicArray.hpp"
#include "FixedArray.hpp"

#include <string>

#define SMALL_STRING_LENGTH 16

namespace Ryujin
{
    class CORE_API String
    {
    private:
        
        union Buffer
        {
            DynamicArray<char> heapString;
            FixedArray<char, SMALL_STRING_LENGTH> stackString;
            
            Buffer() : heapString() {}
            ~Buffer() {}
        };
        Buffer buffer;
        int32 stringLength; // includes null character
        
        
        FORCEINLINE char* GetData()
        {
            return stringLength <= SMALL_STRING_LENGTH ? buffer.stackString.Begin() : buffer.heapString.Begin();
        }
        
        FORCEINLINE const char* GetConstData() const
        {
            return stringLength <= SMALL_STRING_LENGTH ? buffer.stackString.Begin() : buffer.heapString.Begin();
        }
        
        void InternalCopyStackStringToHeapString();
        
        
    public:
        String();
        String(const char* src);
        String(const String& other);
        String(String&& other);
        
        String& operator=(const String& other);
        String& operator=(String&& other);
        String& operator=(const char* other);
        
        ~String();
        
        void ParseIntoArray(DynamicArray<String>& outArray, const String& delim);
        
        bool operator==(const String& right) const;
        bool operator==(const char* right) const;
        bool operator!=(const String& right) const;
        bool operator!=(const char* right) const;
        bool operator <(const String& right) const;
        bool operator >(const String& right) const;
        String& operator+=(const String& right);
        String& operator+=(const char* right);
        String& operator+=(const char right);
        
        FORCEINLINE bool IsHeapAllocated() const
        {
            return stringLength > SMALL_STRING_LENGTH;
        }
        
        FORCEINLINE char& operator[](uint32 index)
        {
            return GetData()[index];
        }
        
        FORCEINLINE const char& operator[](uint32 index) const
        {
            return GetConstData()[index];
        }
        
        FORCEINLINE operator const char*() const
        {
            return GetConstData();
        }
        
        FORCEINLINE const char* operator*() const
        {
            return GetConstData();
        }
        
        FORCEINLINE int32 Length() const
        {
            return stringLength == 0 ? 0 : stringLength - 1; // ignores null character at the end
        }
        
        String ToLower() const
        {
            String lower(*this);
            for (uint32 i = 0; i < lower.Length(); ++i)
            {
                char c = lower[i];
                if (c >= 'A' && c <= 'Z')
                {
                    lower[i] |= 0x20;
                }
//                lower[i] = std::tolower(lower[i]);
            }
            return lower;
        }
        
        String ToUpper() const
        {
            String upper(*this);
            for (uint32 i = 0; i < upper.Length(); ++i)
            {
                char c = upper[i];
                if (c >= 'a' && c <= 'z')
                {
                    c &= 0xdf;
                    upper[i] = c;
                }
            }
            return upper;
        }
        
        friend String operator+(String lhs, const char* rhs)
        {
            lhs += rhs;
            
            return lhs;
        }
        
        String operator+(const String& right)
        {
            String str(*this);
            str += right;
            
            return str;
        }
        
        int32 Find(const String& substr) const;
        void Replace(int32 startIndex, int32 numCharsToReplace, const String& str);
        
        bool StartsWith(const String& subStr) const;
        bool EndsWith(const String& subStr) const;
        
        static String Concat(const char* lhs, const char* rhs)
        {
            String left(lhs);
            left += rhs;
            
            return left;
        }
        
        static String Printf(const char* format, ...)
        {
            va_list args;
            char buffer[RYUJIN_MAX_PATH];
            
            va_start(args, format);
            vsnprintf(buffer, RYUJIN_MAX_PATH, format, args);
            va_end(args);
            
            String out(buffer);
            
            return out;
        }
        
        static void Sprintf(char* buffer, const char* format, ...)
        {
            va_list args;
            
            va_start(args, format);
            vsnprintf(buffer, RYUJIN_MAX_PATH, format, args);
            va_end(args);
        }
        
        static String CurrentDate();
        
        static const char* StrStr(const char* str1, const char* str2)
        {
            return strstr(str1, str2);
        }
        
        static String SubStr(const char* src, const uint32 length);
        
        CORE_API friend void Pup(struct Pupper* p, String& obj, const struct VarInfo& inVarInfo);
    };
    
    // c++11 Unicode string types
    using utf8_t    = char;
    using utf16_t   = char16_t;
    using utf32_t   = char32_t;
    
#define UTF8(str) u8str
#define UTF16(str) ustr
#define UTF32(str) Ustr
};


#endif
