/////////////////////////////////////////////////////////////////////////////
/// Name:        cylinder.geom
/// Purpose:     geometry shader for cylinder imposters
/// Author:      Ahmed Hamdi Boujelben <ahmed.hamdi.boujelben@gmail.com>
///              Parts of this code are based on PyMOL Open source project
/// Created:     2018
/// Copyright:   (c) 2017 Ahmed Hamdi Boujelben
/// Licence:     MIT
/////////////////////////////////////////////////////////////////////////////

#version 330 core
#extension GL_EXT_geometry_shader4 : enable

layout(points) in;
layout(triangle_strip, max_vertices=6) out;

uniform mat4 view;
uniform mat4 projection;
uniform mat3 normalMatrix;
uniform vec3 lightPos;
uniform vec4 eyePoint;

in vec3 cylinder_color_in[];
in vec3 cylinder_direction_in[];
in float cylinder_radius_in[];
in float cylinder_ext_in[];

flat out vec3 cylinder_color;
flat out vec3 lightDir;

out vec3 packed_data_0 ;
out vec3 packed_data_1 ;
out vec3 packed_data_2 ;
out vec4 packed_data_3 ;
out vec3 packed_data_4 ;
out vec3 packed_data_5 ;

#define point ( packed_data_0 )
#define axis ( packed_data_1 )
#define base ( packed_data_2 )
#define end ( packed_data_3.xyz )
#define U ( packed_data_4.xyz )
#define V ( packed_data_5.xyz )
#define cylinder_radius (packed_data_3.w)

void main()
{
  vec3 center = gl_in[0].gl_Position.xyz;
  vec3  dir = cylinder_direction_in[0];
  float ext = cylinder_ext_in[0];
  vec3 ldir;

  cylinder_color  = cylinder_color_in[0];
  cylinder_radius = cylinder_radius_in[0];
  lightDir = normalize(lightPos);

  vec3 cam_dir = normalize(eyePoint.xyz - center);
  float b = dot(cam_dir, dir);
  if(b<0.0) // direction vector looks away, so flip
    ldir = -ext*dir;
  else // direction vector already looks in my direction
    ldir = ext*dir;

  vec3 left = cross(cam_dir, ldir);
  vec3 up = cross(left, ldir);
  left = cylinder_radius*normalize(left);
  up = cylinder_radius*normalize(up);

  // transform to modelview coordinates
  axis =  normalize(normalMatrix * ldir);
  U = normalize(normalMatrix * up);
  V = normalize(normalMatrix * left);

  vec4 base4 = view * vec4(center-ldir, 1.0);
  base = base4.xyz / base4.w;

  vec4 top_position = view*(vec4(center+ldir,1.0));
  vec4 end4 = top_position;
  end = end4.xyz / end4.w;

  vec4 xf0 = view*vec4(center-ldir+left-up,1.0);
  vec4 xf2 = view*vec4(center-ldir-left-up,1.0);
  vec4 xc0 = view*vec4(center+ldir+left-up,1.0);
  vec4 xc1 = view*vec4(center+ldir+left+up,1.0);
  vec4 xc2 = view*vec4(center+ldir-left-up,1.0);
  vec4 xc3 = view*vec4(center+ldir-left+up,1.0);

  vec4 w0 = xf0;
  vec4 w1 = xf2;
  vec4 w2 = xc0;
  vec4 w3 = xc2;
  vec4 w4 = xc1;
  vec4 w5 = xc3;

  // Vertex 1
  point = w0.xyz / w0.w;
  gl_Position = projection  * w0;
  EmitVertex();

  // Vertex 2
  point = w1.xyz / w1.w;
  gl_Position = projection  * w1;
  EmitVertex();

  // Vertex 3
  point = w2.xyz / w2.w;
  gl_Position = projection  * w2;
  EmitVertex();

  // Vertex 4
  point = w3.xyz / w3.w;
  gl_Position = projection  * w3;
  EmitVertex();

  // Vertex 5
  point = w4.xyz / w4.w;
  gl_Position = projection  * w4;
  EmitVertex();

  // Vertex 6
  point = w5.xyz / w5.w;
  gl_Position = projection  * w5;
  EmitVertex();

  EndPrimitive();
}
