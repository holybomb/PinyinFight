#ifndef __GCGRAPHICS_H__
#define __GCGRAPHICS_H__
#include "cocos2d.h"
#include "ExtensionMacros.h"
USING_NS_CC;
enum {
	GCGraphicsNone = 0,
	GCGraphicsLine,
	GCGraphicsRectNoFilled,
	GCGraphicsRectFilled,
};
class CC_EX_DLL GCGraphics : public CCNodeRGBA
{
private:
	//边缘颜色
	ccColor4F m_borderColor;
	//填充颜色 
	ccColor4F m_fillColor;
	//线宽
	float m_lineWidth;
	//绘制内容
	int m_drawType;
	//边缘是否为虚线
	bool m_isBorderStipple;

public:
	GCGraphics();
	~GCGraphics();

	CREATE_FUNC(GCGraphics);

	//******************************************************************
	//								draw Line
	//******************************************************************
	CCPoint lsPt, lePt;//line的开始和结束点
	static GCGraphics *CreateLine(CCPoint sPt, CCPoint ePt);
	static GCGraphics *CreateStippleLine(ccColor4F color, CCPoint sPt, CCPoint ePt);
	static GCGraphics *CreateLine(ccColor4F color, CCPoint sPt, CCPoint ePt, float lineWidth, bool isStipple);
	void drawLine();


	//******************************************************************
	//								draw Rect
	//******************************************************************
	CCPoint rsPt, rePt;//rect的开始和结束点
	bool m_isRectFilled;
	static GCGraphics *CreateRect(CCPoint sPt, CCPoint ePt, bool isFilled);
	static GCGraphics *CreateStippleRect(ccColor4F borderColor, CCPoint sPt, CCPoint ePt);
	static GCGraphics *CreateRect(ccColor4F borderColor, ccColor4F fillColor, CCPoint sPt, CCPoint ePt, float lineWidth, bool isFilled, bool isStipple);
	//更新rect的大小
	void setRect(CCPoint sPt, CCPoint ePt);
	void drawRect();


	virtual void draw();
};
#endif