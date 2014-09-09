

"											\n\
	float alpha = result.a;						\n\
	float brightness = (result.r + result.g + result.b) * (1. / 2.); \n\
	float r = brightness * 1.8;					\n\
	float g = brightness * 0.6;					\n\
	float b = brightness * 0.6;					\n\
	result = vec4(r, g, b, alpha); 				\n\
";