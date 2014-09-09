varying vec2  uv;
attribute vec4 a_position;
// this vertex shader is from AMD RenderMonkey
void main(void)
{
	//a_position.x += 300;
	gl_Position = CC_MVPMatrix * a_position;
   //gl_Position = vec4( gl_Vertex.xy, 0.0, 1.0 );
   gl_Position = sign( gl_Position );

   // Texture coordinate for screen aligned (in correct range):
   uv = (vec2( gl_Position.x , gl_Position.y ) + vec2(1.0 )) / vec2( 2.0 );
   //uv = (vec2( gl_Position.x , gl_Position.y ) + vec2(1.0 , 0.7)) / vec2( 2.0 );
   //uv = vec2( gl_Position.x, 1.0 - gl_Position.y );
}