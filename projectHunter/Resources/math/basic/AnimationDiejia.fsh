#ifdef GL_ES 
precision mediump float; 
#endif 
uniform sampler2D u_texture; 
varying vec2 v_texCoord; 
varying vec4 v_fragmentColor; 

uniform vec3 inPut;
uniform vec2 outPut;
uniform vec4 color;

void main(void) 
{
	vec4 rgbOrigin = texture2D(u_texture, v_texCoord);
	float r = rgbOrigin.r * (1.0 - color.a) + color.r * color.a;
	float g = rgbOrigin.g * (1.0 - color.a) + color.g * color.a;
	float b = rgbOrigin.b * (1.0 - color.a) + color.b * color.a;
	gl_FragColor = vec4(r, g, b, rgbOrigin.a); 
}