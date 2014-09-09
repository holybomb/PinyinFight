

"											\n\
	float t = clamp(u_ShuiDouDongTime / 6.0, 0.0, 1.0);								\n\
																					 \n\
	vec2 coords = texCoord;															 \n\
	vec2 dir;																		 \n\
	dir.x = coords.x - 0.5;															 \n\
	dir.y = coords.y - 0.5;															 \n\
																					 \n\
	float dist = distance(coords, vec2(0.5, 0));//zhong xin dian					 \n\
	vec2 offset = dir * (sin(dist * 80.0 - u_ShuiDouDongTime*15.0) + 0.5) / 100.0;  //dir * (sin(dist * 80.0 - u_ShuiDouDongTime*15.0) + 0.5) / 80.0[zhen fu]	\n\
	texCoord = coords + offset;														\n\
	vec4 diffuse = texture2D(u_texture, texCoord);									\n\
																					\n\
	vec4 mixin = texture2D(u_texture, texCoord);									\n\
																					\n\
	result = mixin * t + diffuse * (1.0 - t);										\n\
";