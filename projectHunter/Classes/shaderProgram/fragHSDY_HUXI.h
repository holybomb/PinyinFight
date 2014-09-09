

"											\n\
	float y= texCoord.y + texCoord.y * (sin (u_huxiWaveWidth+u_huxiTime) * 0.01);				\n\
	float x= texCoord.x + (texCoord.x - 0.25)* (sin (u_huxiWaveWidth+u_huxiTime) * 0.01);		\n\
	\n\
	if(texCoord.x < 0.25)																		\n\
		x= texCoord.x - (0.25 - texCoord.x) *(sin (u_huxiWaveWidth+u_huxiTime) * 0.01);				\n\
	\n\
	texCoord = vec2(x, y);					\n\
";