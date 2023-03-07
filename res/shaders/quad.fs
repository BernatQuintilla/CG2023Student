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
	if(u_task ==1){
		vec3 final_color = mix(red,blue,1-x);
		gl_FragColor = vec4(final_color,1.0);

	}
	if(u_task == 2){
		vec3 final_color = mix(white,black,d);
		gl_FragColor = vec4(final_color,1.0);
	}
	if(u_task == 3){
		int numStripes = 20; // número de rayas
		float stripeWidth = 0.1; // ancho de cada raya
		// calcular la posición del centro de la primera raya
		float firstStripePos = -0.45 + (stripeWidth / 2.0);
		float minDisty = distance(firstStripePos,x);
		float minDistx = distance(firstStripePos,y);
		for (int i = 0; i < numStripes; i++) {
			// calcular la posición del centro de la raya actual
			float stripePos = firstStripePos + (i * stripeWidth);
			// determinar si el fragmento de píxel está dentro de la raya
			float distx = abs(x - stripePos);
			float disty = abs(y - stripePos);
			minDisty = min(minDisty, distx);
			minDistx = min(minDistx, disty);
		}
		float mixAmounty = smoothstep(0.0,stripeWidth, minDisty);
		float mixAmountx = smoothstep(0.0,stripeWidth, minDistx);
		vec3 final_color = mix(vec3(0.0),red,mixAmounty) + mix(vec3(0.0),blue,mixAmountx);
		gl_FragColor = vec4(final_color,1.0);
	}
	if(u_task == 4){
		float mx = mod(x,0.1);
		float my = mod(y,0.1);
		vec3 final_color = mix(vec3(v_uv,0.0),vec3(0.0,0.0,0.0),mx+my);
		gl_FragColor = vec4(final_color,1.0);
	}
	if(u_task == 5){
		float s = step(0.25 * sin(2 * 3.14 * x) + 0.5,y);
		vec3 final_color = mix(mix(vec3(0.0,0.0,0.0),vec3(0.0,1.0,0.0),y),mix(vec3(0.0,1.0,0.0),vec3(0.0,0.0,0.0),y),s);
		gl_FragColor = vec4(final_color,1.0);
	}
	if(u_task == 6){
		int row = int(y*20);
		int col = int(x*20);
		float color = mix(1.0, 0.0, step(1.0, mod(float(row + col), 2.0)));
		vec3 final_color = vec3(color,color,color);
		gl_FragColor = vec4(final_color,1.0);
	}
	if(u_task == 7){
		vec4 color = texture2D(u_texture,v_uv);
		gl_FragColor = color;
	}
	if(u_task == 8){
		vec4 color = texture2D(u_texture,v_uv);
		float lum = (color.x+color.y+color.z)/3;
		gl_FragColor = vec4(lum,lum,lum,1.0);
	}
	if(u_task == 9){
		vec4 color = texture2D(u_texture,v_uv);
		vec4 invColor = vec4(1.0-color.x,1.0-color.y,1.0-color.z,1.0);
		gl_FragColor = invColor;
	}
	if(u_task == 10){
		vec4 color = texture2D(u_texture,v_uv);
		vec4 intColor = vec4(1.0-color.x,color.y,1.0-color.z,1.0);
		gl_FragColor = intColor;
	}
	if(u_task == 11){
		vec4 color = texture2D(u_texture,v_uv);
		float m = step(0.46, (color.x + color.y + color.z)/3);
		gl_FragColor = vec4(m,m,m,1.0);
	}
	if(u_task == 12){
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
	if(u_task == 13){
		float d = length(v_uv - vec2(0.5));
		float vignette = smoothstep(0.65, 0.1, d);
		vec4 color = texture2D(u_texture, v_uv);
		gl_FragColor = vec4(color.x*vignette,color.y*vignette,color.z*vignette,1.0);
	}
	if(u_task == 14){
		vec2 texel = vec2(u_pixelate); 
		vec2 coord = floor(v_uv / texel) * texel; 
		vec4 color = texture2D(u_texture, coord); 
		gl_FragColor = color;
	}
	if(u_task == 15){
		float angle = u_time*0.2*u_direction;
		vec2 center = vec2(0.5,0.5);
		mat2 mrot = mat2(cos(angle),sin(angle),-sin(angle),cos(angle));
		vec2 coord = (v_uv - center) * mrot + center;
		vec4 color = texture2D(u_texture, coord); 
		gl_FragColor = color;
	}
}
