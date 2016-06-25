#version 120

varying vec2 v_texcoord;
uniform sampler2D tex;

void main(void) {
	gl_FragColor = 2 * texture2D(tex, v_texcoord);
}
