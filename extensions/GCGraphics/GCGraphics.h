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
	//��Ե��ɫ
	ccColor4F m_borderColor;
	//�����ɫ 
	ccColor4F m_fillColor;
	//�߿�
	float m_lineWidth;
	//��������
	int m_drawType;
	//��Ե�Ƿ�Ϊ����
	bool m_isBorderStipple;

public:
	GCGraphics();
	~GCGraphics();

	CREATE_FUNC(GCGraphics);

	//******************************************************************
	//								draw Line
	//******************************************************************
	CCPoint lsPt, lePt;//line�Ŀ�ʼ�ͽ�����
	static GCGraphics *CreateLine(CCPoint sPt, CCPoint ePt);
	static GCGraphics *CreateStippleLine(ccColor4F color, CCPoint sPt, CCPoint ePt);
	static GCGraphics *CreateLine(ccColor4F color, CCPoint sPt, CCPoint ePt, float lineWidth, bool isStipple);
	void drawLine();


	//******************************************************************
	//								draw Rect
	//******************************************************************
	CCPoint rsPt, rePt;//rect�Ŀ�ʼ�ͽ�����
	bool m_isRectFilled;
	static GCGraphics *CreateRect(CCPoint sPt, CCPoint ePt, bool isFilled);
	static GCGraphics *CreateStippleRect(ccColor4F borderColor, CCPoint sPt, CCPoint ePt);
	static GCGraphics *CreateRect(ccColor4F borderColor, ccColor4F fillColor, CCPoint sPt, CCPoint ePt, float lineWidth, bool isFilled, bool isStipple);
	//����rect�Ĵ�С
	void setRect(CCPoint sPt, CCPoint ePt);
	void drawRect();


	virtual void draw();
};
#endif