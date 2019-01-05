/////////////////////////////////////////////////////////////////////////////
/// Name:        sphere.vert
/// Purpose:     vertex shader for drawing lines
/// Author:      Ahmed Hamdi Boujelben <ahmed.hamdi.boujelben@gmail.com>
/// Created:     2017
/// Copyright:   (c) 2018 Ahmed Hamdi Boujelben
/// Licence:     Attribution-NonCommercial 4.0 International
/////////////////////////////////////////////////////////////////////////////

#version 330 core

layout(location = 0) in vec3 pos;
layout(location = 1) in vec2 color;

uniform mat4 view;
uniform mat4 projection;
uniform vec3 oilColor;
uniform vec3 waterColor;
uniform vec3 tracerColor;

const float epsilon = 0.01;

flat out vec3 o_color;

void main()
{
    o_color = abs(color.x) < epsilon ? oilColor + (tracerColor - oilColor) * color.y
                                     : waterColor+ (tracerColor - waterColor) * color.y;;
    gl_Position = projection  * view * vec4(pos, 1.0);
}
