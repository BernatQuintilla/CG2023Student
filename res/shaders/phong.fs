// This variables comes from the vertex shader
// They are baricentric interpolated by pixel according to the distance to every vertex
varying vec3 v_world_normal;
varying vec3 v_world_position;
varying vec2 v_uv;

// Uniform variables
uniform mat4 u_model;
uniform mat4 u_viewprojection;

uniform vec3 u_Ia;
uniform vec3 u_Id;
uniform vec3 u_Is;

uniform vec3 u_lightpos;

uniform float u_shininess;
uniform vec3 u_Ka;
uniform vec3 u_Kd;
uniform vec3 u_Ks;
uniform vec3 u_eyepos;

uniform sampler2D u_texture;
uniform sampler2D u_normal_texture;
uniform float u_flag;

void main()
{
	if(u_flag == 1.0){
		vec4 Kd = texture2D(u_texture,v_uv);
		vec4 Ka = vec4(0.02,0.02,0.02,1.0);
		vec4 Ks = vec4(texture2D(u_texture,v_uv).w);

		vec3 N = texture2D(u_normal_texture,v_uv).xyz;
		N = (u_model * vec4(N, 0.0)).xyz;
		N = N*2 - vec3(1.0,1.0,1.0);
		vec3 L = normalize(u_lightpos - v_world_position);
		vec3 V = normalize(u_eyepos - v_world_position);
		vec3 R = normalize(reflect(-L,N));

		vec3 Ip = u_Ia * Ka + Kd* u_Id * clamp(dot(L, N),0.0,1.0) + Ks * u_Is * pow(clamp(dot(R, V),0.0,1.0), u_shininess);
		gl_FragColor = vec4(Ip, 1.0);

	}if(u_flag == 0.0){
		vec3 N = normalize(v_world_normal.xyz);
		vec3 L = normalize(u_lightpos - v_world_position);
		vec3 V = normalize(u_eyepos - v_world_position);
		vec3 R = normalize(reflect(-L,N));
		vec3 Ip = u_Ia * u_Ka + u_Kd* u_Id * clamp(dot(L, N),0.0,1.0) + u_Ks * u_Is * pow(clamp(dot(R, V),0.0,1.0), u_shininess);
		gl_FragColor = vec4(Ip,1.0);
	}
}
