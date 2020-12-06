/////////////////////////////////////////////////////////////////////////////
/// Name:        sphere.frag
/// Purpose:     fragment shader for drawing lines
/// Author:      Ahmed Hamdi Boujelben <ahmed.hamdi.boujelben@gmail.com>
/// Created:     2017
/// Copyright:   (c) 2018-2021 Ahmed Hamdi Boujelben
/// Licence:     MIT
/////////////////////////////////////////////////////////////////////////////

#version 330 core

uniform vec3 oilColor;
uniform vec3 waterColor;
uniform vec3 tracerColor;

out vec4 FragColor;

flat in vec3 o_color;

void main()
{
    FragColor = vec4(o_color, 1.0);
}
