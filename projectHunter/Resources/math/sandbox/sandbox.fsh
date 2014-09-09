#ifdef GL_ES 
precision mediump float; 
#endif 
uniform sampler2D u_texture; 
varying vec2 v_texCoord; 
varying vec4 v_fragmentColor; 
void main(void)
{
    vec4 result = texture2D(u_texture, v_texCoord) * v_fragmentColor;
	
    const vec4  kRGBToYPrime = vec4 (0.299, 0.587, 0.114, 0.0);
    const vec4  kRGBToI     = vec4 (0.596, -0.275, -0.321, 0.0);
    const vec4  kRGBToQ     = vec4 (0.212, -0.523, 0.311, 0.0);

    const vec4  kYIQToR   = vec4 (1.0, 0.956, 0.621, 0.0);
    const vec4  kYIQToG   = vec4 (1.0, -0.272, -0.647, 0.0);
    const vec4  kYIQToB   = vec4 (1.0, -1.107, 1.704, 0.0);


    // Convert to YIQ
    float   YPrime  = dot (result, kRGBToYPrime);
    float   I      = dot (result, kRGBToI);
    float   Q      = dot (result, kRGBToQ);

    // Calculate the hue and chroma
    float   hue     = atan (Q, I);
    float   chroma  = sqrt (I * I + Q * Q);

    // Make the user's adjustments
    hue += 3;

    // Convert back to YIQ
    Q = chroma * sin (hue);
    I = chroma * cos (hue);

    // Convert back to RGB
    vec4    yIQ   = vec4 (YPrime, I, Q, 0.0);
    result.r = dot (yIQ, kYIQToR);
    result.g = dot (yIQ, kYIQToG);
    result.b = dot (yIQ, kYIQToB);

    // Save the result
    gl_FragColor    = result;
}





