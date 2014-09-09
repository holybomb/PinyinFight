

"																					\n\
	float r = result.r * (1.0 - DieJiaColor.a) + DieJiaColor.r * DieJiaColor.a;		\n\
	float g = result.g * (1.0 - DieJiaColor.a) + DieJiaColor.g * DieJiaColor.a;		\n\
	float b = result.b * (1.0 - DieJiaColor.a) + DieJiaColor.b * DieJiaColor.a;		\n\
	result = vec4(r, g, b, result.a * DieJiaCurAlpha);	\n\
";