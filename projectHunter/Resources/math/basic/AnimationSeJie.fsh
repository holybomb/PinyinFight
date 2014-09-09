#ifdef GL_ES 
precision mediump float; 
#endif 
uniform sampler2D u_texture; 
varying vec2 v_texCoord; 
varying vec4 v_fragmentColor; 

uniform vec3 inPut;
uniform vec2 outPut;

void main(void) 
{ 
float alpha = texture2D(u_texture, v_texCoord).a; 
float r = texture2D(u_texture, v_texCoord).r ;
float g = texture2D(u_texture, v_texCoord).g ;
float b = texture2D(u_texture, v_texCoord).b ;

float min_in = inPut.r;
float max_in = inPut.g;
float inGamma = 1.0/inPut.b;

float min_out = outPut.x;
float max_out = outPut.y;

float cr = r;


r = (pow(((cr * 255.0) - min_in) / (max_in - min_in),
                inGamma) * (max_out - min_out) + min_out) / 255.0;
cr = g;
g = (pow(((cr * 255.0) - min_in) / (max_in - min_in),
                inGamma) * (max_out - min_out) + min_out) / 255.0;				
cr = b;
b = (pow(((cr * 255.0) - min_in) / (max_in - min_in),
                inGamma) * (max_out - min_out) + min_out) / 255.0;

gl_FragColor = vec4(r, g, b, alpha); 
}