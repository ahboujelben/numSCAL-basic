/////////////////////////////////////////////////////////////////////////////
/// Name:        sphere.geom
/// Purpose:     geometry shader for sphere imposters
/// Author:      Ahmed Hamdi Boujelben <ahmed.hamdi.boujelben@gmail.com>
/// Created:     2017
/// Copyright:   (c) 2017 Ahmed Hamdi Boujelben
/// Licence:     Attribution-NonCommercial 4.0 International
/////////////////////////////////////////////////////////////////////////////

#version 330
#extension GL_EXT_gpu_shader4 : enable

layout(points) in;
layout(triangle_strip, max_vertices=4) out;

uniform mat4 projection;

in VertexData
{
    vec3 cameraSpherePos;
    float sphereRadius;
    vec3 sphereColor;
} vert[];

out FragData
{
    flat vec3 cameraSpherePos;
    flat float sphereRadius;
    flat vec3 sphereColor;
    smooth vec2 mapping;
};

const float g_boxCorrection = 1.5;

void main()
{
    sphereRadius=vert[0].sphereRadius;
    sphereColor=vert[0].sphereColor;
    cameraSpherePos = vec3(vert[0].cameraSpherePos);
    vec4 cameraCornerPos;

    //Top-left
    mapping = vec2(-1.0, 1.0) * g_boxCorrection;
    cameraCornerPos = vec4(cameraSpherePos, 1.0);
    cameraCornerPos.xy += vec2(-vert[0].sphereRadius, vert[0].sphereRadius) * g_boxCorrection;
    gl_Position = projection * cameraCornerPos;
    gl_PrimitiveID = gl_PrimitiveIDIn;
    EmitVertex();

    //Top-Right
    mapping = vec2(1.0, 1.0) * g_boxCorrection;
    cameraCornerPos = vec4(cameraSpherePos, 1.0);
    cameraCornerPos.xy += vec2(vert[0].sphereRadius, vert[0].sphereRadius) * g_boxCorrection;
    gl_Position = projection * cameraCornerPos;
    gl_PrimitiveID = gl_PrimitiveIDIn;
    EmitVertex();

    //Bottom-left
    mapping = vec2(-1.0, -1.0) * g_boxCorrection;
    cameraCornerPos = vec4(cameraSpherePos, 1.0);
    cameraCornerPos.xy += vec2(-vert[0].sphereRadius, -vert[0].sphereRadius) * g_boxCorrection;
    gl_Position = projection * cameraCornerPos;
    gl_PrimitiveID = gl_PrimitiveIDIn;
    EmitVertex();

    //Bottom-right
    mapping = vec2(1.0, -1.0) * g_boxCorrection;
    cameraCornerPos = vec4(cameraSpherePos, 1.0);
    cameraCornerPos.xy += vec2(vert[0].sphereRadius, -vert[0].sphereRadius) * g_boxCorrection;
    gl_Position = projection * cameraCornerPos;
    gl_PrimitiveID = gl_PrimitiveIDIn;
    EmitVertex();
}
