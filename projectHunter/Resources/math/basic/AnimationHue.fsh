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
	
	
	vec3 rgbOrigin = texture2D(u_texture, v_texCoord);
	
	//rgb2hsv
	vec4 K = vec4(0.0, -1.0 / 3.0, 2.0 / 3.0, -1.0);
    vec4 p = mix(vec4(rgbOrigin.bg, K.wz), vec4(rgbOrigin.gb, K.xy), step(rgbOrigin.b, rgbOrigin.g));
    vec4 q = mix(vec4(p.xyw, rgbOrigin.r), vec4(rgbOrigin.r, p.yzx), step(p.x, rgbOrigin.r));

    float d = q.x - min(q.w, q.y);
    float e = 1.0e-10;
	
	vec3 hsv = vec3(abs(q.z + (q.w - q.y) / (6.0 * d + e)), d / (q.x + e), q.x);
	
	//	http://gamedev.stackexchange.com/questions/59797/glsl-shader-change-hue-saturation-brightness
	hsv.x -= 0.05;// in photoShop this is in SeXiang none ZhuoSe value / 360
	
	hsv.x = mod(hsv.x, 1.0);
    hsv.y = mod(hsv.y, 1.0);
    hsv.z = mod(hsv.z, 1.0);
	
	vec4 K1 = vec4(1.0, 2.0 / 3.0, 1.0 / 3.0, 3.0);
    vec3 p1 = abs(fract(hsv.xxx + K1.xyz) * 6.0 - K1.www);
	
	vec3 rgbFinal = hsv.z * mix(K1.xxx, clamp(p1 - K1.xxx, 0.0, 1.0), hsv.y);
	gl_FragColor = vec4(rgbFinal.r, rgbFinal.g, rgbFinal.b, alpha); 
}