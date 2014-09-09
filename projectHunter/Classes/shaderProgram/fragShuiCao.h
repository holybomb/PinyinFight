

"											\n\
	float t= texCoord.x + (sin (texCoord.y * u_ShuiCaoWaveWidth+u_ShuiCaoTime + u_ShuiCaoRandom) * u_ShuiCaoFreq);	\n\
	texCoord = vec2(t, texCoord.y);	\n\
";