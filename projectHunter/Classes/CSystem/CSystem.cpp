#include "CSystem.h"
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
	#include "../../../iconv/include/iconv.h"
#endif
#define FLOAT_2_INT_MUL		10000
int CSystem::Float2Int(float f)
{
	int i = f * FLOAT_2_INT_MUL;
	return (int)i;
}
float CSystem::Int2Float(int i)
{
	float f = (float)i / FLOAT_2_INT_MUL;
	return f;
}

float CSystem::Limit(float in, float min, float max)
{
	if(max < min)
	{
		//CCAssert(false, "max small than min");
		return in;
	}
	in = in < min ? min : in;
	in = in > max ? max : in;
	return in;
}

int CSystem::Limit(int in, int min, int max)
{
	if(max < min)
	{
		//CCAssert(false, "max small than min");
		return in;
	}
	in = in < min ? min : in;
	in = in > max ? max : in;
	return in;
}

BYTE_TYPE CSystem::Mem_GetByte(BYTE_ARRAY_REF src, int src_off)
{
	return src[src_off];
}

SHORT_TYPE CSystem::Mem_GetShort(BYTE_ARRAY_REF src, int src_off) 				// 2 bytes
{
	return (short)((src[src_off] & 0xFF) |
		((src[src_off+1] & 0xFF) << 8));
}

INT_TYPE CSystem::Mem_GetInt(BYTE_ARRAY_REF src, int src_off) 				// 4 bytes
{
	return((src[src_off] & 0xFF) | ((src[src_off+1] & 0xFF) << 8) | ((src[src_off+2] & 0xFF) << 16) | ((src[src_off+3] & 0xFF) << 24));
}

int CSystem::code_convert(const char *from_charset, const char *to_charset, const char *inbuf, size_t inlen, char *outbuf, size_t outlen)
{
//#ifndef ANDROID
	iconv_t cd;
	const char *temp = inbuf;
	const char **pin = &temp;
	char **pout = &outbuf;
	memset(outbuf,0,outlen);
	cd = iconv_open(to_charset,from_charset);
	if(cd==0) return -1;
#ifdef WIN32
	if(iconv(cd,pin,&inlen,pout,&outlen)==-1) return -1;
#else
	if(iconv(cd,(char **)pin,&inlen,(char **)pout,&outlen)==-1) return -1;
#endif
	iconv_close(cd);
//#endif
	return 0;
}

/*UTF8תΪGB2312*/
std::string CSystem::u2a(const char *inbuf)
{
	std::string strRet;
#ifndef ANDROID
	size_t inlen = strlen(inbuf);
	char * outbuf = new char[inlen * 2 + 2];
	
	if(code_convert("utf-8", "gb2312", inbuf, inlen, outbuf, inlen * 2 + 2) == 0)
	{
		strRet = outbuf;
	}
	delete [] outbuf;
#endif
	return strRet;

}

/*GB2312תΪUTF8*/
std::string CSystem::a2u(const char *inbuf)
{
	std::string strRet;
//#ifndef ANDROID
	size_t inlen = strlen(inbuf);
	char * outbuf = new char[inlen * 2 + 2];
	
	if(code_convert("gb2312", "utf-8", inbuf, inlen, outbuf, inlen * 2 + 2) == 0)
	{
		strRet = outbuf;
	}
	delete [] outbuf;
//#endif
	return strRet;
}
std::vector<CCPoint> CSystem::getCircleTrack(CCPoint sp, CCPoint offset,float radius, float timeInterval)//获得一个圆的轨迹序列点
{
	//int counter = radius * 2 / timeInterval;

	float detla = timeInterval;

	float x = 0;
	float y = 0;
	float radiusSquare = radius * radius;

	std::vector<CCPoint> tmp;

	int CenterX = -radius;
	int CenterY = 0;

	while(CenterX < radius)
	{
		

		y = sqrt(radiusSquare - CenterX * CenterX) + offset.y;

		tmp.push_back(ccp(sp.x + CenterX + offset.x + radius, sp.y + y));

		CenterX += detla;
	}

	if((radiusSquare - CenterX * CenterX) < 0)
		CenterX = radius;

	while(CenterX >= -radius)
	{
		
		y = -sqrt(radiusSquare - CenterX * CenterX) + offset.y;

		tmp.push_back(ccp(sp.x + CenterX + offset.x + radius, sp.y + y));

		CenterX -= detla;
	}
	return tmp;
}

LONG_TYPE CSystem::currentTimeMillis()
{
    struct cc_timeval ptv;
    CCTime::gettimeofdayCocos2d(&ptv, NULL);
    LONG_TYPE tmp = ptv.tv_sec;
    LONG_TYPE tmp2 = 1000;
    tmp *= tmp2;
    tmp += ptv.tv_usec / tmp2;
    return tmp;
}

std::string CSystem::getTimeString(LONG_TYPE time)
{
	int hour = time / 3600000;
	int min = time / 60000 - hour * 60;
	int sec = time / 1000 - hour * 3600 - min * 60;

	char t[255];
	char shour[5];
	char smin[5];
	char ssec[5];
	if(hour > 0)
	{
		if(hour < 10)
			sprintf(shour, "0%d", hour);
		else
			sprintf(shour, "%d", hour);
	}

	if(min == 0)
		sprintf(smin, "00");
	else if(min < 10)
		sprintf(smin, "0%d", min);
	else
		sprintf(smin, "%d", min);

	if(sec == 0)
		sprintf(ssec, "00");
	else if(sec < 10)
		sprintf(ssec, "0%d", sec);
	else
		sprintf(ssec, "%d", sec);

	if(hour > 0)
		sprintf(t, "%s:%s:%s", shour, smin, ssec);
	else
		sprintf(t, "%s:%s", smin, ssec);

	std::string str = std::string();
	return t;

}

//以左上方为Y的0点坐标系转换为左下方为0点坐标系的转换函数，此函数将坐标系由左上角为0,0点转换为左下角为0,0点
float CSystem::coordinatesYTrans(float LeftTopCoordiinateY)
{
	return coordinatesYTrans(LeftTopCoordiinateY, 0, 0);
}
float CSystem::coordinatesYTrans(float LeftTopCoordiinateY, float ObjectHeight)
{
	return coordinatesYTrans(LeftTopCoordiinateY, ObjectHeight, 0);
}
float CSystem::coordinatesYTransWithHeightZero(float LeftTopCoordiinateY, float layerHeight)//点坐标转化，无height
{
	return coordinatesYTrans(LeftTopCoordiinateY, 0, layerHeight);
}
//layerHeight为-1时，自动将挂载layer转换为screen Height
float CSystem::coordinatesYTrans(float LeftTopCoordiinateY, float ObjectHeight, float layerHeight)
{
	if(ObjectHeight <= 0)
		ObjectHeight = 0;

	if(layerHeight <= 0)
	{
		CCSize s = CCDirector::sharedDirector()->getWinSize();
		layerHeight = s.height;
	}

	return layerHeight - LeftTopCoordiinateY - ObjectHeight;
}

int	CSystem::get2PtPositionRelation(CCPoint SrcPoint, CCPoint targetPoint)
{
	int ret = QUADRANT_ORIGIN;
	if(targetPoint.x >= SrcPoint.x)
	{
		if(targetPoint.y > SrcPoint.y)
		{
			ret = QUADRANT_1;
		} else 
		{
			ret = QUADRANT_4;
		}
	} else {
		if(targetPoint.y > SrcPoint.y)
		{
			ret = QUADRANT_2;
		} else 
		{
			ret = QUADRANT_3;
		}
	}

	return ret;
}

CCRect CSystem::rectSetScale(CCRect rect, float scale)
{
	return CCRectMake(rect.getMinX() * scale, rect.getMinY() * scale, rect.size.width * scale, rect.size.height * scale);
}
