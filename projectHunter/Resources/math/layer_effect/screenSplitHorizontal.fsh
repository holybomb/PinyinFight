// Shader taken from: http://webglsamples.googlecode.com/hg/electricflower/electricflower.html

#ifdef GL_ES
precision mediump float;
#endif


uniform sampler2D CC_Texture0;

varying vec2 uv;


// some const, tweak for best look
const float sampleDist = 1.0;
//const float sampleStrength = 1.0; 
uniform float sampleStrength;
void main() {


   
    vec2 dir = 0.5 - uv; 
 
    // calculate the distance to the center of the screen
    float dist = sqrt(dir.x*dir.x + dir.y*dir.y); 
 
    // normalize the direction (reuse the distance)
    //dir = dir/dist; 
 
	if(uv.y > 0.5)
		uv.x += sampleStrength;
    // this is the original colour of this fragment
    // using only this would result in a nonblurred version
    vec4 color = texture2D(CC_Texture0,uv); 
 
 
    gl_FragColor = color;
}

