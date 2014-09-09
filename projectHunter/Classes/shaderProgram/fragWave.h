

"											\n\
	float t= texCoord.y + u_waveRandom + (sin (texCoord.x * u_waveWaveWidth+u_waveTime) * u_wavefreq);	\n\
	texCoord = vec2(texCoord.x, t);					\n\
";