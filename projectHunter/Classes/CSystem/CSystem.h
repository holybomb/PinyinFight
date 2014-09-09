#ifndef ___CSYSTEM___H
#define ___CSYSTEM___H
#include <string>
#include "cocos2d.h"
USING_NS_CC;
#include "../defines.h"
#ifndef ANDROID
	#include "iconv.h"
#endif



class CSystem
{
public:
	static int Float2Int(float f);
	static float Int2Float(int i);

	static float Angle2Arc(float angle) { return angle * 3.14 / 180; };

	static BYTE_TYPE Mem_GetByte(BYTE_ARRAY_REF src, int src_off);
	static SHORT_TYPE Mem_GetShort(BYTE_ARRAY_REF src, int src_off); 	// 2 bytes
	static INT_TYPE Mem_GetInt(BYTE_ARRAY_REF src, int src_off); 		// 4 bytes

	static float Limit(float in, float min, float max);
    static int Limit(int in, int min, int max);
    static LONG_TYPE currentTimeMillis();
	static std::string getTimeString(LONG_TYPE time);

	static int code_convert(const char *from_charset, const char *to_charset, const char *inbuf, size_t inlen, char *outbuf, size_t outlen);
	static std::string u2a(const char *inbuf) ;
	static std::string a2u(const char *inbuf);

	//以左上方为Y的0点坐标系转换为左下方为0点坐标系的转换函数，此函数将坐标系由左上角为0,0点转换为左下角为0,0点
	static float coordinatesYTrans(float LeftTopCoordiinateY);
	static float coordinatesYTrans(float LeftTopCoordiinateY, float ObjectHeight);
	static float coordinatesYTransWithHeightZero(float LeftTopCoordiinateY, float layerHeight);//点坐标转化，无height
	//layerHeight为-1时，自动将挂载layer转换为screen Height
	static float coordinatesYTrans(float LeftTopCoordiinateY, float ObjectHeight, float layerHeight);

	static std::vector<CCPoint> getCircleTrack(CCPoint sp, CCPoint offset,float radius, float timeInterval);//获得一个圆的轨迹序列点

	static int get2PtPositionRelation(CCPoint SrcPoint, CCPoint targetPoint);//确定两点之间的位置关系，1,2,3,4象限，Src为原点，Target为需要计算的响应点

	static CCRect rectSetScale(CCRect rect, float scale);
};

#endif
