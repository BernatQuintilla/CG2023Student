// This variables comes from the vertex shader
// They are baricentric interpolated by pixel according to the distance to every vertex
varying vec3 v_world_normal;
varying vec3 v_world_position;
varying vec2 v_uv;

uniform sampler2D u_texture;

void main()
{
	// Set the ouput color per pixel
	gl_FragColor = texture2D(u_texture, v_uv); 
}
