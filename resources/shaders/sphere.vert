/////////////////////////////////////////////////////////////////////////////
/// Name:        sphere.vert
/// Purpose:     vertex shader for sphere imposters
/// Author:      Ahmed Hamdi Boujelben <ahmed.hamdi.boujelben@gmail.com>
/// Created:     2017
/// Copyright:   (c) 2018 Ahmed Hamdi Boujelben
/// Licence:     Attribution-NonCommercial 4.0 International
/////////////////////////////////////////////////////////////////////////////

#version 330 core

layout(location = 0) in vec3 spherePos;
layout(location = 1) in float sphereRadius;
layout(location = 2) in vec3  sphereColor;

uniform mat4 view;

out VertexData
{
    vec3 cameraSpherePos;
    float sphereRadius;
    vec3 sphereColor;
} outData;

void main()
{
    outData.cameraSpherePos = vec3(view*vec4(spherePos,1));
    outData.sphereRadius = sphereRadius;
    outData.sphereColor=sphereColor;
}
