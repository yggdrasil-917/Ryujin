
#include "../RyujinUnitTesting/RyujinUnitTesting.hpp"
#include "../RyujinUnitTesting/TestCase.hpp"


#include "RyujinExtensions/Utils/DOTExporter.hpp"



namespace Ryujin
{
    TEST_CASE(SimpleDOT, "Test to make basic dot file")
    {
        DOTExporter dot;
        REQUIRE(dot.Create("testGraph", true));

        DOTAttributeBuilder builder;
        dot.AddNodeAttribute("a", builder.SetNodeName("Foo").SetNodeShape(DOTShape::box).ToString());
        dot.AddEdge("a", "b", "[color=blue]");
        dot.AddEdge("b", "c");
        dot.AddEdge("b", "d");
        dot.FlushContentBuffer();
        dot.Finish();
    }
}
