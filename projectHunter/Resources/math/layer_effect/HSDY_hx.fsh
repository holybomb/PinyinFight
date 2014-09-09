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
	float y= v_texCoord.y + v_texCoord.y * (sin (u_waveWidth+u_time) * 0.005);
	float x= v_texCoord.x + (v_texCoord.x - 0.25)* (sin (u_waveWidth+u_time) * 0.005);
	
	if(v_texCoord.x < 0.25)
		x= v_texCoord.x - (0.25 - v_texCoord.x) *(sin (u_waveWidth+u_time) * 0.005);
	
	
	
	gl_FragColor = 1.0 * texture2D (u_texture, vec2 (x, y));
}