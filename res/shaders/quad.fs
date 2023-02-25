varying vec2 v_uv;
uniform float u_time;
uniform int u_task;

void main()
{
	vec3 red = vec3(1,0,0);
	vec3 blue = vec3(0,0,1);
	vec3 white = vec3(0,0,0);
	vec3 black = vec3(1,1,1);
	float x = v_uv.x;
	float y = v_uv.y;
	vec3 final_color = mix(red,blue,1-x);
	float dx = x - 0.5;
	float dy = y - 0.5;
	float d = sqrt(dx*dx + dy*dy);
	final_color = mix(white,black,d);
	if(u_task == 1){
		float dx = x - 0.5;
		float dy = y - 0.5;
		float d = sqrt(dx*dx + dy*dy);
		final_color = mix(white,black,d);
	}
	
	gl_FragColor = vec4(final_color,1.0);
}
