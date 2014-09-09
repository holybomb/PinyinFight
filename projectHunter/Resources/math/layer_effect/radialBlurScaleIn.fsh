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
	 // some sample positions
   float samples[10] ;
   samples[0] = -0.12;
   samples[1] = -0.11;
   samples[2] = -0.10;
   samples[3] = -0.09;
   samples[4] = -0.08;
   samples[5] = -0.07;
   samples[6] = -0.06;
   samples[7] = -0.05;
   samples[8] = -0.04;
   samples[9] = -0.03;

   
   
   //float[](-0.08,-0.05,-0.03,-0.02,-0.01,0.01,0.02,0.03,0.05,0.08);

    // 0.5,0.5 is the center of the screen
    // so substracting uv from it will result in
    // a vector pointing to the middle of the screen
    vec2 dir = 0.5 - uv; 
 
    // calculate the distance to the center of the screen
    float dist = sqrt(dir.x*dir.x + dir.y*dir.y); 
 
    // normalize the direction (reuse the distance)
    dir = dir/dist; 
 
    // this is the original colour of this fragment
    // using only this would result in a nonblurred version
    vec4 color = texture2D(CC_Texture0,uv); 
 
    vec4 sum = color;
 
    // take 10 additional blur samples in the direction towards
    // the center of the screen
    for (int i = 0; i < 10; i++)
    {
      sum += texture2D( CC_Texture0, uv + dir * samples[i] * sampleDist );
    }
 
    // we have taken eleven samples
    sum *= 1.0/11.0;
 
    // weighten the blur effect with the distance to the
    // center of the screen ( further out is blurred more)
    float t = dist * sampleStrength;
    t = clamp( t ,0.0,1.0); //0 &lt;= t &lt;= 1
 
    //Blend the original color with the averaged pixels
    gl_FragColor = mix( color, sum, t );
}

