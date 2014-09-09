

"											\n\
											\n\
	float height = 1.0 - texCoord.y;		\n\
	float offset = pow(height, 5.0);		\n\
	offset *= (sin(u_grassWaveTime * 2.0) * 0.2);	\n\
	texCoord.x = texCoord.x + offset;		\n\
						\n\
";