
#include "Json.hpp"



namespace Ryujin
{
    
    JsonValue::JsonValue()
    {
        type = JsonType::JT_Null;
    }
    
    JsonValue::JsonValue(const char* str)
    : type(JsonType::JT_String)
    {
        int32 length = (int32)std::strlen(str);
//        data.stringValue = RYUJIN_NEW char[length + 1];
        std::strcpy(data.stringValue, str);
    }
    
    JsonValue::JsonValue(bool inBool)
    {
        type = JsonType::JT_Bool;
        data.boolValue = inBool;
    }
    
    JsonValue::JsonValue(PtrSize inNumber)
    {
        type = JsonType::JT_Number;
        data.numberValue = (float)inNumber;
    }
    
#if PLATFORM_MAC
    JsonValue::JsonValue(uint64 inNumber)
    {
        type = JsonType::JT_Number;
        data.numberValue = inNumber;
    }
#endif
    
    JsonValue::JsonValue(float inNumber)
    {
        type = JsonType::JT_Number;
        data.numberValue = inNumber;
    }
    
    JsonValue::JsonValue(const JsonArray& inArray)
    {
        type = JsonType::JT_Array;
        data.arrayValue = RYUJIN_NEW JsonArray;
        *data.arrayValue = inArray;
    }
    
    JsonValue::JsonValue(const JsonObject& inObject)
    {
        type = JsonType::JT_Object;
        data.objectValue = RYUJIN_NEW JsonObject;
        *data.objectValue = inObject;
    }
    
    JsonValue::~JsonValue()
    {
        if (type == JsonType::JT_Array)
        {
            data.arrayValue->RemoveAll();
        }
        else if (type == JsonType::JT_Object)
        {
            JsonObject::iterator iter;
            for (iter = data.objectValue->begin(); iter != data.objectValue->end(); iter++)
            {
                delete (*iter).second;
            }
        }
        else if (type == JsonType::JT_String)
        {
//            delete data.stringValue;
        }
    }
    
    JsonValue* JsonValue::Parse(char*& data)
    {
        if (!data)
            return nullptr;
        
        if (*data == '"')
        {
            // Parse string
            String str;
            if (!Json::ExtractString(++data, str))
            {
                return nullptr;
            }
            else
            {
                return RYUJIN_NEW JsonValue(*str);
            }
        }
        else if (*data == 't' || *data == 'T' || *data == 'f' || *data == 'F')
        {
            // bool
            return RYUJIN_NEW JsonValue((*data == 't' || *data == 'T'));
        }
        else if (*data == 'n')
        {
            // null
            data += 4; // 4 for null
            return RYUJIN_NEW JsonValue();
        }
        else if (*data == '-' || (*data >= '0' && *data <= '9'))
        {
            // number
            return RYUJIN_NEW JsonValue(AtoF(data));
        }
        else if (*data == '{')
        {
            // object
            JsonObject object;
            ++data;
            
            while (*data)
            {
                if (!Json::SkipWhitespace(data))
                {
                    FREE_OBJECT(object);
                    return nullptr;
                }
                
                if (object.size() == 0 && *data == '}')
                {
                    ++data;
                    return RYUJIN_NEW JsonValue(object);
                }
                
                String name;
                if (!Json::ExtractString(++(data), name))
                {
                    FREE_OBJECT(object);
                    return nullptr;
                }
                
                if (!Json::SkipWhitespace(data))
                {
                    FREE_OBJECT(object);
                    return nullptr;
                }
                
                if (*data != ':')
                {
                    FREE_OBJECT(object);
                    return nullptr;
                }
                ++data;
                
                if (!Json::SkipWhitespace(data))
                {
                    FREE_OBJECT(object);
                    return nullptr;
                }
                
                JsonValue *value = Parse(data);
                if (!value)
                {
                    FREE_OBJECT(object);
                    return nullptr;
                }
                
                if (object.find(name) != object.end())
                    delete object[name];
//                object.insert(std::pair<String, JsonValue*>(name, value));
                object[name] = value;
                
                if (!Json::SkipWhitespace(data))
                {
                    FREE_OBJECT(object);
                    return nullptr;
                }
                
                if (*data == '}')
                {
                    ++data;
                    return RYUJIN_NEW JsonValue(object);
                }
                
                if (*data != ',')
                {
                    FREE_OBJECT(object);
                    return nullptr;
                }
                
                ++data;
            }
            
            FREE_OBJECT(object);
            return nullptr;
        }
        else if (*data == '[')
        {
            // array
            JsonArray array;
            ++data;
            
            while (*data)
            {
                if (!Json::SkipWhitespace(data))
                {
                    array.RemoveAll();
                    return nullptr;
                }
                
                if (array.GetSize() == 0 && *data == ']')
                {
                    ++data;
                    return RYUJIN_NEW JsonValue(array);
                }
                
                JsonValue* value = Parse(data);
                if (!value)
                {
                    array.RemoveAll();
                    return nullptr;
                }
                
                array.Add(value);
                
                if (!Json::SkipWhitespace(data))
                {
                    array.RemoveAll();
                    return nullptr;
                }
                
                if (*data == ']')
                {
                    ++data;
                    return RYUJIN_NEW JsonValue(array);
                }
                
                if (*data != ',')
                {
                    array.RemoveAll();
                    return nullptr;
                }
                
                ++data;
            }
            
            array.RemoveAll();
            return nullptr;
        }
        
        return nullptr;
    }
    
    static String Indent(PtrSize depth)
    {
        const PtrSize indentStep = 2;
        depth > 0 ? --depth : 0;
        String indent;
        int32 numWhitespaces = (int32)(depth * indentStep);
        while (numWhitespaces)
        {
            indent += ' ';
            --numWhitespaces;
        }
        
        return indent;
    }
    
    String JsonValue::Stringify(int32 indentDepth) const
    {
        String strValue;
        PtrSize indentDepth1 = indentDepth > 0 ?  indentDepth + 1 : indentDepth;
        String indentString = Indent(indentDepth);
        String indentString1 = Indent(indentDepth1);
        
        switch (type)
        {
            case JsonType::JT_Null:
                strValue = "null";
                break;
                
            case JsonType::JT_String:
                strValue = StringifyString(data.stringValue);
                break;
                
            case JsonType::JT_Bool:
                strValue = data.boolValue ? "true" : "false";
                break;
                
            case JsonType::JT_Number:
            {
                if (isnan(data.numberValue) || isinf(data.numberValue))
                {
                    strValue = "nan";
                }
                else
                {
                    std::stringstream ss;
                    ss.precision(15);
                    ss << data.numberValue;
                    strValue = ss.str().c_str();
                }
                break;
            }
                
            case JsonType::JT_Array:
            {
                strValue = indentDepth ? "[\n" + indentString1 : "[";
                DynamicArray<JsonValue*>::ConstIterator it = data.arrayValue->Begin();
                while (it != data.arrayValue->End())
                {
                    strValue += (*it)->Stringify((int32)indentDepth1);
                    
                    if (++it != data.arrayValue->End())
                    {
                        strValue += ",";
                    }
                }
                strValue += indentDepth ? "\n" + indentString1 + "]" : "]";
                break;
            }
                
            case JsonType::JT_Object:
            {
                strValue = indentDepth ? "{\n" + indentString1 : "{";
                JsonObject::const_iterator iter = data.objectValue->begin();
                while (iter != data.objectValue->end())
                {
                    strValue += StringifyString(*(*iter).first);
                    strValue += ":";
                    strValue += (*iter).second->Stringify((int32)indentDepth1);
                    
                    // Not at the end - add a separator
                    if (++iter != data.objectValue->end())
                        strValue += ",";
                }
//                strValue += indentDepth ? "\n" + indentString + "}" : "}";
                if (indentDepth)
                {
                    strValue += "\n";
                    strValue += indentString;
                    strValue += "}";
                }
                else
                {
                    strValue += "}";
                }
                break;
            }
        }
        
        return strValue;
    }
    
    String JsonValue::StringifyString(const char* str) const
    {
        String out = "\"";
        
        while (*str)
        {
            char chr = *str;
            
            if (chr == '"' || chr == '\\' || chr == '/')
            {
                out += '\\';
                out += chr;
            }
            else if (chr == '\b')
            {
                out += "\\b";
            }
            else if (chr == '\f')
            {
                out += "\\f";
            }
            else if (chr == '\n')
            {
                out += "\\n";
            }
            else if (chr == '\r')
            {
                out += "\\r";
            }
            else if (chr == '\t')
            {
                out += "\\t";
            }
            else if (chr < ' ' || chr > 126)
            {
                out += "\\u";
                for (int i = 0; i < 4; i++)
                {
                    int value = (chr >> 12) & 0xf;
                    if (value >= 0 && value <= 9)
                        out += (char)('0' + value);
                    else if (value >= 10 && value <= 15)
                        out += (char)('A' + (value - 10));
                    chr <<= 4;
                }
            }
            else
            {
                out += chr;
            }
            
            ++str;
        }
        
        out += "\"";
        
        return out;
    }
    
    
    
    
    JsonValue* Json::ParseFile(const char* filepath)
    {
        if (!filepath)
            return nullptr;
        
        FILE* fp = fopen(filepath, "rb");
        if (!fp)
            return nullptr;
        
        fseek(fp, 0, SEEK_END);
        PtrSize filesize = ftell(fp);
        rewind(fp);
        char* fileContent = RYUJIN_NEW char[filesize + 1];
        fread(fileContent, 1, filesize, fp);
        fclose(fp);
        fileContent[filesize] = '\0';
        
        char* data = fileContent;
        JsonValue* rootValue = Parse(data);
        
        delete[] fileContent;
        
        return rootValue;
    }
    
    JsonValue* Json::Parse(char*& data)
    {
        if (!SkipWhitespace(data))
        {
            return nullptr;
        }
        
        JsonValue *value = JsonValue::Parse(data);
        if (!value)
            return nullptr;
        
        if (SkipWhitespace(data))
        {
            delete value;
            return nullptr;
        }
        
        return value;
    }
    
    bool Json::SkipWhitespace(char*& data)
    {
        while (*data && (*data == ' ' || *data == '\t' || *data == '\r' || *data == '\n'))
            ++data;
        
        return *data != 0;
    }
    
    bool Json::ExtractString(char*& data, String& outStr)
    {
        while(*data)
        {
            char nextChar = *data;
            
            if (nextChar == '\\')
            {
                // handle escape characters
                ++data;
                
                switch (*data)
                {
                    case '"':
                        nextChar = '"';
                        break;
                        
                    case '\\':
                        nextChar = '\\';
                        break;
                        
                    case '/':
                        nextChar = '/';
                        break;
                        
                    case 'b':
                        nextChar = '\b';
                        break;
                        
                    case 'f':
                        nextChar = '\f';
                        break;
                        
                    case 'n':
                        nextChar = '\n';
                        break;
                        
                    case 'r':
                        nextChar = '\r';
                        break;
                        
                    case 't':
                        nextChar = '\t';
                        break;
                        
                    case 'u':
                    {
                        // TODO: parse hex
                    }
                        break;
                        
                    default:
                        return false;
                }
            }
            else if (nextChar == '"')
            {
                // end of string
                ++data;
                return true;
            }
            else if (nextChar < ' ' && nextChar != '\t')
            {
                // disallowed chars
                return false;
            }
            
            outStr += nextChar;
            ++data;
        }
        
        return false;
    }
}
