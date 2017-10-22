#pragma once

#include "RyujinCore/Containers/String.hpp"
#include "RyujinCore/OS/FileSystem.hpp"
#include "RyujinCore/CoreUtils/SmartEnum.hpp"
#include "RyujinCore/Threading/Thread.hpp"
#include "ExtensionsBase.hpp"


namespace Ryujin
{
#define DOT_FILEPATH "DOT/"
    
    SMART_ENUM(DOTShape, uint8,
               box = 1 << 0,
               ellipse = 1 << 1,
               plaintext = 1 << 2);
    
    SMART_ENUM(DOTEdgeColor, uint8,
               black = 1 << 0,
               red = 1 << 1,
               blue = 1 << 2,
               green = 1 << 3);
    
    SMART_ENUM(DOTEdgeStyle, uint8,
               filled = 1 << 0,
               dotted = 1 << 1);
    
    class EXTENSIONS_API DOTAttributeBuilder
    {
    private:
        String attribs;
        
        
    public:
        DOTAttributeBuilder& SetNodeName(const char* name);
        DOTAttributeBuilder& SetNodeShape(DOTShape shape);
        DOTAttributeBuilder& SetEdgeColor(DOTEdgeColor color);
        DOTAttributeBuilder& SetEdgeStyle(DOTEdgeStyle style);
        
        FORCEINLINE const String& ToString() const { return attribs; }
    };
    
    //https://en.wikipedia.org/wiki/DOT_%28graph_description_language%29
    // Need to have GraphViz toolkit to generate pdf afterwards
    // Use terminal command dot -Tpdf graph.dot -o graph.pdf to make a pdf and visualize the dot file
    class EXTENSIONS_API DOTExporter
    {
    private:
        String content;
        bool bIsDigraph = true;
        FileHandle file;
        Mutex writeLock;
        
    public:
        ~DOTExporter();
        bool Create(const char* graphName, bool bIsDirected);
        void Finish();
        void AddEdge(const String& from, const String& to);
        void AddEdge(const String& from, const String& to, const String& attribute);
        void AddNodeAttribute(const String& node, const String& attribute);
        void FlushContentBuffer();
    };
}
