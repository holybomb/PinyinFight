

"											\n\
float angle = atan(uv.y,uv.x);										 \n\
																	 \n\
																	 \n\
vec2 len = vec2(uv.y,uv.x );										 \n\
float radius = length(len);											 \n\
																	 \n\
																	 \n\
angle += u_BaiDongFreq * sin(u_BaiDongTime + u_BaiDongRandom);		 \n\
																	 \n\
texCoord = vec2(cos(angle) * radius, sin(angle) * radius);			\n\
texCoord.x += 0.5;													 \n\
";

