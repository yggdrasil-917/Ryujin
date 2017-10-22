


#if COMPILING_METAL
#include "../Metal/MetalShaderCommon.hpp"

#define META_DATA(x) x

#else

// PC
#include "../Renderer/ShaderCommonDefines.hpp"

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
