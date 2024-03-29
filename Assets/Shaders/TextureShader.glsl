// Basic Texture Shader

#type vertex
#version 450 core

layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec3 a_Normal;
layout(location = 2) in vec2 a_TexCoord;
layout(location = 3) in float a_TexIndex;
layout(location = 4) in int a_EntityId;

struct PointLight {
    vec3 Ka;
    vec3 Kd;
    vec3 Ks;
    vec3 Pos;
    vec3 Intensity;
};
uniform PointLight u_Light;
uniform mat4 u_Model;
uniform mat4 u_View;
uniform mat4 u_Projection;
uniform mat4 u_Trans;


layout(location = 0) out vec4 o_Color;
layout(location = 1) out vec2 o_TexCoord;
layout(location = 2) out float o_TexIndex;
layout(location = 3) out vec3 o_Normal;
layout(location = 4) out vec3 o_ShadingPoint;
layout(location = 5) out vec3 o_LightPos;
layout(location = 6) out int o_EntityId;

void main()
{
    o_Color = vec4(1., 1., 1., 1.);
    o_TexCoord = a_TexCoord;
    o_TexIndex = a_TexIndex;
    o_Normal = (u_View * vec4(a_Normal, 1.0)).xyz;
    o_ShadingPoint = (u_View * vec4(a_Position, 1.0)).xyz;
    o_LightPos = (u_View * u_Trans * vec4(u_Light.Pos, 1.)).xyz;
    o_EntityId = a_EntityId;
    gl_Position =  u_Projection * u_View * u_Model * vec4(a_Position, 1.0);
}


#type fragment
#version 450 core

struct PointLight {
    vec3 Ka;
    vec3 Kd;
    vec3 Ks;
    vec3 Pos;
    vec3 Intensity;
};
uniform PointLight u_Light;
uniform sampler2D u_Textures[32];
uniform vec3 u_EyePos;

layout(location = 0) in vec4 v_Color;
layout(location = 1) in vec2 v_TexCoord;
layout(location = 2) flat in float v_TexIndex;  // use flat to avoid interpolation
layout(location = 3) in vec3 v_Normal;
layout(location = 4) in vec3 v_ShadingPoint;
layout(location = 5) in vec3 v_LightPos;
layout(location = 6) flat in int v_EntityId;

// Multi Render Targets
layout(location = 0) out vec4 o_Color;
layout(location = 1) out int o_EntityId;

void main()
{
    vec3 l = normalize(v_LightPos - v_ShadingPoint);

    vec3 v = normalize(u_EyePos - v_ShadingPoint);

    // TODO r
    vec3 n = normalize(v_Normal);

    vec4 LightDiffuse = vec4(u_Light.Kd * u_Light.Intensity * max(0., dot(n, l)), 1.);

    // TODO r
    vec3 h = normalize(v+l);
    vec4 LightSpecular = vec4(u_Light.Ks * u_Light.Intensity * pow(max(0., dot(n, h)), 40), 1.);

    vec4 texColor;
    switch(int(v_TexIndex)) {  // dynamic flow control(causes thread divergence)
        case -1: texColor = v_Color;                             break;
        case  0: texColor = texture(u_Textures[ 0], v_TexCoord); break;
        case  1: texColor = texture(u_Textures[ 1], v_TexCoord); break;
        case  2: texColor = texture(u_Textures[ 2], v_TexCoord); break;
        case  3: texColor = texture(u_Textures[ 3], v_TexCoord); break;
        case  4: texColor = texture(u_Textures[ 4], v_TexCoord); break;
        case  5: texColor = texture(u_Textures[ 5], v_TexCoord); break;
        case  6: texColor = texture(u_Textures[ 6], v_TexCoord); break;
        case  7: texColor = texture(u_Textures[ 7], v_TexCoord); break;
        case  8: texColor = texture(u_Textures[ 8], v_TexCoord); break;
        case  9: texColor = texture(u_Textures[ 9], v_TexCoord); break;
        case 10: texColor = texture(u_Textures[10], v_TexCoord); break;
        case 11: texColor = texture(u_Textures[11], v_TexCoord); break;
        case 12: texColor = texture(u_Textures[12], v_TexCoord); break;
        case 13: texColor = texture(u_Textures[13], v_TexCoord); break;
        case 14: texColor = texture(u_Textures[14], v_TexCoord); break;
        case 15: texColor = texture(u_Textures[15], v_TexCoord); break;
        case 16: texColor = texture(u_Textures[16], v_TexCoord); break;
        case 17: texColor = texture(u_Textures[17], v_TexCoord); break;
        case 18: texColor = texture(u_Textures[18], v_TexCoord); break;
        case 19: texColor = texture(u_Textures[19], v_TexCoord); break;
        case 20: texColor = texture(u_Textures[20], v_TexCoord); break;
        case 21: texColor = texture(u_Textures[21], v_TexCoord); break;
        case 22: texColor = texture(u_Textures[22], v_TexCoord); break;
        case 23: texColor = texture(u_Textures[23], v_TexCoord); break;
        case 24: texColor = texture(u_Textures[24], v_TexCoord); break;
        case 25: texColor = texture(u_Textures[25], v_TexCoord); break;
        case 26: texColor = texture(u_Textures[26], v_TexCoord); break;
        case 27: texColor = texture(u_Textures[27], v_TexCoord); break;
        case 28: texColor = texture(u_Textures[28], v_TexCoord); break;
        case 29: texColor = texture(u_Textures[29], v_TexCoord); break;
        case 30: texColor = texture(u_Textures[30], v_TexCoord); break;
        case 31: texColor = texture(u_Textures[31], v_TexCoord); break;
    }
    vec3 AmbientIntensity = vec3(1, 1, 1);
    vec4 LightAmbient = vec4(u_Light.Ka * AmbientIntensity, 1.);
    o_Color = texColor + LightDiffuse + LightAmbient + LightSpecular;

    o_EntityId = v_EntityId;
}