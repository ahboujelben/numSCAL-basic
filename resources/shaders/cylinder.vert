/////////////////////////////////////////////////////////////////////////////
/// Name:        cylinder.vert
/// Purpose:     vertex shader for cylinder imposters
/// Author:      Ahmed Hamdi Boujelben <ahmed.hamdi.boujelben@gmail.com>
///              Parts of this code are based on PyMOL Open source project
/// Created:     2017
/// Copyright:   (c) 2018 Ahmed Hamdi Boujelben
/// Licence:     Attribution-NonCommercial 4.0 International
/////////////////////////////////////////////////////////////////////////////

#version 330 core
#extension GL_EXT_gpu_shader4 : enable

layout(location = 0) in vec3  cylinderPosition;
layout(location = 1) in float cylinderExt;
layout(location = 2) in vec3  cylinderDirection;
layout(location = 3) in float cylinderRadius;
layout(location = 4) in vec2  cylinderColor;

out vec3 cylinder_color_in;
out vec3 cylinder_direction_in;
out float cylinder_radius_in;
out float cylinder_ext_in;

uniform vec3 oilColor;
uniform vec3 waterColor;
uniform vec3 tracerColor;

const float epsilon = 0.01;

void main()
{
  cylinder_ext_in = cylinderExt;
  cylinder_direction_in = normalize(cylinderDirection);
  cylinder_radius_in = cylinderRadius;
  cylinder_color_in = abs(cylinderColor.x) < epsilon  ? oilColor+ (tracerColor - oilColor) * cylinderColor.y
                     :abs(cylinderColor.x - 1) < epsilon ? waterColor + (tracerColor - waterColor) * cylinderColor.y
                     :abs(cylinderColor.x - 2) < epsilon ? vec3(1.0f, 0.0f, 0.0f)
                     :abs(cylinderColor.x - 3) < epsilon ? vec3(0.0f, 1.0f, 0.0f)
                     :vec3(0.2f, 0.2f, 1.0f);

  gl_Position = vec4(cylinderPosition,1.0);
}
