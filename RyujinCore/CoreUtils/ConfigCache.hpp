#pragma once


#include "../Containers/String.hpp"
#include "../CoreUtils/SmartPointer.hpp"
#include "../Containers/HashTable.hpp"
#include "../OS/FileSystem.hpp"
#include "../Logger/Logger.hpp"


#define CONFIG_PATH "Config/"

namespace Ryujin
{
	struct DataValue
	{
		union
		{
			bool boolVal;
			int32 intVal;
			float floatVal;
			char* stringVal;
		};
		bool bIsString;

		DataValue() : bIsString(false) {}
	};
    
    class CORE_API ConfigCache
    {
    private:
        StringMap<StringMap<SharedPtr<DataValue>>> configValues;
        
        void InitializeSectionMap(StringMap<SharedPtr<DataValue>>& sectionMap, FileHandle fp);
        
    public:
        ~ConfigCache();
        
        bool LoadConfig(const char* filename);
        
        FORCEINLINE bool GetBool(const char* section, const char* key, bool& value, const bool defaultValue = false) const
        {
            StringMap<SharedPtr<DataValue>> sectionMap;
            if (!configValues.TryGetValue(section, sectionMap))
            {
                value = defaultValue;
                return false;
            }
            
            auto newValue = sectionMap[key];
            if (!newValue)
            {
                value = defaultValue;
                return false;
            }
            
            value = newValue->boolVal;
            return true;
        }
        
        FORCEINLINE bool GetFloat(const char* section, const char* key, float& value, const float defaultValue = 0.0f) const
        {
            StringMap<SharedPtr<DataValue>> sectionMap;
            if (!configValues.TryGetValue(section, sectionMap))
            {
                value = defaultValue;
                return false;
            }
            
            auto newValue = sectionMap[key];
            if (!newValue)
            {
                value = defaultValue;
                return false;
            }
            
            value = newValue->floatVal;
            return true;
        }
        
        FORCEINLINE bool GetInt(const char* section, const char* key, int32& value, const int32 defaultValue = 0) const
        {
            StringMap<SharedPtr<DataValue>> sectionMap;
            if (!configValues.TryGetValue(section, sectionMap))
            {
                value = defaultValue;
                return false;
            }
            
            auto newValue = sectionMap[key];
            if (!newValue)
            {
                value = defaultValue;
                return false;
            }
            
            value = newValue->intVal;
            return true;
        }
        
        FORCEINLINE bool GetString(const char* section, const char* key, String& value, const String defaultValue = "") const
        {
            StringMap<SharedPtr<DataValue>> sectionMap;
            if (!configValues.TryGetValue(section, sectionMap))
            {
                value = defaultValue;
                return false;
            }
            
            auto newValue = sectionMap[key];
            if (!newValue)
            {
                value = defaultValue;
                return false;
            }
            
            value = newValue->stringVal;
            return true;
        }
    };
}

