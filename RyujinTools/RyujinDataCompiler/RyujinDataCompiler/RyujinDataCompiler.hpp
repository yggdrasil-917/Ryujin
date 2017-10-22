
#ifndef RYUJIN_DATA_COMPILER_HPP
#define RYUJIN_DATA_COMPILER_HPP


#include "RyujinCore/CoreUtils/Json.hpp"
#include "RyujinCore/CoreUtils/SmartPointer.hpp"
#include "RyujinCore/CoreUtils/StringHash.hpp"
#include "RyujinCore/Containers/HashTable.hpp"


namespace Ryujin
{
#define DATA_COMPILER_PATH "../../../Content/DataCompiler/"
    
#define MAX_VAR_NAME_LENGTH 64
    
#define CHAR_TYPE_LITERAL       "char"
#define STRING_TYPE_LITERAL     "string"
#define U8_TYPE_LITERAL         "uint8"
#define U16_TYPE_LITERAL        "uint16"
#define U32_TYPE_LITERAL        "uint32"
#define U64_TYPE_LITERAL        "uint64"
#define I8_TYPE_LITERAL         "int8"
#define I16_TYPE_LITERAL        "int16"
#define I32_TYPE_LITERAL        "int32"
#define I64_TYPE_LITERAL        "int64"
#define FLOAT_TYPE_LITERAL      "float"
#define DOUBLE_TYPE_LITERAL     "double"
    
    class Variable
    {
    public:
        enum class Type : uint8
        {
            Char,
            String,
            
            U8,
            U16,
            U32,
            U64,
            
            I8,
            I16,
            I32,
            I64,
            
            Float,
            Double,
            
            Unknown,
        };
        
    private:
        Type type;
        char name[MAX_VAR_NAME_LENGTH];
        int32 arrayCount;
        void* buffer;
        
    public:
        Variable();
        Variable(const char* inName, Type inType, int32 inArrayCount = 0);
        Variable(const Variable& other);
        ~Variable();
        
        Variable& operator=(const Variable& other);
        
        void* ConvertJsonDataToBinaryData(const JsonValue* value) const;
        void SetDefault(void* data, int32 byteSize);
        void* GetData() const;
        
        int32 GetVariableTypeByteSize() const;
        int32 GetVariableByteSize() const;
        const char* GetName() const;
        Variable::Type GetVariableType() const;
        bool IsArray() const;
        int32 GetArrayCount() const;
        
        static Variable::Type GetVariableTypeFromString(const String& strType);
        static PtrSize WriteValue(uint8* dest, const JsonValue* value, Variable::Type inType);
        
        bool operator==(const Variable& right) const;
        bool operator!=(const Variable& right) const;
        
        String GetVariableTypeToString() const;
        String ToString(int32 tabs) const;
    };
    
    class RyujinStruct
    {
    private:
        DynamicArray<Variable> variables;
        StringHash stringId;
        int32 byteSize;
        
    public:
        RyujinStruct();
        RyujinStruct(const char* structName);
        RyujinStruct(const RyujinStruct& other);
        ~RyujinStruct();
        
        RyujinStruct& operator=(const RyujinStruct& other);
        
        void AddVariable(const Variable& inVariable);
        void ClearStruct();
        void RearrangeStructMemoryLayout();
        
        bool Equals(const RyujinStruct& other) const;
        uint8* GenerateInstanceBlob(const JsonObject& instanceData);
        
        void SetStructName(const char* structName);
        const Variable& GetVariableAt(int32 index) const;
        uint32 GetHash() const;
        const char* GetName() const;
        int32 GetStructByteSize() const;
        
        String ToString(int32 tabs) const;
    };
    
    class RyujinDataCompiler
    {
    private:
        DynamicArray<String> filesToCompile;
        HashTable<uint32, RyujinStruct> structMap;
        
        String BuildStruct(const JsonObject& value);
        void SetVariableDefaultFromArray(Variable& var, const JsonArray& values);
        void SetVariableDefaultFromValue(Variable& var, const JsonValue* value);
        
        void BuildInstances(const JsonArray& instances, const String& outputFile);
        
    public:
        RyujinDataCompiler();
        ~RyujinDataCompiler();
        
        void AddFileToCompile(const String& file);
        void CompileFiles();
        
        bool HasFileContentChanged(const char* filename, uint32 newContentCrc) const;
    };
}


#endif