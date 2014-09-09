

"											\n\
	float alpha = result.a;						\n\
	\n\
	const vec4  kRGBToYPrime = vec4 (0.299, 0.587, 0.114, 0.0);			\n\
    const vec4  kRGBToI     = vec4 (0.596, -0.275, -0.321, 0.0);			\n\
    const vec4  kRGBToQ     = vec4 (0.212, -0.523, 0.311, 0.0);			\n\
	\n\
	const vec4  kYIQToR   = vec4 (1.0, 0.956, 0.621, 0.0);			\n\
    const vec4  kYIQToG   = vec4 (1.0, -0.272, -0.647, 0.0);			\n\
    const vec4  kYIQToB   = vec4 (1.0, -1.107, 1.704, 0.0);			\n\
    // Convert to YIQ										\n\
    float   YPrime  = dot (result, kRGBToYPrime);			\n\
    float   I      = dot (result, kRGBToI);			\n\
    float   Q      = dot (result, kRGBToQ);			\n\
	\n\
    // Calculate the hue and chroma  \n\
    float   hue     = atan (Q, I);			\n\
    float   chroma  = sqrt (I * I + Q * Q);			\n\
	\n\
    // Make the user's adjustments\n\
    hue += u_hue;			\n\
	\n\
    // Convert back to YIQ\n\
    Q = chroma * sin (hue);			\n\
    I = chroma * cos (hue);			\n\
	\n\
    // Convert back to RGB\n\
    vec4    yIQ   = vec4 (YPrime, I, Q, 0.0);			\n\
    float r = dot (yIQ, kYIQToR);			\n\
    float g = dot (yIQ, kYIQToG);			\n\
    float b = dot (yIQ, kYIQToB);			\n\
	\n\
    result = vec4(r, g, b, alpha); 				\n\
";