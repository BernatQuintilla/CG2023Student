// This variables comes from the vertex shader
// They are baricentric interpolated by pixel according to the distance to every vertex
varying vec3 v_world_normal;
varying vec3 v_world_position;
varying vec2 v_uv;
varying vec3 v_finalcolor;
uniform sampler2D u_texture;
void main()
{
	// Set the ouput color per pixel
	gl_FragColor = vec4(v_finalcolor,1.0);
}
