#include "String.hpp"


#include "../Serialization/BinaryFilePupper.hpp"


namespace Ryujin
{
    String::String()
    : stringLength(1)
    {
        
    }
    
    String::String(const char* src)
    : stringLength(0)
    {
        if (src)
        {
            int32 length = (int32)strlen(src);
            stringLength = length + 1;
            
            if (length >= SMALL_STRING_LENGTH)
            {
                buffer.heapString.Resize(length + 1);
                
                while (*src)
                {
                    buffer.heapString.Add(*src);
                    ++src;
                }
                buffer.heapString.Add('\0');
            }
            else
            {
                int32 start = 0;
                int32 end = start + length;
                int32 i = 0;
                for (; start < end; ++start)
                {
                    buffer.stackString[start] = src[i++];
                }
                buffer.stackString[end] = '\0';
            }
        }
    }
    
    String::String(const String& other)
    {
        stringLength = other.stringLength;
        if (stringLength > SMALL_STRING_LENGTH)
        {
            buffer.heapString = other.buffer.heapString;
        }
        else
        {
            buffer.stackString = other.buffer.stackString;
        }
    }
    
    String::String(String&& other)
    {
        stringLength = other.stringLength;
        if (stringLength > SMALL_STRING_LENGTH)
        {
            buffer.heapString = Move(other.buffer.heapString);
        }
        else
        {
            buffer.stackString = Move(other.buffer.stackString);
        }
        
        other.stringLength = 0;
    }
    
    String& String::operator=(const String& other)
    {
        stringLength = other.stringLength;
        if (stringLength > SMALL_STRING_LENGTH)
        {
            buffer.heapString = other.buffer.heapString;
        }
        else
        {
            buffer.stackString = other.buffer.stackString;
        }
        
        return *this;
    }
    
    String& String::operator=(String&& other)
    {
        stringLength = other.stringLength;
        if (stringLength > SMALL_STRING_LENGTH)
        {
            buffer.heapString = Move(other.buffer.heapString);
        }
        else
        {
            buffer.stackString = Move(other.buffer.stackString);
        }
        
        return *this;
    }
    
    String& String::operator=(const char* other)
    {
        int32 newStringLength = (int32)strlen(other) + 1;
        
        if (newStringLength > SMALL_STRING_LENGTH)
        {
            buffer.heapString.Resize(newStringLength + 1);
            while(*other)
            {
                buffer.heapString.Add(*other);
                ++other;
            }
            buffer.heapString.Add('\0');
        }
        else
        {
            if (IsHeapAllocated() && newStringLength != stringLength)
            {
                buffer.heapString.RemoveAll();
                stringLength = newStringLength;
            }
            
            char* data = GetData();
            int32 i = 0;
            while (*other)
            {
                data[i++] = *other;
                ++other;
            }
            data[i] = '\0';
            if (newStringLength == 0)
                newStringLength = 1;
        }
        stringLength = newStringLength;
        
        return *this;
    }
    
    String::~String()
    {
        
    }
    
    bool String::operator==(const String& right) const
    {
        if (Length() != right.Length())
        {
            return false;
        }
        
        const uint32 count = Length();
        const char* data = GetConstData();
        const char* other = right.GetConstData();
        for (uint32 i = 0; i < count; ++i)
        {
            if (data[i] != other[i])
            {
                return false;
            }
        }
        
        return true;
    }
    
    bool String::operator==(const char* right) const
    {
        if (!right)
        {
            return false;
        }
        
        int32 count = 0;
        const char* data = GetConstData();
        while (*right)
        {
            if (count >= stringLength)
            {
                return false;
            }
            
            if (data[count++] != *right)
            {
                return false;
            }
            ++right;
        }
        
        return true;
    }
    
    bool String::operator!=(const String& right) const
    {
        if (Length() != right.Length())
        {
            return true;
        }
        
        const uint32 count = Length();
        const char* data = GetConstData();
        const char* other = right.GetConstData();
        for (uint32 i = 0; i < count; ++i)
        {
            if (data[i] != other[i])
            {
                return true;
            }
        }
        
        return false;
    }
    
    bool String::operator!=(const char* right) const
    {
        if (!right)
        {
            return false;
        }
        
        int32 count = 0;
        const char* data = GetConstData();
        while (*right)
        {
            if (count >= stringLength)
            {
                return true;
            }
            
            if (data[count++] != *right)
            {
                return true;
            }
            ++right;
        }
        
        return false;
    }
    
    bool String::operator <(const String& right) const
    {
        return std::strcmp(GetConstData(), *right) < 0;
    }
    
    bool String::operator >(const String& right) const
    {
        return std::strcmp(GetConstData(), *right) > 0;
    }
    
    void String::InternalCopyStackStringToHeapString()
    {
        // copy stack string to heap string
        char copy[16];
        strcpy(copy, buffer.stackString.Begin());
        buffer.heapString = DynamicArray<char>();
        const char* stack = copy;
        while (*stack)
        {
            buffer.heapString.Add(*stack);
            ++stack;
        }
        buffer.heapString.Add('\0');
    }
    
    String& String::operator+=(const String& right)
    {
        if (IsHeapAllocated() || (stringLength + right.Length()) > SMALL_STRING_LENGTH)
        {
            if (stringLength == 1)
            {
                this->operator=(right);
                return *this;
            }
            
            if (!IsHeapAllocated() && (stringLength + right.Length()) > SMALL_STRING_LENGTH)
            {
                InternalCopyStackStringToHeapString();
            }
            
            const char* data = right.GetConstData();
            if (buffer.heapString.IsFull())
            {
                buffer.heapString.Resize(buffer.heapString.GetSize() * 2 + 8);
            }
            
            if (buffer.heapString.IsEmpty())
            {
                buffer.heapString.Add(right[0]);
                ++data;
            }
            else
            {
                int32 index = MathUtils::Max<int32>(Length(), 0);
                buffer.heapString[index] = right[0];
                ++data;
            }
            
            while (*data)
            {
                buffer.heapString.Add(*data);
                ++data;
            }
            buffer.heapString.Add('\0');
            stringLength = buffer.heapString.GetSize();
        }
        else
        {
            int32 start = stringLength == 0 ? 0 : stringLength - 1;
            int32 end = start + right.Length();
            int32 i = 0;
            for (; start < end; ++start)
            {
                buffer.stackString[start] = right[i++];
            }
            buffer.stackString[end] = '\0';
            stringLength += right.Length();
        }
        
        return *this;
    }
    
    String& String::operator+=(const char* right)
    {
        if (!right)
        {
            return *this;
        }
        
        int32 length = (int32)strlen(right);
        
        if (IsHeapAllocated() || (stringLength + length) > SMALL_STRING_LENGTH)
        {
            if (stringLength == 1)
            {
                this->operator=(right);
                return *this;
            }
            
            if (!IsHeapAllocated() && (stringLength + length) > SMALL_STRING_LENGTH)
            {
                InternalCopyStackStringToHeapString();
            }
            
            if (buffer.heapString.IsFull())
            {
                buffer.heapString.Resize(buffer.heapString.GetSize() * 2 + 8);
            }
            
            if (buffer.heapString.IsEmpty())
            {
                buffer.heapString.Add(*right);
            }
            else
            {
                int32 index = MathUtils::Max<int32>(Length(), 0);
                buffer.heapString[index] = *right;
            }
            ++right;
            
            while (*right)
            {
                buffer.heapString.Add(*right);
                ++right;
            }
            buffer.heapString.Add('\0');
            stringLength = buffer.heapString.GetSize();
        }
        else
        {
            int32 start = stringLength == 0 ? 0 : stringLength - 1;
            int32 end = start + length;
            int32 i = 0;
            for (; start < end; ++start)
            {
                buffer.stackString[start] = right[i++];
            }
            buffer.stackString[end] = '\0';
            stringLength += length;
        }
        
        return *this;
    }
    
    String& String::operator+=(const char right)
    {
        if (IsHeapAllocated() || (stringLength + 1) > SMALL_STRING_LENGTH)
        {
            if (!IsHeapAllocated() && (stringLength + 1) > SMALL_STRING_LENGTH)
            {
                InternalCopyStackStringToHeapString();
            }
            
            if (buffer.heapString.IsFull())
            {
                buffer.heapString.Resize(buffer.heapString.GetSize() * 2 + 8);
            }
            
            if (buffer.heapString.IsEmpty())
            {
                buffer.heapString.Add(right);
            }
            else
            {
                int32 index = MathUtils::Max<int32>(Length(), 0);
                buffer.heapString[index] = right;
            }
            buffer.heapString.Add('\0');
            stringLength = buffer.heapString.GetSize();
        }
        else
        {
            buffer.stackString[stringLength == 0 ? 0 : stringLength - 1] = right;
            buffer.stackString[stringLength++] = '\0';
        }
        
        return *this;
    }
    
    int32 String::Find(const String &substr) const
    {
        const int32 length = Length();
        const int32 substrLength = substr.Length();
        const int32 elementsToScan = length - substrLength;
        if (elementsToScan < 0)
            return -1;
        
        const char* data = GetConstData();
        
        for (int32 i = 0; i <= elementsToScan; ++i)
        {
            bool bContinueProcessing = true;
            for (int32 j = 0; j < substrLength && bContinueProcessing; ++j)
            {
                // Returns index of first occurence of substr
                if (data[i + j] == substr[j])
                {
                    if ((j + 1) == substrLength)
                        return i;
                }
                else
                {
                    bContinueProcessing = false;
                }
            }
        }
        
        return -1;
    }
    
    void String::Replace(int32 startIndex, int32 numCharsToReplace, const String &str)
    {
        int32 index = 0;
        int32 endIndex = startIndex + numCharsToReplace;
        endIndex = endIndex >= Length() ? Length() : endIndex;
        int32 i = 0;
        char* data = GetData();
        for(i = startIndex; i < endIndex && index < str.Length(); ++i)
        {
            data[i] = str[index++];
        }
        
        if (index == numCharsToReplace)
        {
            return;
        }
        
        if (i == Length())
        {
            // Append however many characters are left to replace
            while (index != numCharsToReplace && index < str.Length())
            {
                this->operator+=(str[index]);
                ++index;
            }
        }
        else
        {
            // Left shift the characters
            int32 charsLeftToReplace = numCharsToReplace - index; // index points to the first char that needs to be replaced
            startIndex = i;
            endIndex = i + charsLeftToReplace;
            endIndex = endIndex >= Length() ? Length() : endIndex;
            int32 j = 0;
            for (j = startIndex; j < endIndex && j + charsLeftToReplace < endIndex; ++j)
            {
                data[j] = data[j + charsLeftToReplace];
            }
            
            if (j < Length())
            {
                data[j] = '\0';
            }
        }
    }
    
    bool String::StartsWith(const String &subStr) const
    {
        if (subStr.Length() > Length())
            return false;
        
        const char* data = GetConstData();
        for (int32 i = 0; i < subStr.Length(); ++i)
        {
            if (subStr[i] != data[i])
                return false;
        }
        
        return true;
    }
    
    bool String::EndsWith(const String &subStr) const
    {
        if (subStr.Length() > Length())
            return false;
        
        int32 index = Length() - subStr.Length();
        const char* data = GetConstData();
        for (int32 i = 0; i < subStr.Length(); ++i)
        {
            if (subStr[i] != data[index + i])
                return false;
        }
        
        return true;
    }
    
    void String::ParseIntoArray(DynamicArray<String> &outArray, const String &delim)
    {
        if (stringLength == 0 || delim.Length() == 0)
        {
            return;
        }
        
        outArray.RemoveAll();
        
        const uint32 delimLength = delim.Length();
        const char * start = GetConstData();
        
        if (start && delimLength)
        {
            while (const char* at = StrStr(start, delim))
            {
                if (at - start)
                {
                    PtrSize count = at - start;
                    outArray.Add( SubStr(start, (uint32)count) );
                }
                start += delimLength + (at - start);
            }
            if (*start)
            {
                String str = start;
                outArray.Add(str);
            }
        }
    }
    
    String String::CurrentDate()
    {
        auto now = std::chrono::system_clock::now();
        auto inTime = std::chrono::system_clock::to_time_t(now);
        
        std::stringstream ss;
        ss << std::put_time(std::localtime(&inTime), "%Y-%m-%d_%H-%M-%S");
        return ss.str().c_str();
    }
    
    String String::SubStr(const char *src, const uint32 length)
    {
        String str;
        if (src)
        {
            for (uint32 i = 0; i < length && (src + i); ++i)
            {
                str += *(src + i);
            }
            str += '\0';
        }
        
        return str;
    }
    
    void Pup(Pupper* p, String& obj, const VarInfo& inVarInfo)
    {
        int32 length = obj.Length();
        p->Pup(length, inVarInfo.name + ".length");
        char* data = (length+1) > SMALL_STRING_LENGTH ? obj.buffer.heapString.Begin() : obj.buffer.stackString.Begin();

        if (p->ioMode == Pupper::IoMode::Read && (length+1) > SMALL_STRING_LENGTH)
        {
            data = RYUJIN_NEW char[length+1];
        }
        
        for (int32 i = 0; i < length; ++i)
        {
            p->Pup(data[i], String::Printf("%s.[%d]", *inVarInfo.name, i));
        }
        
        if (p->ioMode == Pupper::IoMode::Read)
        {
            if ((length+1) > SMALL_STRING_LENGTH)
            {
                obj.buffer.heapString.MoveTemp(data, length);
                obj.buffer.heapString.Add('\0');
                obj.stringLength = obj.buffer.heapString.GetSize();
            }
            else
            {
                obj.stringLength = length;
                obj.buffer.stackString[obj.stringLength++] = '\0';
            }
        }
    }
}
