
// Global variables from the CPU
uniform mat4 u_model;
uniform mat4 u_viewprojection;

// Variables to pass to the fragment shader
varying vec2 v_uv;
varying vec3 v_world_position;
varying vec3 v_world_normal;
varying vec3 Ip;

//here create uniforms for all the data we need here
uniform vec3 u_Ia;
uniform vec3 u_Id;
uniform vec3 u_Is;

uniform vec3 u_lightpos;

uniform float u_shine;
uniform vec3 u_Ka;
uniform vec3 u_Kd;
uniform vec3 u_Ks;

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
	//vec3 R = 
	//vec3 Ip = u_Ka * u_Ia + u_Kd *(dot(v_world_position,v_world_normal)) * u_Id + u_Ks*(dot())

	// Project the vertex using the model view projection matrix
	gl_Position = u_viewprojection * vec4(world_position, 1.0); //output of the vertex shader
}