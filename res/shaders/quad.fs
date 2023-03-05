varying vec2 v_uv;
uniform float u_time;
uniform float u_task;
uniform sampler2D u_texture;

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
}
