

"											\n\
	vec4 sum = vec4(0.0);					\n\
	sum += texture2D(u_texture, v_texCoord - 4.0 * GaussianBlurSize) * 0.05;	\n\
	sum += texture2D(u_texture, v_texCoord - 3.0 * GaussianBlurSize) * 0.09;	\n\
	sum += texture2D(u_texture, v_texCoord - 2.0 * GaussianBlurSize) * 0.12;	\n\
	sum += texture2D(u_texture, v_texCoord - 1.0 * GaussianBlurSize) * 0.15;	\n\
	sum += texture2D(u_texture, v_texCoord                 ) * 0.2;				\n\
	sum += texture2D(u_texture, v_texCoord + 1.0 * GaussianBlurSize) * 0.15;	\n\
	sum += texture2D(u_texture, v_texCoord + 2.0 * GaussianBlurSize) * 0.12;	\n\
	sum += texture2D(u_texture, v_texCoord + 3.0 * GaussianBlurSize) * 0.09;	\n\
	sum += texture2D(u_texture, v_texCoord + 4.0 * GaussianBlurSize) * 0.05;	\n\
																				\n\
	result = (sum - GaussianSubstract) * v_fragmentColor;						\n\
";