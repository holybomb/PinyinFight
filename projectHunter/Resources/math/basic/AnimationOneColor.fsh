#ifdef GL_ES 
precision mediump float; 
#endif 
uniform sampler2D u_texture; 
varying vec2 v_texCoord; 
varying vec4 v_fragmentColor; 
uniform vec3 color;
void main(void) 
{ 
// Convert to greyscale using NTSC weightings 
float alpha = texture2D(u_texture, v_texCoord).a; 

gl_FragColor = vec4(color.r, color.g, color.b, alpha); // white color only
}