
// This is animPlayer shader

attribute vec4 a_position;							
attribute vec2 a_texCoord;							
attribute vec4 a_color;								

#ifdef GL_ES										
varying lowp vec4 v_fragmentColor;					
varying mediump vec2 v_texCoord;					
#else												
varying vec4 v_fragmentColor;						
varying vec2 v_texCoord;							
#endif												

varying vec2 uv;

uniform int	enableBaiDongVex;
void set2Polar() 
{ 
	gl_Position = CC_MVPMatrix * a_position;		
	v_fragmentColor = a_color;
    v_texCoord = a_texCoord;
   //v_texCoord = sign( v_texCoord );

   uv = (vec2( v_texCoord.x, v_texCoord.y ) + vec2(-0.5,0) );

}


void main()											
{		
	if(enableBaiDongVex == 1)
	{
		set2Polar();
	} else 
	{
		gl_Position = CC_MVPMatrix * a_position;		
		v_fragmentColor = a_color;						
		v_texCoord = a_texCoord;						
	}
}													