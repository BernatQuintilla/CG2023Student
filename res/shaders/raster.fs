// This variables comes from the vertex shader
// They are baricentric interpolated by pixel according to the distance to every vertex
varying vec3 v_world_normal;
varying vec3 v_world_position;
varying vec2 v_uv;

uniform sampler2D u_texture;
uniform float u_task;

void main()
{
	// Set the ouput color per pixel
	//if(u_task == 16){texture2D(u_texture, v_uv);
		gl_FragColor = texture2D(u_texture, v_uv);
	//}
}
