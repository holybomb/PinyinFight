

"											\n\
	float alpha = result.a;									\n\
	float r = result.r ;										\n\
	float g = result.g ;										\n\
	float b = result.b ;										\n\
																\n\
	float min_in = SeJieInPut.r;								\n\
	float max_in = SeJieInPut.g;								\n\
	float inGamma = 1.0/SeJieInPut.b;							\n\
																\n\
	float min_out = SeJieOutPut.x;								\n\
	float max_out = SeJieOutPut.y;								\n\
																\n\
	float cr = r;												\n\
																\n\
																\n\
	r = (pow(((cr * 255.0) - min_in) / (max_in - min_in),		\n\
		inGamma) * (max_out - min_out) + min_out) / 255.0;		\n\
	cr = g;														\n\
	g = (pow(((cr * 255.0) - min_in) / (max_in - min_in),		\n\
		inGamma) * (max_out - min_out) + min_out) / 255.0;		\n\
	cr = b;														\n\
	b = (pow(((cr * 255.0) - min_in) / (max_in - min_in),		\n\
		inGamma) * (max_out - min_out) + min_out) / 255.0;		\n\
																\n\
	result = vec4(r, g, b, alpha); 								\n\
";