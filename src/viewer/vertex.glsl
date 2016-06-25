#version 120

attribute vec2 coord2d;
attribute vec2 texcoord;
varying vec2 v_texcoord;

void main(void) {
	gl_Position = vec4(coord2d, 0.0, 1.0);
	v_texcoord = texcoord;
}
