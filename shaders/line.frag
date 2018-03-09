/////////////////////////////////////////////////////////////////////////////
/// Name:        sphere.frag
/// Purpose:     fragment shader for drawing lines
/// Author:      Ahmed Hamdi Boujelben <ahmed.hamdi.boujelben@gmail.com>
/// Created:     2017
/// Copyright:   (c) 2018 Ahmed Hamdi Boujelben
/// Licence:     Attribution-NonCommercial 4.0 International
/////////////////////////////////////////////////////////////////////////////

#version 330 core
out vec4 FragColor;

flat in vec3 o_color;

void main()
{
    vec3 objectColor=o_color;
    FragColor = vec4(objectColor, 1.0);
}
