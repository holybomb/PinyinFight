#ifdef GL_ES
precision mediump float;
#endif
varying vec2 v_texCoord;
uniform sampler2D u_texture;
uniform float u_time;
uniform float u_random;
uniform float u_waveWidth;
uniform float u_freq;
void main()
{
	float t= v_texCoord.x + (sin (v_texCoord.y * u_waveWidth+u_time) * u_freq);
	gl_FragColor = 1.0 * texture2D (u_texture, vec2 (t, v_texCoord.y));
}