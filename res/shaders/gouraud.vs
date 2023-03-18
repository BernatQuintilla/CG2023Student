
// Global variables from the CPU
uniform mat4 u_model;
uniform mat4 u_viewprojection;

// Variables to pass to the fragment shader
varying vec2 v_uv;
varying vec3 v_world_position;
varying vec3 v_world_normal;
varying vec3 v_finalcolor;

//here create uniforms for all the data we need here
uniform vec3 u_Ia;
uniform vec3 u_Id;
uniform vec3 u_Is;

uniform vec3 u_lightpos;

uniform float u_shininess;
uniform vec3 u_Ka;
uniform vec3 u_Kd;
uniform vec3 u_Ks;
uniform sampler2D u_texture;
uniform vec3 u_eyepos;

void main()
{	
	v_uv = gl_MultiTexCoord0.xy;

	// Convert local position to world space
	vec3 world_position = (u_model * vec4( gl_Vertex.xyz, 1.0)).xyz;

	// Convert local normal to world space
	vec3 world_normal = (u_model * vec4( gl_Normal.xyz, 0.0)).xyz;

	// Pass them to the fragment shader interpolated
	v_world_position = world_position;
	v_world_normal = world_normal;

	// Compute Ip
	vec3 N = normalize(world_normal);
	vec3 L = normalize(u_lightpos - world_position);
	vec3 V = normalize(u_eyepos - world_position);
	vec3 R = normalize(reflect(-L, N));
	
	vec3 Ip = u_Ia * u_Ka + u_Kd* u_Id * clamp(dot(L, N),0.0,1.0) + u_Ks * u_Is * pow(clamp(dot(R, V),0.0,1.0), u_shininess);
	v_finalcolor = Ip;

	// Project the vertex using the model view projection matrix
	gl_Position = u_viewprojection * vec4(world_position, 1.0); //output of the vertex shader
}