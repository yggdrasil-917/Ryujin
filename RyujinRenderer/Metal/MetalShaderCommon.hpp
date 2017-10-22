
#include "../Renderer/ShaderCommonDefines.hpp"


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
