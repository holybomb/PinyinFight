

"											\n\
	float alpha = result.a;											\n\
	float grey = dot(result.rgb, vec3(0.299, 0.587, 0.114)); 		\n\
	result = vec4(grey, grey, grey, alpha); 						\n\
";