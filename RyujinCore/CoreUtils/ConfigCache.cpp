#include "ConfigCache.hpp"
#include "../OS/Application.hpp"


#if PLATFORM_WINDOWS
#pragma warning( push )
#pragma warning(disable: 6031) // return value ignored warning
#endif


namespace Ryujin
{
    ConfigCache::~ConfigCache()
    {
        StringMap<StringMap<SharedPtr<DataValue>>>::Iterator it(configValues);
        while (it)
        {
            StringMap<SharedPtr<DataValue>>::Iterator it2(it->value);
            while (it2)
            {
                auto value = it2->value;
                if (value && value->bIsString)
                    delete[] value->stringVal;
                ++it2;
            }
            ++it;
        }
    }
    
    bool ConfigCache::LoadConfig(const char *filename)
    {
        if (!filename)
            return false;
        
        String configFilepath = String::Printf("%s%s", AppInfo::GetResourcesDir(), CONFIG_PATH);
        String filepath = String::Concat(*configFilepath, filename);
        FileHandle fp = File::Open(*filepath, FileMode::FM_Read);
        if (!fp)
        {
            fp = File::Open(*String::Printf("../../../Content/Config/%s", filename), FileMode::FM_Read);
            if (!fp)
                return false;
        }
        
        char line[512];
        while (File::ReadLine(line, 512, fp))
        {
            if (line[0] == '#' || line[0] == '\r' || line[0] == '\n')	// skip if it is an empty line or a comment
                continue;
            
            if (line[0] == '[') // start of a section
            {
                StringMap<SharedPtr<DataValue>> sectionMap;
                char sectionKey[64];
                sscanf(line, "[%[^]]", sectionKey);
                InitializeSectionMap(sectionMap, fp);
                configValues.Add(sectionKey, sectionMap);
            }
        }
        
        return true;
    }
    
    void ConfigCache::InitializeSectionMap(StringMap<SharedPtr<DataValue>>& sectionMap, FileHandle fp)
    {
        char line[512];
        fpos_t position = 0;
        while (File::ReadLine(line, 512, fp))
        {
            if (line[0] == '#' || line[0] == '\r' || line[0] == '\n')	// skip if it is an empty line or a comment
                continue;
            
            if (line[0] == '[')
            {
                // start of another section so we return
                fsetpos(fp, &position);
                break;
            }
            
            char key[64], value[128];
            int matches = sscanf(line, "%s = %[^\r\t\n]", key, value);
            if (matches != 2)
                sscanf(line, "%s=%[^\r\t\n]", key, value);
            
            SharedPtr<DataValue> dataValue(RYUJIN_NEW DataValue);
            bool bShouldAddValue = false;
            if (value[0] == 't' || value[0] == 'f')
            {
                // boolean
                dataValue->boolVal = value[0] == 't' ? true : false;
                bShouldAddValue = true;
            }
            else if (value[0] == '"')
            {
                // string
                dataValue->stringVal = RYUJIN_NEW char[128];
                sscanf(value, "\"%[^\"]\"", dataValue->stringVal);
                dataValue->bIsString = true;
                bShouldAddValue = true;
            }
            else
            {
                // number
                PtrSize valueLength = strlen(value);
                bool floatFound = false;
                if (value[valueLength] == 'f' || value[valueLength] == 'F')
                {
                    sscanf(value, "%f", &dataValue->floatVal);
                    floatFound = true;
                    bShouldAddValue = true;
                }
                else
                {
                    // search to see if there is a decimal point
                    for (uint32 i = 0; i < valueLength; ++i)
                    {
                        if (value[i] == '.')
                        {
                            // we have a float
                            sscanf(value, "%f", &dataValue->floatVal);
                            floatFound = true;
                            bShouldAddValue = true;
                            break;
                        }
                    }
                }
                
                if (!floatFound)
                {
                    int32 i = sscanf(value, "%d", &dataValue->intVal);
                    if (i == 0)
                    {
                        char c;
                        sscanf(value, "%c", &c);
                        dataValue->intVal = int32(c);
                    }
                    bShouldAddValue = true;
                }
            }
            
            if (bShouldAddValue)
            {
                sectionMap.Add(key, dataValue);
            }
            fgetpos(fp, &position);
        }
    }
}



#if PLATFORM_WINDOWS
#pragma warning( pop )
#endif
