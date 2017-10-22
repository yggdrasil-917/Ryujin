#define COMPILING_METAL 1
#include <metal_stdlib>
#include <simd/simd.h>
using namespace metal;





#if COMPILING_METAL



#define VERTEX_BUFFER_SLOT  0
#define FRAME_CB_SLOT       1
#define MESH_CB_SLOT        2


#define float4x4 matrix_float4x4
#define float3x3 matrix_float3x3
#define float4 vector_float4
#define float3 vector_float3
#define float2 vector_float2


#define CB_STRUCT(name, code) \
typedef struct __attribute__((__aligned__(256))) { \
code \
} name;

#define VERTEX_BUFFER  [[buffer(VERTEX_BUFFER_SLOT)]]
#define FRAME_CB       [[buffer(FRAME_CB_SLOT)]]
#define MESH_CB        [[buffer(MESH_CB_SLOT)]]

#define POS_ATTRIBUTE       [[attribute(0)]]
#define NORMAL_ATTRIBUTE    [[attribute(1)]]
#define UV_ATTRIBUTE        [[attribute(2)]]
#define COLOR_ATTRIBUTE     [[attribute(3)]]

#define POS_SEMANTIC [[position]]

#define VERTEX_ID [[vertex_id]]

#define DECLARE_TEX2D(name, type, slot) texture2d<type> name [[texture(slot)]], sampler sampler##name [[sampler(slot)]]
#define DECLARE_TEX2D_NO_SAMPLER(name, type) texture2d<type> name [[texture(slot)]]

#define DECLARE_TEXCUBE(name, type, slot) texturecube<type> name [[texture(slot)]], sampler sampler##name [[sampler(slot)]]
#define DECLARE_TEXCUBE_NO_SAMPLER(name, type) texturecube<type> name [[texture(slot)]]

#define SAMPLE_TEX2D(t, uv) t.sample(sampler##t, uv);
#define SAMPLE_TEXCUBE(t, uv) t.sample(sampler##t, uv);

#define VERTEX_ENTRY vertex
#define FRAG_ENTRY fragment


#define INPUT(x) x [[stage_in]]

#define PACKED __attribute((packed))

#define META_DATA(x) x

#else

// PC


#define VERTEX_BUFFER_SLOT  0
#define FRAME_CB_SLOT       1
#define MESH_CB_SLOT        2

#define CB_STRUCT(name, code) STRUCT_ALIGN(name, code, 256)
#define META_DATA(x)

#define float4x4    Matrix44
#define float3x3    Matrix33
#define float4      Vector4
#define float3      Vector3
#define float2      Vector2

#define POS_ATTRIBUTE           0
#define NORMAL_ATTRIBUTE        1
#define UV_ATTRIBUTE            2
#define COLOR_ATTRIBUTE         3

#define PACKED
#endif

typedef struct PACKED
{
    float4 position;
    float4 color;
} VertexIn;

typedef struct PACKED
{
    float4 position;
} Vertex;

typedef struct PACKED
{
    float4 position META_DATA(POS_ATTRIBUTE);
    float2 uv META_DATA(UV_ATTRIBUTE);
} VertexBase;

#pragma pack(push, 1)
CB_STRUCT(PerFrameCB,
          float4x4 view;
          float4x4 viewInv;
          float4x4 projection;
          float4x4 projectionInv;
          float4x4 viewProjection;
          float4x4 viewProjectionInv;
          float3 cameraWorldPosition;
          bool pad[28];
          );

CB_STRUCT(PerObjectCB,
          float4x4 worldMatrix;
          float3x3 normalMatrix;
          bool pad[156];
          );

CB_STRUCT(Uniform,
          float4 color;
          bool pad[240];
          );
#pragma pack(pop)





struct ColoredVertex
{
    float4 position POS_SEMANTIC;
    float4 color;
};

struct V2F
{
    float4 position POS_SEMANTIC;
    float2 uv;
};

struct V2FBasic
{
    float4 position POS_SEMANTIC;
};

VERTEX_ENTRY V2FBasic vertex_basic(constant VertexIn* vertices VERTEX_BUFFER,
                               constant PerObjectCB& uniforms MESH_CB,
                               constant PerFrameCB& perFrameCB FRAME_CB,
                               uint vid VERTEX_ID)
{
    V2FBasic o;
    o.position = perFrameCB.viewProjection * uniforms.worldMatrix * vertices[vid].position;
    return o;
}

FRAG_ENTRY float4 fragment_basic(INPUT(V2FBasic vert), constant Uniform& uniforms MESH_CB)
{
    return uniforms.color;
}

VERTEX_ENTRY ColoredVertex vertex_main(constant VertexIn* vertices VERTEX_BUFFER,
                                       constant PerObjectCB& uniforms MESH_CB,
                                       constant PerFrameCB& perFrameCB FRAME_CB,
                                       uint vid VERTEX_ID)
{
    ColoredVertex vert;
    vert.position = perFrameCB.viewProjection * uniforms.worldMatrix * vertices[vid].position;
    vert.color = vertices[vid].color; 
    return vert;
}

FRAG_ENTRY float4 fragment_main(INPUT(ColoredVertex vert))
{
    return vert.color;
}


VERTEX_ENTRY V2F vertex_textured(constant VertexBase* vertices VERTEX_BUFFER,
                                   constant PerObjectCB& uniforms MESH_CB,
                                   constant PerFrameCB& perFrameCB FRAME_CB,
                                   uint vid VERTEX_ID)
{
    V2F vert;
    vert.position = perFrameCB.viewProjection * uniforms.worldMatrix * vertices[vid].position;
    vert.uv = vertices[vid].uv;
    return vert;
}



FRAG_ENTRY float4 fragment_textured(INPUT(V2F vert),
                                    DECLARE_TEX2D(diffuse, float, 0))
{
    return SAMPLE_TEX2D(diffuse, vert.uv);
}

VERTEX_ENTRY ColoredVertex skyboxVertex(constant VertexIn* vertices VERTEX_BUFFER,
                              constant PerFrameCB& perFrameCB FRAME_CB,
                              uint vid VERTEX_ID)
{
    float4 position = vertices[vid].position;
    
    ColoredVertex outVert;
    outVert.position = perFrameCB.viewProjection * vertices[vid].position;
    outVert.color.xyz = (position.xyz - perFrameCB.cameraWorldPosition);
    return outVert;
}

FRAG_ENTRY half4 skyboxFragment(INPUT(ColoredVertex vert),
                                DECLARE_TEXCUBE(skybox, half, 0))
{
    float3 texCoords = float3(vert.color.x, vert.color.y, -vert.color.z);
    return SAMPLE_TEXCUBE(skybox, texCoords);
}
