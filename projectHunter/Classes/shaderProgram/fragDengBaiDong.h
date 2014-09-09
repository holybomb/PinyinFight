

"											\n\
	texCoord = uv;																			\n\
																								\n\
	texCoord.x += u_DengBaiDongFreq * uv.y *  sin(u_DengBaiDongTime + u_DengBaiDongRandom);		\n\
	texCoord.x += 0.5;																			\n\
";