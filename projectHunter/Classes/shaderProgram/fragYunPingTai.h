

"											\n\
	float y= texCoord.y + texCoord.y * (sin (u_YunPingTaiWaveWidth+u_YunPingTaiTime / 2.0) * 0.02);				\n\
	float x= texCoord.x + (texCoord.x - 0.25)* (sin (u_YunPingTaiWaveWidth+u_YunPingTaiTime / 2.0) * 0.02);		\n\
	\n\
	if(texCoord.x < 0.25)																		\n\
		x= texCoord.x - (0.25 - texCoord.x) *(sin (u_YunPingTaiWaveWidth+u_YunPingTaiTime / 2.0) * 0.02);				\n\
	\n\
	texCoord = vec2(x, y);					\n\
";