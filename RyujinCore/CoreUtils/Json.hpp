
#ifndef JSON_HPP
#define JSON_HPP


#include <map>
#include "../Containers/DynamicArray.hpp"
#include "../Containers/String.hpp"


namespace Ryujin
{
    class JsonValue;
    using JsonArray = DynamicArray<JsonValue*>;
    using JsonObject = std::map<String, JsonValue*>;
    
    enum class JsonType : uint8
    {
        JT_Null,
        JT_String,
        JT_Bool,
        JT_Number,
        JT_Array,
        JT_Object,
    };
    
    class CORE_API JsonValue
    {
    private:
        union
        {
            char stringValue[64];
            float numberValue;
            bool boolValue;
            JsonArray* arrayValue;
            JsonObject* objectValue;
        } data;
        
        JsonType type;
        
    public:
        JsonValue();
        JsonValue(const char* str);
        JsonValue(bool inBool);
        JsonValue(PtrSize inNumber);

#if PLATFORM_MAC
        JsonValue(uint64 inNumber);
#endif

        JsonValue(float inNumber);
        JsonValue(const JsonArray& inArray);
        JsonValue(const JsonObject& inObject);
        
        ~JsonValue();
        
        FORCEINLINE bool IsNull() const
        {
            return type == JsonType::JT_Null;
        }
        
        FORCEINLINE bool IsString() const
        {
            return type == JsonType::JT_String;
        }
        
        FORCEINLINE bool IsBool() const
        {
            return type == JsonType::JT_Bool;
        }
        
        FORCEINLINE bool IsNumber() const
        {
            return type == JsonType::JT_Number;
        }
        
        FORCEINLINE bool IsArray() const
        {
            return type == JsonType::JT_Array;
        }
        
        FORCEINLINE bool IsObject() const
        {
            return type == JsonType::JT_Object;
        }
        
        FORCEINLINE const char* AsString() const
        {
            return data.stringValue;
        }
        
        FORCEINLINE bool AsBool() const
        {
            return data.boolValue;
        }
        
        FORCEINLINE float AsNumber() const
        {
            return data.numberValue;
        }
        
        FORCEINLINE const JsonArray& AsArray() const
        {
            return *data.arrayValue;
        }
        
        FORCEINLINE const JsonObject& AsObject() const
        {
            return *data.objectValue;
        }
        
        String Stringify(int32 indentDepth = 0) const;
        String StringifyString(const char* str) const;
        
        static JsonValue* Parse(char*& data);
    };
    
    
    class CORE_API Json
    {
    public:
        static JsonValue* ParseFile(const char* filepath);
        static JsonValue* Parse(char*& data);
        static bool ExtractString(char*& data, String& outStr);
        static bool SkipWhitespace(char*& data);
    };
    
#define FREE_OBJECT(x) { JsonObject::iterator iter; for (iter = x.begin(); iter != x.end(); iter++) { delete (*iter).second; } }
}


#endif
