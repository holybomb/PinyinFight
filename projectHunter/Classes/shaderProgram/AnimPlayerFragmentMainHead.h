

"											\n\
#ifdef GL_ES								\n\
precision mediump float;					\n\
#endif										\n\
\n\
varying vec4 v_fragmentColor;				\n\
varying vec2 v_texCoord;					\n\
uniform sampler2D u_texture;				\n\
varying vec2 uv;							\n\
\n\
uniform float u_DengBaiDongTime;			\n\
uniform float u_DengBaiDongRandom;			\n\
uniform float u_DengBaiDongFreq;			\n\
\n\
uniform float u_BaiDongTime;				\n\
uniform float u_BaiDongRandom;				\n\
uniform float u_BaiDongWaveWidth;			\n\
uniform float u_BaiDongFreq;				\n\
\n\
uniform vec3 oneColor;				\n\
\n\
uniform vec4 DieJiaColor;					\n\
uniform float DieJiaCurAlpha;				\n\
\n\
uniform vec2 GaussianBlurSize;				\n\
uniform vec4 GaussianSubstract;				\n\
\n\
uniform float u_huxiTime;					\n\
uniform float u_huxiWaveWidth;				\n\
\n\
uniform float u_ShuiCaoTime;				\n\
uniform float u_ShuiCaoRandom;				\n\
uniform float u_ShuiCaoWaveWidth;			\n\
uniform float u_ShuiCaoFreq;				\n\
\n\
uniform float u_waveTime;					\n\
uniform float u_waveRandom;					\n\
uniform float u_waveWaveWidth;				\n\
uniform float u_wavefreq;					\n\
uniform float u_hue;					\n\
\n\
uniform vec3 SeJieInPut;					\n\
uniform vec2 SeJieOutPut;					\n\
\n\
uniform vec3 actorBianSe;					\n\
\n\
uniform float u_ShuiDouDongTime;			\n\
\n\
uniform float u_BaoJiShadowAlpha;			\n\
\n\
uniform float u_grassWaveTime;				\n\
\n\
uniform float u_YunPingTaiTime;					\n\
uniform float u_YunPingTaiWaveWidth;		\n\
\n\
void main()									\n\
{											\n\
	vec2 texCoord = v_texCoord;				\n\
";