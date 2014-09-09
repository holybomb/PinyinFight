#ifdef GL_ES 
precision mediump float; 
#endif 
uniform sampler2D u_texture; 
varying vec2 v_texCoord; 
varying vec4 v_fragmentColor; 

// Die Jia
uniform vec4 DieJiaColor;
uniform int	enableDieJia;
vec4 getDiejia(vec4 rgbOrigin)
{
	//rgbOrigin = texture2D(u_texture, v_texCoord);
	float r = rgbOrigin.r * (1.0 - DieJiaColor.a) + DieJiaColor.r * DieJiaColor.a;
	float g = rgbOrigin.g * (1.0 - DieJiaColor.a) + DieJiaColor.g * DieJiaColor.a;
	float b = rgbOrigin.b * (1.0 - DieJiaColor.a) + DieJiaColor.b * DieJiaColor.a;
	return vec4(r,g,b,rgbOrigin.a);
}

uniform int	enableGray;
vec4 getGray(vec4 rgbOrigin) 
{ 
	float alpha = rgbOrigin.a;
	float grey = dot(rgbOrigin.rgb, vec3(0.299, 0.587, 0.114)); 
	return vec4(grey, grey, grey, alpha); 
}

// select shan shuo
uniform int	enableGlow;
vec4 getGlow(vec4 rgbOrigin)
{
	float alpha = rgbOrigin.a; 
	float r = rgbOrigin.r * 1.6;
	float g = rgbOrigin.g * 1.6;
	float b = rgbOrigin.b ;
	return vec4(r, g, b, alpha); 
}



//One Color
uniform vec3 oneColor;
uniform int	enableOneColor;
vec4 getOneColor(vec4 rgbOrigin)
{
	float alpha = rgbOrigin.a; 
	return vec4(oneColor.r, oneColor.g, oneColor.b, alpha); // white color only
}



// Se Jie
uniform int	enableSeJie;
uniform vec3 SeJieInPut;
uniform vec2 SeJieOutPut;
vec4 getSeJie(vec4 rgbOrigin) 
{ 
	float alpha = rgbOrigin.a; 
	float r = rgbOrigin.r ;
	float g = rgbOrigin.g ;
	float b = rgbOrigin.b ;

	float min_in = SeJieInPut.r;
	float max_in = SeJieInPut.g;
	float inGamma = 1.0/SeJieInPut.b;

	float min_out = SeJieOutPut.x;
	float max_out = SeJieOutPut.y;

	float cr = r;


	r = (pow(((cr * 255.0) - min_in) / (max_in - min_in),
					inGamma) * (max_out - min_out) + min_out) / 255.0;
	cr = g;
	g = (pow(((cr * 255.0) - min_in) / (max_in - min_in),
					inGamma) * (max_out - min_out) + min_out) / 255.0;				
	cr = b;
	b = (pow(((cr * 255.0) - min_in) / (max_in - min_in),
					inGamma) * (max_out - min_out) + min_out) / 255.0;

	return vec4(r, g, b, alpha); 
}



// Gaussian blur
uniform int	  enableGaussianBlur;
uniform vec2 GaussianBlurSize;
uniform vec4 GaussianSubstract;
vec4 getGaussianBlur(vec4 rgbOrigin, vec2 texCoord)
{
	vec4 sum = vec4(0.0);
	sum += texture2D(u_texture, v_texCoord - 4.0 * GaussianBlurSize) * 0.05;
	sum += texture2D(u_texture, v_texCoord - 3.0 * GaussianBlurSize) * 0.09;
	sum += texture2D(u_texture, v_texCoord - 2.0 * GaussianBlurSize) * 0.12;
	sum += texture2D(u_texture, v_texCoord - 1.0 * GaussianBlurSize) * 0.15;
	sum += texture2D(u_texture, v_texCoord                 ) * 0.2;
	sum += texture2D(u_texture, v_texCoord + 1.0 * GaussianBlurSize) * 0.15;
	sum += texture2D(u_texture, v_texCoord + 2.0 * GaussianBlurSize) * 0.12;
	sum += texture2D(u_texture, v_texCoord + 3.0 * GaussianBlurSize) * 0.09;
	sum += texture2D(u_texture, v_texCoord + 4.0 * GaussianBlurSize) * 0.05;

	vec4 ret = (sum - GaussianSubstract) * v_fragmentColor;
	vec4 ret1 = vec4(ret.r, ret.g, ret.b, rgbOrigin.a);
	return ret;
}

uniform int	  enableWave;
uniform float u_waveTime;
uniform float u_waveRandom;
uniform float u_waveWaveWidth;
uniform float u_wavefreq;
vec2 getWave(vec2 texCoord)
{
	float t= texCoord.y + u_waveRandom + (sin (texCoord.x * u_waveWaveWidth+u_waveTime) * u_wavefreq);
	return vec2(texCoord.x, t);
}

// shui cao wave
uniform int	  enableShuiCao;
uniform float u_ShuiCaoTime;
uniform float u_ShuiCaoRandom;
uniform float u_ShuiCaoWaveWidth;
uniform float u_ShuiCaoFreq;
vec2 getShuiCao(vec2 texCoord)
{
	float t= texCoord.x + (sin (texCoord.y * u_ShuiCaoWaveWidth+u_ShuiCaoTime + u_ShuiCaoRandom) * u_ShuiCaoFreq);
	return vec2(t, texCoord.y);
}


//huo shan di yu HUXI
uniform int enableHSDYHuXi;
uniform float u_huxiTime;
uniform float u_huxiWaveWidth;
vec2 getHSDY_HUXI(vec2 texCoord)
{
	float y= texCoord.y + texCoord.y * (sin (u_huxiWaveWidth+u_huxiTime) * 0.01);
	float x= texCoord.x + (texCoord.x - 0.25)* (sin (u_huxiWaveWidth+u_huxiTime) * 0.01);
	
	if(texCoord.x < 0.25)
		x= texCoord.x - (0.25 - texCoord.x) *(sin (u_huxiWaveWidth+u_huxiTime) * 0.01);
	
	return vec2(x, y);
}


// shui cao wave
uniform int	  enableBaiDong;
uniform float u_BaiDongTime;
uniform float u_BaiDongRandom;
uniform float u_BaiDongWaveWidth;
uniform float u_BaiDongFreq;
varying vec2 uv;
vec2 getBaiDong(vec2 texCoord)
{
	float angle = atan(uv.y,uv.x);
	
	
	vec2 len = vec2(uv.y,uv.x );
	float radius = length(len);
	
	
	angle += u_BaiDongFreq * sin(u_BaiDongTime + u_BaiDongRandom);
	
	vec2 shifted = vec2(cos(angle) * radius, sin(angle) * radius);
	//shifted.x += 0.5;
	//shifted.y -= 1.0;
	shifted.x += 0.5;
	return shifted;
}



// shui cao wave
uniform int	  enableDengBaiDong;
uniform float u_DengBaiDongTime;
uniform float u_DengBaiDongRandom;
uniform float u_DengBaiDongFreq;
vec2 getDengBaiDong(vec2 texCoord)
{
	vec2 shifted = uv;
	
	shifted.x += u_DengBaiDongFreq * uv.y *  sin(u_DengBaiDongTime + u_DengBaiDongRandom);
	shifted.x += 0.5;
	
	return shifted;
}





void main(void) 
{
	vec2 texCoord = v_texCoord;
	
	//wave part
	if(enableWave == 1)
	{
		vec2 ret = getWave(texCoord);
		texCoord = ret;
	}
	
	if(enableShuiCao == 1)
	{
		vec2 ret = getShuiCao(texCoord);
		texCoord = ret;
	}
	
	if(enableHSDYHuXi == 1)
	{
		vec2 ret = getHSDY_HUXI(texCoord);
		texCoord = ret;
	}
	
	
	if(enableDengBaiDong == 1)
	{
		vec2 ret = getDengBaiDong(texCoord);
		texCoord = ret;
	}
	
	
	if(enableBaiDong == 1)
	{
		vec2 ret = getBaiDong(texCoord);
		texCoord = ret;
	}
	
	
	vec4 result = texture2D(u_texture, texCoord);
	if(enableGaussianBlur == 1)
	{
		vec4 ret = getGaussianBlur(result, texCoord);
		result = ret;
	}
	
	
	// color part
	if(enableDieJia == 1)
	{
		vec4 ret = getDiejia(result);
		result = ret;
	}	
	
	if(enableSeJie == 1)
	{
		vec4 ret = getSeJie(result);
		result = ret;
	}
	
	
	if(enableGlow == 1)
	{
		vec4 ret = getGlow(result);
		result = ret;
	}
	
	
	if(enableOneColor == 1)
	{
		vec4 ret = getOneColor(result);
		result = ret;
	}
	
	if(enableGray == 1)
	{
		vec4 ret = getGray(result);
		result = ret;
	}
	
	
	

	gl_FragColor = vec4(result.r, result.g, result.b, result.a); 
}