#version 120

void main() {
	vec4 rgba = vec4(0.7, 0.8, 1.0, 1.0);

	if(mod(gl_FragCoord.y, 4.0) >= 2.0) rgba *= 0.5;

	gl_FragColor = rgba;
}

