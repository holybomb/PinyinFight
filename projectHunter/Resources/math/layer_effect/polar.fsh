#ifdef GL_ES 
precision mediump float; 
#endif 
uniform sampler2D u_texture; 
varying vec2 v_texCoord; 
//uniform float amount; //amount of twirl
varying vec2 uv;
void main() {

   //uv.x = (uv.x * 2 - 1.0) - 0.5; 
   
   //uv.x -= 0.5;
   
   
	
	
	
	float angle = atan2(uv.y,uv.x);
	
	
	vec2 len = vec2(uv.y,uv.x );
	float radius = length(len);
	
	if(uv.x < 0)
	{
		//angle = 3.1415926535 - angle;
		//angle += 0.01;
	} else 
	{
		//angle -= 0.01;
	}
	//radius = pow(radius, 2);
	//radius += radius * 0.01;
	float yPlus = pow(radius, 3);
	vec2 shifted = vec2(cos(angle) * radius, sin(angle) * yPlus);

	
	shifted.x += 0.5;
	shifted.y += 0.9;
	//shifted.y /= 3;
	gl_FragColor = texture(u_texture, shifted);	

  
}
