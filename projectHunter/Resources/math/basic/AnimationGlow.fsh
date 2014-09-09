#ifdef GL_ES 
precision mediump float; 
#endif 
uniform sampler2D u_texture; 
varying vec2 v_texCoord; 
varying vec4 v_fragmentColor; 
void main(void) 
{ 
// Convert to greyscale using NTSC weightings 
float alpha = texture2D(u_texture, v_texCoord).a; 
float r = texture2D(u_texture, v_texCoord).r * 1.6;
float g = texture2D(u_texture, v_texCoord).g * 1.6;
float b = texture2D(u_texture, v_texCoord).b ;
gl_FragColor = vec4(r, g, b, alpha); 
}