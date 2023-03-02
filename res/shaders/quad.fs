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
	float dx = distance(x,0.5);
	float dy = distance(y,0.5);
	float d = sqrt(dx*dx + dy*dy);
	gl_FragColor = vec4(final_color,1.0);
	if(u_task == 2){
		final_color = mix(white,black,d);
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
		vec3 color = mix(vec3(0.0),red,mixAmounty) + mix(vec3(0.0),blue,mixAmountx);
		gl_FragColor = vec4(color,1.0);
	}

}
