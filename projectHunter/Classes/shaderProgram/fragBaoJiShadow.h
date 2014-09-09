

"											\n\
	vec4 colorOrg =  vec4(1.0-result.r,1.0-result.g, 1.0-result.b,u_BaoJiShadowAlpha * result.a ) ;						\n\
	vec4 colorDieJia = vec4(1,1, 1,0.5) ;												\n\
    float r = colorOrg.r * (1.0 - colorDieJia.a) + colorDieJia.r * colorDieJia.a;		\n\
	float g = colorOrg.g * (1.0 - colorDieJia.a) + colorDieJia.g * colorDieJia.a;		\n\
	float b = colorOrg.b * (1.0 - colorDieJia.a) + colorDieJia.b * colorDieJia.a;		\n\
	result = vec4(r,g,b,u_BaoJiShadowAlpha * result.a);											\n\
";