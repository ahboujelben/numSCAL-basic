/////////////////////////////////////////////////////////////////////////////
/// Name:        sphere.vert
/// Purpose:     vertex shader for drawing lines
/// Author:      Ahmed Hamdi Boujelben <ahmed.hamdi.boujelben@gmail.com>
/// Created:     2017
/// Copyright:   (c) 2017 Ahmed Hamdi Boujelben
/// Licence:     Attribution-NonCommercial 4.0 International
/////////////////////////////////////////////////////////////////////////////

#version 330 core

layout(location = 0) in vec3 pos;
layout(location = 1) in vec3 color;

uniform mat4 view;
uniform mat4 projection;

flat out vec3 o_color;

void main()
{
	o_color=color;
    gl_Position = projection  * view * vec4(pos, 1.0);
}
