/////////////////////////////////////////////////////////////////////////////
/// Name:        cylinder.frag
/// Purpose:     fragment shader for cylinder imposters
/// Author:      Ahmed Hamdi Boujelben <ahmed.hamdi.boujelben@gmail.com>
///              Parts of this code are based on PyMOL Open source project
/// Created:     2017
/// Copyright:   (c) 2018-2021 Ahmed Hamdi Boujelben
/// Licence:     MIT
/////////////////////////////////////////////////////////////////////////////

#version 330 core

flat in vec3 cylinder_color;
flat in vec3 lightDir;

in vec3 packed_data_0 ;
in vec3 packed_data_1 ;
in vec3 packed_data_2 ;
in vec4 packed_data_3 ;
in vec3 packed_data_4 ;
in vec3 packed_data_5 ;

#define surface_point ( packed_data_0 )
#define axis ( packed_data_1 )
#define base ( packed_data_2 )
// end -> end_cyl
#define end_cyl packed_data_3.xyz
#define U ( packed_data_4 )
#define V ( packed_data_5 )
#define radius ( packed_data_3.w )

uniform mat4 projection;
uniform mat4 view;
uniform vec3 lightColor;

out vec4 out_Color;

vec4 ComputeColorForLight(vec3 N, vec3 L, vec4 ambient, vec4 diffuse, vec4 color){
  float NdotL;
  vec4 ret_val = vec4(0.);
  ret_val += ambient * color;
  NdotL = dot(N, L);
  if (NdotL > 0.0) {
    ret_val += diffuse * NdotL * color;
  }

  // specular
  float specularStrength = 1.0;
  vec3 reflectDir = reflect(-L, N);
  vec3 viewDir = normalize(vec3(-view*vec4(packed_data_0,1)));
  float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
  vec3 specular = specularStrength * spec * lightColor;
  ret_val += vec4(specular,1.0) * color;

  return ret_val;
}

void main()
{
    vec3 objectColor=cylinder_color;

    const float ortho=0;

    vec4 color = vec4(objectColor,1.0);
    vec3 ray_target = surface_point;
    vec3 ray_origin = vec3(0.0);
    vec3 ray_direction = mix(normalize(ray_origin - ray_target), vec3(0.0, 0.0, 1.0), ortho);
    mat3 basis = mat3(U, V, axis);

    vec3 diff = ray_target - 0.5 * (base + end_cyl);
    vec3 P = diff * basis;

    // angle (cos) between cylinder cylinder_axis and ray direction
    float dz = dot(axis, ray_direction);

    float radius2 = radius*radius;

    // calculate distance to the cylinder from ray origin
    vec3 D = vec3(dot(U, ray_direction),
                dot(V, ray_direction),
                dz);
    float a0 = P.x*P.x + P.y*P.y - radius2;
    float a1 = P.x*D.x + P.y*D.y;
    float a2 = D.x*D.x + D.y*D.y;
    // calculate a dicriminant of the above quadratic equation
    float d = a1*a1 - a0*a2;
    if (d < 0.0)
    // outside of the cylinder
    discard;

    float dist = (-a1 + sqrt(d))/a2;

    // point of intersection on cylinder surface
    vec3 new_point = ray_target + dist * ray_direction;

    vec3 tmp_point = new_point - base;
    vec3 normal = normalize(tmp_point - axis * dot(tmp_point, axis));

    ray_origin = mix(ray_origin, surface_point, ortho);


    // test front cap
    float cap_test = dot((new_point - base), axis);


    // to calculate caps, simply check the angle between
    // the point of intersection - cylinder end vector
    // and a cap plane normal (which is the cylinder cylinder_axis)
    // if the angle < 0, the point is outside of cylinder
    // test front cap

    // flat
    if (cap_test < 0.0)
    {
    // ray-plane intersection
    float dNV = dot(-axis, ray_direction);
    if (dNV < 0.0)
      discard;
    float near = dot(-axis, (base)) / dNV;
    new_point = ray_direction * near + ray_origin;
    // within the cap radius?
    if (dot(new_point - base, new_point-base) > radius2)
      discard;
    normal = -axis;
    }

    // test end cap

    cap_test = dot((new_point - end_cyl), axis);

    // flat
    if (cap_test > 0.0)
    {
    // ray-plane intersection
    float dNV = dot(axis, ray_direction);
    if (dNV < 0.0)
      discard;
    float near = dot(axis, end_cyl) / dNV;
    new_point = ray_direction * near + ray_origin;
    // within the cap radius?
    if (dot(new_point - end_cyl, new_point-base) > radius2)
      discard;
    normal = axis;
    }

    vec2 clipZW = new_point.z * projection[2].zw + projection[3].zw;
    float depth = 0.5 + 0.5 * clipZW.x / clipZW.y;

    // this is a workaround necessary for Mac
    // otherwise the modified fragment won't clip properly

    if (depth <= 0.0)
    discard;

    if (depth >= 1.0)
    discard;

    gl_FragDepth = depth;


    vec4 final_color = 0.01 * color;
    final_color += ComputeColorForLight(normal, lightDir,
                                      vec4(0.3,0.3,0.3,1.0), // ambient
                                      vec4(1.0,1.0,1.0,1.0), // diffuse
                                      color);

    out_Color = final_color;
}
