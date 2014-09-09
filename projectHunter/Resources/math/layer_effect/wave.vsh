/* Vertex shader */
uniform float u_time;
 
void main(void)
{
	vec4 v = vec4(gl_Vertex);
 
	v.z = sin(0.1 * v.x + u_time) * cos(0.1 * v.y + u_time) * 3.0;
 
	gl_Position = gl_ModelViewProjectionMatrix * v;
}