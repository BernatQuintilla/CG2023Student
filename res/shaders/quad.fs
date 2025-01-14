varying vec2 v_uv;
uniform float u_time;
uniform float u_task;
uniform sampler2D u_texture;
uniform sampler2D u_texture1;
uniform float u_pixelate;
uniform float u_direction;

void main()
{
	vec3 red = vec3(1,0,0);
	vec3 blue = vec3(0,0,1);
	vec3 white = vec3(0,0,0);
	vec3 black = vec3(1,1,1);
	float x = v_uv.x;
	float y = v_uv.y;
	float dx = distance(x,0.5);
	float dy = distance(y,0.5);
	float d = sqrt(dx*dx + dy*dy);
	if(u_task ==1){ //1a
		vec3 final_color = mix(red,blue,1-x);
		gl_FragColor = vec4(final_color,1.0);

	}
	if(u_task == 2){ //1b
		vec3 final_color = mix(white,black,d);
		gl_FragColor = vec4(final_color,1.0);
	}
	if(u_task == 3){ //1c 
		gl_FragColor = vec4(step(5, mod(x * 34, 6.5)), 0.0, step(5, mod(y * 34, 6.5)), 1.0);
	}
	if(u_task == 4){ //1d
		gl_FragColor = vec4(floor(x * 10.0)/10, floor(y * 10.0)/10, 0, 1);
	}
	if(u_task == 5){ //1e
		float s = step(0.25 * sin(2 * 3.14 * x) + 0.5,y);
		vec3 final_color = mix(mix(vec3(0.0,0.0,0.0),vec3(0.0,1.0,0.0),y),mix(vec3(0.0,1.0,0.0),vec3(0.0,0.0,0.0),y),s);
		gl_FragColor = vec4(final_color,1.0);
	}
	if(u_task == 6){ //1f
		int row = int(y*20);
		int col = int(x*20);
		float color = mix(1.0, 0.0, step(1.0, mod(float(row + col), 2.0)));
		vec3 final_color = vec3(color,color,color);
		gl_FragColor = vec4(final_color,1.0);
	}
	if(u_task == 7){ //2 Original Image
		vec4 color = texture2D(u_texture,v_uv);
		gl_FragColor = color;
	}
	if(u_task == 8){ //2a
		vec4 color = texture2D(u_texture,v_uv);
		float lum = (color.x+color.y+color.z)/3;
		gl_FragColor = vec4(lum,lum,lum,1.0);
	}
	if(u_task == 9){ //2b
		vec4 color = texture2D(u_texture,v_uv);
		vec4 invColor = vec4(1.0-color.x,1.0-color.y,1.0-color.z,1.0);
		gl_FragColor = invColor;
	}
	if(u_task == 10){ //2c
		vec4 color = texture2D(u_texture,v_uv);
		vec4 intColor = vec4(1.4-color.y,color.x,color.z*0.4,1.0);
		gl_FragColor = intColor;
	}
	if(u_task == 11){ //2d
		vec4 color = texture2D(u_texture,v_uv);
		float m = step(0.46, (color.x + color.y + color.z)/3);
		gl_FragColor = vec4(m,m,m,1.0);
	}
	if(u_task == 12){ //2e
		float v = 0.0025;
		vec4 color1 = texture2D(u_texture,v_uv);
		vec4 color2 = texture2D(u_texture,vec2(v_uv.x-v,v_uv.y-v));
		vec4 color3 = texture2D(u_texture,vec2(v_uv.x,v_uv.y-v));
		vec4 color4 = texture2D(u_texture,vec2(v_uv.x-v,v_uv.y));
		vec4 color5 = texture2D(u_texture,vec2(v_uv.x+v,v_uv.y-v));
		vec4 color6 = texture2D(u_texture,vec2(v_uv.x+v,v_uv.y));
		vec4 color7 = texture2D(u_texture,vec2(v_uv.x+v,v_uv.y+v));
		vec4 color8 = texture2D(u_texture,vec2(v_uv.x,v_uv.y+v));
		vec4 color9 = texture2D(u_texture,vec2(v_uv.x-v,v_uv.y+v));
		float r = (color1.x+color2.x+color3.x+color4.x+color5.x+color6.x+color7.x+color8.x+color9.x)/9;
		float g = (color1.y+color2.y+color3.y+color4.y+color5.y+color6.y+color7.y+color8.y+color9.y)/9;
		float b = (color1.z+color2.z+color3.z+color4.z+color5.z+color6.z+color7.z+color8.z+color9.z)/9;
		vec4 final_color = vec4(r,g,b,1.0);
		gl_FragColor = final_color;
	}
	if(u_task == 13){ //2f
		float d = length(v_uv - vec2(0.5));
		float vignette = smoothstep(0.65, 0.1, d);
		vec4 color = texture2D(u_texture, v_uv);
		gl_FragColor = vec4(color.x*vignette,color.y*vignette,color.z*vignette,1.0);
	}
	if(u_task == 14){ //2 extra: brightness
		vec4 color = texture2D(u_texture,v_uv);
		float s = clamp(sin(u_time),0,0.5);
		gl_FragColor = vec4(color.x+s,color.y+s,color.z+s,1.0);
	}
	if(u_task == 15){ //3 pixelization
		vec2 texel = vec2(u_pixelate); 
		vec2 coord = floor(v_uv / texel) * texel; 
		vec4 color = texture2D(u_texture1, coord); 
		gl_FragColor = color;
	}
	if(u_task == 16){ //3 rotation
		float angle = u_time*0.2*u_direction;
		vec2 center = vec2(0.5,0.5);
		mat2 mrot = mat2(cos(angle),sin(angle),-sin(angle),cos(angle));
		vec2 coord = (v_uv - center) * mrot + center;
		vec4 color = texture2D(u_texture1, coord); 
		gl_FragColor = color;
	}
	if(u_task == 17){ //3 extra: Displacement
		float cx = x + clamp(sin(u_time),0,2);
		float cy = y + clamp(cos(u_time),0,2);
		//v_uv.x = cx;
		//v_uv.y = cy;
		gl_FragColor =  texture2D(u_texture,vec2(cx,cy));
	}
}
