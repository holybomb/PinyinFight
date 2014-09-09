#include "GCGraphics.h"

GCGraphics::GCGraphics()
:m_isBorderStipple(false)
{

}

GCGraphics::~GCGraphics()
{

}


//******************************* draw line *******************************************************
GCGraphics *GCGraphics::CreateLine(CCPoint sPt, CCPoint ePt)
{
	return CreateLine(ccc4f(0,0,0,255), sPt, ePt, 1, false);
}

GCGraphics *GCGraphics::CreateStippleLine(ccColor4F color, CCPoint sPt, CCPoint ePt)
{
	return CreateLine(color, sPt, ePt, 1, true);
}
GCGraphics *GCGraphics::CreateLine(ccColor4F color, CCPoint sPt, CCPoint ePt, float lineWidth, bool isStipple)
{
	GCGraphics *pRet = new GCGraphics();

	if(pRet)
	{
		pRet->lsPt = sPt;
		pRet->lePt = ePt;
		pRet->m_borderColor = color;
		pRet->m_lineWidth = lineWidth;
		pRet->m_drawType = GCGraphicsLine;
		pRet->m_isBorderStipple = isStipple;
		pRet->autorelease();
		return pRet;
	}

	return NULL;

}

#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
	#pragma comment(lib, "opengl32.lib")
#endif
void GCGraphics::drawLine()
{
	if(m_isBorderStipple)
	{
		glEnable(GL_LINE_STIPPLE);//可以虚线，先声明可以使用 
		glLineStipple (3, 0xAAAA); 
	}

	ccDrawColor4B(m_borderColor.r, m_borderColor.g, m_borderColor.b, m_borderColor.a);
	glLineWidth(m_lineWidth);
	ccDrawLine(lsPt, lePt);

	if(m_isBorderStipple)
	{
		glDisable(GL_LINE_STIPPLE);//可以虚线，先声明可以使用 
	}
}




GCGraphics *GCGraphics::CreateRect(CCPoint sPt, CCPoint ePt, bool isFilled)
{
	return CreateRect(ccc4f(0,0,0,255), ccc4f(0,0,0,255), sPt, ePt, 1, isFilled, false);
}

GCGraphics *GCGraphics::CreateStippleRect(ccColor4F borderColor, CCPoint sPt, CCPoint ePt)
{
	return CreateRect(borderColor, ccc4f(0,0,0,255), sPt, ePt, 1, false, true);
}
GCGraphics *GCGraphics::CreateRect(ccColor4F borderColor, ccColor4F fillColor, CCPoint sPt, CCPoint ePt, float lineWidth, bool isFilled, bool isStipple)
{
	GCGraphics *pRet = new GCGraphics();

	if(pRet)
	{
		pRet->rsPt = sPt;
		pRet->rePt = ePt;
		pRet->m_borderColor = borderColor;
		pRet->m_fillColor = fillColor;
		pRet->m_lineWidth = lineWidth;
		pRet->m_isBorderStipple = isStipple;
		if(isFilled)
			pRet->m_drawType = GCGraphicsRectFilled;
		else
			pRet->m_drawType = GCGraphicsRectNoFilled;
		pRet->autorelease();
		return pRet;
	}

	return NULL;
}

//更新rect的大小
void GCGraphics::setRect(CCPoint sPt, CCPoint ePt)
{

	CCAssert(m_drawType == GCGraphicsRectFilled || m_drawType == GCGraphicsRectNoFilled, "This is Not Rect Graphics");

	rsPt = sPt;
	rePt = ePt;
}

void GCGraphics::drawRect()
{
	ccDrawColor4B(m_borderColor.r, m_borderColor.g, m_borderColor.b, m_borderColor.a);
	glLineWidth(m_lineWidth);

	if(m_isBorderStipple)
	{
		glEnable(GL_LINE_STIPPLE);//可以虚线，先声明可以使用 
		glLineStipple (3, 0xAAAA); 
	}
	
	if(m_drawType == GCGraphicsRectFilled)
		ccDrawSolidRect(rsPt,rePt, m_fillColor);
	else
		ccDrawRect(rsPt, rePt);

	if(m_isBorderStipple)
	{
		glDisable(GL_LINE_STIPPLE);//可以虚线，先声明可以使用 
	}
}



void GCGraphics::draw()
{
	switch(m_drawType)
	{
	case GCGraphicsLine:
		drawLine();
		break;
	case GCGraphicsRectFilled:
	case GCGraphicsRectNoFilled:
		drawRect();
		break;
	}
}