#include "DOTExporter.hpp"
#include "RyujinCore/OS/Application.hpp"


namespace Ryujin
{
    DOTAttributeBuilder& DOTAttributeBuilder::SetNodeName(const char *name)
    {
        attribs += String::Printf("[label=\"%s\"]", name);
        return *this;
    }
    
    DOTAttributeBuilder& DOTAttributeBuilder::SetNodeShape(DOTShape shape)
    {
        attribs += String::Printf("[shape=%s]", shape.ToString());
        return *this;
    }
    
    DOTAttributeBuilder& DOTAttributeBuilder::SetEdgeColor(DOTEdgeColor color)
    {
        attribs += String::Printf("[color=%s]", color.ToString());
        return *this;
    }
    
    DOTAttributeBuilder& DOTAttributeBuilder::SetEdgeStyle(DOTEdgeStyle style)
    {
        attribs += String::Printf("[style=%s]", style.ToString());
        return *this;
    }
    
    DOTExporter::~DOTExporter()
    {
        if (file)
        {
            File::Close(file);
            file = nullptr;
        }
    }
    
    bool DOTExporter::Create(const char* graphName, bool bIsDirected)
    {
        String dotDir = String::Printf("%s../%s", AppInfo::GetDevCookedContentDir(), DOT_FILEPATH);
        if (!Directory::DirectoryExists(*dotDir))
        {
            Directory::CreateDirectory(*dotDir);
        }
        dotDir += graphName;
        dotDir += ".dot";
        file = File::Open(*dotDir, FileMode::FM_Write);
        if (!file)
            return false;
        
        if (bIsDirected)
        {
            content = String::Printf("digraph %s {\nrankdir=LR;\n", graphName);
        }
        else
        {
            content = String::Printf("graph %s {\nrankdir=LR\n", graphName);
        }
        FlushContentBuffer();
        
        return true;
    }
    
    void DOTExporter::AddEdge(const String& from, const String& to)
    {
        ScopedLock lock(writeLock);
        content += String::Printf("%s %s %s;\n", *from, (bIsDigraph ? " -> " : " -- "), *to);
    }
    
    void DOTExporter::AddEdge(const String& from, const String& to, const String& attribute)
    {
        ScopedLock lock(writeLock);
        content += String::Printf("%s %s %s %s;\n", *from, (bIsDigraph ? " -> " : " -- "), *to, *attribute);
    }
    
    void DOTExporter::AddNodeAttribute(const String& node, const String& attribute)
    {
        ScopedLock lock(writeLock);
        content += String::Printf("%s %s;\n", *node, *attribute);
    }
    
    void DOTExporter::FlushContentBuffer()
    {
        ScopedLock lock(writeLock);
        File::Write(content, sizeof(char), content.Length(), file);
        content = "";
    }
    
    void DOTExporter::Finish()
    {
        ScopedLock lock(writeLock);
        content += "}";
        File::Write(content, sizeof(char), content.Length() + 1, file);
        content = "";
        File::Close(file);
        file = nullptr;
    }
}
