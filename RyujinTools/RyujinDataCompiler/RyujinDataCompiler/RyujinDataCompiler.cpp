

#include "RyujinDataCompiler.hpp"

#include "RyujinCore/OS/FileSystem.hpp"
#include "RyujinCore/CoreUtils/CRC.hpp"
#include "RyujinExtensions/Utils/SymbolLocator.hpp"


namespace Ryujin
{
    Variable::Variable()
    :
    type(Variable::Type::Unknown),
    arrayCount(0),
    buffer(nullptr)
    {
        
    }
    
    Variable::Variable(const char* inName, Variable::Type inType, int32 inArrayCount)
    :
    type(inType),
    arrayCount(inArrayCount),
    buffer(nullptr)
    {
        String::Sprintf(name, "%s", inName);
    }
    
    Variable::Variable(const Variable& other)
    :
    type(other.type),
    arrayCount(other.arrayCount)
    {
        String::Sprintf(name, "%s", other.name);
        if (other.buffer)
        {
            buffer = Memory::Malloc(other.GetVariableByteSize());
            Memory::MemCpy(buffer, other.buffer, other.GetVariableByteSize());
        }
        else
        {
            buffer = nullptr;
        }
    }
    
    Variable& Variable::operator=(const Variable &other)
    {
        type = other.type;
        arrayCount = other.arrayCount;
        String::Sprintf(name, "%s", other.name);
        if (other.buffer)
        {
            buffer = Memory::Malloc(other.GetVariableByteSize());
            Memory::MemCpy(buffer, other.buffer, other.GetVariableByteSize());
        }
        else
        {
            buffer = nullptr;
        }
        
        return *this;
    }
    
    Variable::~Variable()
    {
        if (buffer)
        {
            Memory::Free(buffer);
            buffer = nullptr;
        }
    }
    
    PtrSize Variable::WriteValue(uint8 *dest, const Ryujin::JsonValue *value, Variable::Type inType)
    {
        switch (inType)
        {
            case Variable::Type::U8:
            {
                uint8 number = (uint8)value->AsNumber();
                Memory::MemCpy(dest, &number, sizeof(uint8));
                return sizeof(uint8);
            }
                
            case Variable::Type::U16:
            {
                uint16 number = (uint16)value->AsNumber();
                Memory::MemCpy(dest, &number, sizeof(uint16));
                return sizeof(uint16);
            }
                
            case Variable::Type::U32:
            {
                uint32 number = (uint32)value->AsNumber();
                Memory::MemCpy(dest, &number, sizeof(uint32));
                return sizeof(uint32);
            }
                
            case Variable::Type::U64:
            {
                uint64 number = (uint64)value->AsNumber();
                Memory::MemCpy(dest, &number, sizeof(uint64));
                return sizeof(uint64);
            }
                
            case Variable::Type::I8:
            {
                int8 number = (int8)value->AsNumber();
                Memory::MemCpy(dest, &number, sizeof(int8));
                return sizeof(int8);
            }
                
            case Variable::Type::I16:
            {
                int16 number = (int16)value->AsNumber();
                Memory::MemCpy(dest, &number, sizeof(int16));
                return sizeof(int16);
            }
                
            case Variable::Type::I32:
            {
                int32 number = (int32)value->AsNumber();
                Memory::MemCpy(dest, &number, sizeof(int32));
                return sizeof(int32);
            }
                
            case Variable::Type::I64:
            {
                int64 number = (int64)value->AsNumber();
                Memory::MemCpy(dest, &number, sizeof(int64));
                return sizeof(int64);
            }
                
            case Variable::Type::Float:
            {
                float number = value->AsNumber();
                Memory::MemCpy(dest, &number, sizeof(float));
                return sizeof(float);
            }
                
            case Variable::Type::Double:
            {
                double number = (double)value->AsNumber();
                Memory::MemCpy(dest, &number, sizeof(double));
                return sizeof(double);
            }
                
            case Variable::Type::Char:
            {
                char c = (char)value->AsNumber();
                Memory::MemCpy(dest, &c, sizeof(char));
                return sizeof(char);
            }
                
            case Variable::Type::String:
            {
                char buffer[MAX_VAR_NAME_LENGTH];
                String::Sprintf(buffer, "%s", value->AsString());
                Memory::MemCpy(dest, buffer, sizeof(buffer));
                return sizeof(buffer);
            }
                
            default:
                return 0;
        }
    }
    
    void* Variable::ConvertJsonDataToBinaryData(const JsonValue *value) const
    {
        uint8* data = static_cast<uint8*>(Memory::Malloc(GetVariableByteSize()));
        PtrSize offset = 0;
        if (value->IsArray())
        {
            const JsonArray& values = value->AsArray();
            for (const auto& v : values)
            {
                offset += WriteValue((data + offset), v, GetVariableType());
            }
        }
        else
        {
            offset += WriteValue((data + offset), value, GetVariableType());
        }
        
        return data;
    }
    
    void Variable::SetDefault(void* data, int32 byteSize)
    {
        if (!buffer)
        {
            buffer = Memory::Malloc(byteSize);
        }
        Memory::MemCpy(buffer, data, byteSize);
    }
    
    void* Variable::GetData() const
    {
        return buffer;
    }
    
    int32 Variable::GetVariableTypeByteSize() const
    {
        switch (type)
        {
            case Variable::Type::Char:
                return sizeof(char);
                
            case Variable::Type::String:
                return sizeof(char[MAX_VAR_NAME_LENGTH]);
                
            case Variable::Type::U8:
                return sizeof(uint8);
                
            case Variable::Type::U16:
                return sizeof(uint16);
                
            case Variable::Type::U32:
                return sizeof(uint32);
                
            case Variable::Type::U64:
                return sizeof(uint64);
                
            case Variable::Type::I8:
                return sizeof(int8);
                
            case Variable::Type::I16:
                return sizeof(int16);
                
            case Variable::Type::I32:
                return sizeof(int32);
                
            case Variable::Type::I64:
                return sizeof(int64);
                
            case Variable::Type::Float:
                return sizeof(float);
                
            case Variable::Type::Double:
                return sizeof(double);
                
            case Variable::Type::Unknown:
            default:
                return 0;
        }
    }
    
    int32 Variable::GetVariableByteSize() const
    {
        return IsArray() ? GetVariableTypeByteSize() * arrayCount : GetVariableTypeByteSize();
    }
    
    const char* Variable::GetName() const
    {
        return name;
    }
    
    Variable::Type Variable::GetVariableType() const
    {
        return type;
    }
    
    bool Variable::IsArray() const
    {
        return type != Variable::Type::String && arrayCount > 0;
    }
    
    int32 Variable::GetArrayCount() const
    {
        return arrayCount;
    }
    
    String Variable::GetVariableTypeToString() const
    {
        switch (type)
        {
            case Variable::Type::Char:
                return CHAR_TYPE_LITERAL;
                
            case Variable::Type::String:
                return CHAR_TYPE_LITERAL;//String::Printf("char[%d]", MAX_VAR_NAME_LENGTH);
                
            case Variable::Type::U8:
                return U8_TYPE_LITERAL;
                
            case Variable::Type::U16:
                return U16_TYPE_LITERAL;
                
            case Variable::Type::U32:
                return U32_TYPE_LITERAL;
                
            case Variable::Type::U64:
                return U64_TYPE_LITERAL;
                
            case Variable::Type::I8:
                return I8_TYPE_LITERAL;
                
            case Variable::Type::I16:
                return I16_TYPE_LITERAL;
                
            case Variable::Type::I32:
                return I32_TYPE_LITERAL;
                
            case Variable::Type::I64:
                return I64_TYPE_LITERAL;
                
            case Variable::Type::Float:
                return FLOAT_TYPE_LITERAL;
                
            case Variable::Type::Double:
                return DOUBLE_TYPE_LITERAL;
                
            case Variable::Type::Unknown:
            default:
                Warnf(Info, "Unknown variable encountered in Variable::GetVariableTypeToString()");
                return "";
        }
    }
    
    bool Variable::operator==(const Variable& right) const
    {
        return type == right.type && arrayCount == right.arrayCount && strcmp(name, right.name) == 0;
    }
    
    bool Variable::operator!=(const Variable& right) const
    {
        return type != right.type || arrayCount != right.arrayCount || strcmp(name, right.name) != 0;
    }
    
    Variable::Type Variable::GetVariableTypeFromString(const String &strType)
    {
        if (strType == STRING_TYPE_LITERAL)
        {
            return Variable::Type::String;
        }
        else if (strType == CHAR_TYPE_LITERAL)
        {
            return Variable::Type::Char;
        }
        else if (strType == U8_TYPE_LITERAL)
        {
            return Variable::Type::U8;
        }
        else if (strType == U16_TYPE_LITERAL)
        {
            return Variable::Type::U16;
        }
        else if (strType == U32_TYPE_LITERAL)
        {
            return Variable::Type::U32;
        }
        else if (strType == U64_TYPE_LITERAL)
        {
            return Variable::Type::U64;
        }
        else if (strType == U8_TYPE_LITERAL)
        {
            return Variable::Type::U8;
        }
        else if (strType == I8_TYPE_LITERAL)
        {
            return Variable::Type::I8;
        }
        else if (strType == I16_TYPE_LITERAL)
        {
            return Variable::Type::I16;
        }
        else if (strType == I32_TYPE_LITERAL)
        {
            return Variable::Type::I32;
        }
        else if (strType == I64_TYPE_LITERAL)
        {
            return Variable::Type::I64;
        }
        else if (strType == FLOAT_TYPE_LITERAL)
        {
            return Variable::Type::Float;
        }
        else if (strType == DOUBLE_TYPE_LITERAL)
        {
            return Variable::Type::Double;
        }
        
        return Variable::Type::Unknown;
    }
    
    String Variable::ToString(int32 tabs) const
    {
        String indent;
        for (int32 i = 0; i < tabs; ++i)
        {
            indent += "\t";
        }
        
        if (type == Variable::Type::String)
        {
            return String::Printf("%s%s %s[%d];\n", *indent, *GetVariableTypeToString(), name, arrayCount);
        }
        
        return IsArray() ?      String::Printf("%s%s %s[%d];\n", *indent, *GetVariableTypeToString(), name, arrayCount)
                            :   String::Printf("%s%s %s;\n", *indent, *GetVariableTypeToString(), name);
    }
    
    
    RyujinStruct::RyujinStruct()
    :
    byteSize(0)
    {
        
    }
    
    RyujinStruct::RyujinStruct(const char* structName)
    :
    stringId(structName), 
    byteSize(0)
    {
        
    }
    
    RyujinStruct::RyujinStruct(const RyujinStruct& other)
    :
    stringId(other.stringId),
    byteSize(other.byteSize)
    {
        variables.Resize(other.variables.GetSize());
        for (auto& variable : other.variables)
        {
            variables.Add(variable);
        }
    }
    
    RyujinStruct::~RyujinStruct()
    {
        
    }
    
    RyujinStruct& RyujinStruct::operator=(const RyujinStruct &other)
    {
        stringId = other.stringId;
        byteSize = other.byteSize;
        
        variables.Resize(other.variables.GetSize());
        for (auto& variable : other.variables)
        {
            variables.Add(variable);
        }
        
        return *this;
    }
    
    void RyujinStruct::AddVariable(const Variable &inVariable)
    {
        byteSize += inVariable.GetVariableByteSize();
        variables.Add(inVariable);
    }
    
    void RyujinStruct::ClearStruct()
    {
        variables.RemoveAll();
    }
    
    void RyujinStruct::RearrangeStructMemoryLayout()
    {
        // Sort the variables of the struct by largest to smallest
        std::sort(variables.Begin(), variables.End(),
                  [](const Variable& a, const Variable& b)
                  {
                      return a.GetVariableByteSize() > b.GetVariableByteSize();
                  });
    }
    
    bool RyujinStruct::Equals(const RyujinStruct &other) const
    {
        if (other.variables.GetSize() != variables.GetSize() ||
            other.GetStructByteSize() != GetStructByteSize())
            return false;
        
        for (uint32 i = 0; i < variables.GetSize(); ++i)
        {
            const auto& var1 = variables[i];
            const auto& var2 = other.variables[i];
            if (var1 != var2)
            {
                return false;
            }
        }
        
        return true;
    }
    
    uint8* RyujinStruct::GenerateInstanceBlob(const JsonObject& instanceData)
    {
        uint8* data = static_cast<uint8*>(Memory::Malloc(byteSize));
        PtrSize offset = 0;
        
        for (const auto& var : variables)
        {
            const auto& nameIt = instanceData.find(var.GetName());
            if (nameIt != instanceData.end())
            {
                // override value specified
                void* srcData = var.ConvertJsonDataToBinaryData(nameIt->second);
                Memory::MemCpy((data + offset), srcData, var.GetVariableByteSize());
                Memory::Free(srcData);
            }
            else
            {
                // use default value that is already stored in the variable object
                Memory::MemCpy((data + offset), var.GetData(), var.GetVariableByteSize());
            }
            
            offset += var.GetVariableByteSize();
        }
        
        return data;
    }
    
    void RyujinStruct::SetStructName(const char *structName)
    {
        stringId = StringHash(structName);
    }
    
    const Variable& RyujinStruct::GetVariableAt(int32 index) const
    {
        return variables[index];
    }
    
    uint32 RyujinStruct::GetHash() const
    {
        return stringId.GetHash();
    }
    
    const char* RyujinStruct::GetName() const
    {
        return stringId.ToString();
    }
    
    int32 RyujinStruct::GetStructByteSize() const
    {
        return byteSize;
    }
    
    String RyujinStruct::ToString(int32 tabs) const
    {
        String indent;
        for (int32 i = 0; i < tabs; ++i)
        {
            indent += "\t";
        }
        String str = String::Printf("\n%sstruct %s\n\t{\n", *indent, stringId.ToString());
        
        for (const auto& var : variables)
        {
            str += var.ToString(tabs + 1);
        }
        
        str += String::Printf("%s};\n\n", *indent);
        
        return str;
    }
    
    
    RyujinDataCompiler::RyujinDataCompiler()
    {
        
    }
    
    RyujinDataCompiler::~RyujinDataCompiler()
    {
        
    }
    
    void RyujinDataCompiler::AddFileToCompile(const Ryujin::String &file)
    {
        filesToCompile.Add(file);
    }
    
    void RyujinDataCompiler::SetVariableDefaultFromValue(Variable& var, const JsonValue* value)
    {
        switch (var.GetVariableType())
        {
            case Variable::Type::U8:
            {
                uint8 number = (uint8)value->AsNumber();
                var.SetDefault(&number, sizeof(uint8));
            }
                break;
                
            case Variable::Type::U16:
            {
                uint16 number = (uint16)value->AsNumber();
                var.SetDefault(&number, sizeof(uint16));
            }
                break;
                
            case Variable::Type::U32:
            {
                uint32 number = (uint32)value->AsNumber();
                var.SetDefault(&number, sizeof(uint32));
            }
                break;
                
            case Variable::Type::U64:
            {
                uint64 number = (uint64)value->AsNumber();
                var.SetDefault(&number, sizeof(uint64));
            }
                break;
            case Variable::Type::I8:
            {
                int8 number = (int8)value->AsNumber();
                var.SetDefault(&number, sizeof(int8));
            }
                break;
                
            case Variable::Type::I16:
            {
                int16 number = (int16)value->AsNumber();
                var.SetDefault(&number, sizeof(int16));
            }
                break;
                
            case Variable::Type::I32:
            {
                int32 number = (int32)value->AsNumber();
                var.SetDefault(&number, sizeof(int32));
            }
                break;
                
            case Variable::Type::I64:
            {
                int64 number = (int64)value->AsNumber();
                var.SetDefault(&number, sizeof(int64));
            }
                break;
                
            case Variable::Type::Float:
            {
                float number = value->AsNumber();
                var.SetDefault(&number, sizeof(float));
            }
                break;
                
            case Variable::Type::Double:
            {
                double number = (double)value->AsNumber();
                var.SetDefault(&number, sizeof(double));
            }
                break;
                
            case Variable::Type::Char:
            {
                char c = (char)value->AsNumber();
                var.SetDefault(&c, sizeof(char));
            }
                break;
                
            case Variable::Type::String:
            {
                char buffer[MAX_VAR_NAME_LENGTH];
                String::Sprintf(buffer, "%s", value->AsString());
                var.SetDefault(buffer, sizeof(buffer));
            }
                break;
                
            default:
                break;
        }
    }
    
    void RyujinDataCompiler::SetVariableDefaultFromArray(Variable& var, const JsonArray& values)
    {
        const int32 count = values.GetSize();
        uint8* data = (uint8*)Memory::Malloc(count * var.GetVariableTypeByteSize());
        PtrSize offset = 0;
        for (uint32 i = 0; i < values.GetSize(); ++i)
        {
            switch (var.GetVariableType())
            {
                case Variable::Type::U8:
                {
                    uint8 number = (uint8)values[i]->AsNumber();
                    Memory::MemCpy((data + offset), &number, sizeof(uint8));
                    offset += sizeof(uint8);
                }
                    break;
                    
                case Variable::Type::U16:
                {
                    uint16 number = (uint16)values[i]->AsNumber();
                    Memory::MemCpy((data + offset), &number, sizeof(uint16));
                    offset += sizeof(uint16);
                }
                    break;
                    
                case Variable::Type::U32:
                {
                    uint32 number = (uint32)values[i]->AsNumber();
                    Memory::MemCpy((data + offset), &number, sizeof(uint32));
                    offset += sizeof(uint32);
                }
                    break;
                    
                case Variable::Type::U64:
                {
                    uint64 number = (uint64)values[i]->AsNumber();
                    Memory::MemCpy((data + offset), &number, sizeof(uint64));
                    offset += sizeof(uint64);
                }
                    break;
                case Variable::Type::I8:
                {
                    int8 number = (int8)values[i]->AsNumber();
                    Memory::MemCpy((data + offset), &number, sizeof(int8));
                    offset += sizeof(int8);
                }
                    break;
                    
                case Variable::Type::I16:
                {
                    int16 number = (int16)values[i]->AsNumber();
                    Memory::MemCpy((data + offset), &number, sizeof(int16));
                    offset += sizeof(int16);
                }
                    break;
                    
                case Variable::Type::I32:
                {
                    int32 number = (int32)values[i]->AsNumber();
                    Memory::MemCpy((data + offset), &number, sizeof(int32));
                    offset += sizeof(int32);
                }
                    break;
                    
                case Variable::Type::I64:
                {
                    int64 number = (int64)values[i]->AsNumber();
                    Memory::MemCpy((data + offset), &number, sizeof(int64));
                    offset += sizeof(int64);
                }
                    break;
                    
                case Variable::Type::Float:
                {
                    float number = values[i]->AsNumber();
                    Memory::MemCpy((data + offset), &number, sizeof(float));
                    offset += sizeof(float);
                }
                    break;
                    
                case Variable::Type::Double:
                {
                    double number = (double)values[i]->AsNumber();
                    Memory::MemCpy((data + offset), &number, sizeof(double));
                    offset += sizeof(double);
                }
                    
                case Variable::Type::Char:
                {
                    char c = (char)values[i]->AsNumber();
                    Memory::MemCpy((data + offset), &c, sizeof(char));
                    offset += sizeof(char);
                }
                    break;
                    
                case Variable::Type::String:
                {
                    char buffer[MAX_VAR_NAME_LENGTH];
                    String::Sprintf(buffer, "%s", values[i]->AsString());
                    Memory::MemCpy((data + offset), buffer, sizeof(buffer));
                    offset += sizeof(buffer);
                }
                    break;
            }
        }
        
        var.SetDefault(data, count * var.GetVariableTypeByteSize());
    }
    
    String RyujinDataCompiler::BuildStruct(const JsonObject& value)
    {
        StringHash structHash(value.find("structName")->second->AsString());
        RyujinStruct dataStruct(structHash.ToString());
        
        int32 numVariables = (int32)(value.size() - 1); // -1 to account for "structName"
        char varName[16];
        for (int i = 0; i < numVariables; ++i)
        {
            String::Sprintf(varName, "var%d", i);
            const auto& varIt = value.find( varName );
            if (varIt != value.end())
            {
                const JsonObject& varObject = varIt->second->AsObject();
                String type = varObject.find("varType")->second->AsString();
                int32 arrayCount = 0;
                
                const auto& arrayCountIt = varObject.find("arrayCount");
                if (arrayCountIt != varObject.end())
                {
                    arrayCount = (int32)arrayCountIt->second->AsNumber();
                }
                else if (type == STRING_TYPE_LITERAL)
                {
                    arrayCount = MAX_VAR_NAME_LENGTH;
                }
                
                Variable::Type varType = Variable::GetVariableTypeFromString(type);
                Variable var(varObject.find("varName")->second->AsString(),
                             varType,
                             arrayCount);
                
                const auto& defaultIt = varObject.find("default");
                RYUJIN_ASSERT(defaultIt != varObject.end(),
                              *String::Printf("Default value not specified for var %s in struct %s", var.GetName(), structHash.ToString()));
                
                if (varType != Variable::Type::String && arrayCount)
                {
                    const auto& defaultArray = defaultIt->second->AsArray();
                    SetVariableDefaultFromArray(var, defaultArray);
                }
                else
                {
                    SetVariableDefaultFromValue(var, defaultIt->second);
                }
                
                dataStruct.AddVariable(var);
            }
        }
        
        dataStruct.RearrangeStructMemoryLayout();
        
        RyujinStruct mapStruct;
        if (structMap.TryGetValue(dataStruct.GetHash(), mapStruct))
        {
            // struct has already been declared
            RYUJIN_ASSERT(mapStruct.Equals(dataStruct), *String::Printf("%s redeclared with different layout", dataStruct.GetName()));
        }
        else
        {
            structMap.Add(dataStruct.GetHash(), dataStruct);
        }
        
        return dataStruct.ToString(1);
    }
    
    void RyujinDataCompiler::BuildInstances(const JsonArray& instances, const String& outputFile)
    {
        FileHandle fileHandle = File::Open(*outputFile, FileMode::FM_Write, true);
        RYUJIN_ASSERT(fileHandle, *String::Printf("Failed to create file %s", *outputFile));
        
        TableOfContent toc;
        PtrSize tocOffset = 0;
        
        // Build the table of content
        for (const auto& object : instances)
        {
            const JsonObject& instance = object->AsObject();
            StringHash basedOnStruct(instance.find("basedOn")->second->AsString());
            RyujinStruct baseStruct;
            RYUJIN_ASSERT(structMap.TryGetValue(basedOnStruct.GetHash(), baseStruct), *String::Printf("%s not found", basedOnStruct.ToString()));
            
            StringHash instanceStringId(instance.find("instance")->second->AsString());
            TocEntry tocEntry(instanceStringId, tocOffset, baseStruct.GetStructByteSize());
            toc.AddEntry(tocEntry);
            tocOffset += baseStruct.GetStructByteSize();
        }
        
        // NOTE: changes to the TOC file header need to be accounted for in TableOfContent::LookupSymbol
        int32 tocEntryCount = toc.GetTable().GetSize();
        File::Write(&tocEntryCount, sizeof(int32), 1, fileHandle);
        File::Write(toc.GetTable().GetData(), sizeof(TocEntry), toc.GetTable().GetSize(), fileHandle);
        
        // Write instance data to file
        for (const auto& object : instances)
        {
            const JsonObject& instance = object->AsObject();
            StringHash basedOnStruct(instance.find("basedOn")->second->AsString());
            RyujinStruct baseStruct;
            structMap.TryGetValue(basedOnStruct.GetHash(), baseStruct);
            
            File::Write(baseStruct.GenerateInstanceBlob(instance), sizeof(uint8), baseStruct.GetStructByteSize() / sizeof(uint8), fileHandle);
        }
        
        File::Close(fileHandle);
    }
    
    void RyujinDataCompiler::CompileFiles()
    {
        Debugf(Info, "Compiling ", filesToCompile.GetSize(), " files");
        
        for (const auto& file : filesToCompile)
        {
            UniquePtr<JsonValue> json(Json::ParseFile(*(DATA_COMPILER_PATH + file)));
            RYUJIN_ASSERT(json, *String::Printf("Failed to parse json %s", *file));
            
            const JsonObject& root = json->AsObject();
            
            String headerFile = file;
            headerFile.Replace(headerFile.Find(".json"), 5, ".hpp");
            
            String headerFileStr = String::Printf("\n/* \n\tAUTO GENERATED FILE USING RYUJIN DATA COMPILER \n*/\n\n#pragma once\n\nnamespace Ryujin \n{\n");
            
            JsonArray structs = root.find("structs")->second->AsArray();
            for (const auto& s : structs)
            {
                headerFileStr += BuildStruct(s->AsObject());
            }
            
            headerFileStr += "}";
//            Debugf(Info, *headerFileStr);
            
            // Check if header file content changed
            String filepath = String::Printf("%s%s", root.find("outputLocation")->second->AsString(), *headerFile);
            uint32 headerFileCRC = CRC::MemCrc(*headerFileStr, headerFileStr.Length() * sizeof(char));
            if (HasFileContentChanged(*filepath, headerFileCRC))
            {
                Debugf(Info, "Writing header file ", *filepath, " to disk.");
                FileHandle fileHandle = File::Open(*filepath, FileMode::FM_Write);
                if (fileHandle)
                {
                    File::Write(*headerFileStr, sizeof(char), headerFileStr.Length(), fileHandle);
                    File::Close(fileHandle);
                }
            }
            
            // Generate the instance data file
            String dataFile = DATA_COMPILER_PATH + file;
            dataFile.Replace(dataFile.Find(".json"), 5, ".dat");
            
            const auto& instanceArray = root.find("instances")->second->AsArray();
            Debugf(Info, "Processing ", instanceArray.GetSize(), " instances");
            BuildInstances(instanceArray, dataFile);
        }
    }
    
    bool RyujinDataCompiler::HasFileContentChanged(const char *filename, uint32 newContentCrc) const
    {
        FileHandle file = File::Open(filename, FileMode::FM_Read);
        if (!file)
        {
            return true;
        }
        
        uint64 fileSize = (uint64)File::FileSize(file);
        char* data = static_cast<char*>(Memory::Malloc(fileSize * sizeof(char)));
        File::Read(data, sizeof(char), fileSize, file);
        
        uint32 crc = CRC::MemCrc(data, fileSize * sizeof(char));
        
        Memory::Free(data);
        
        return crc != newContentCrc;
    }
}
