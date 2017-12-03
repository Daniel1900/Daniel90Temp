#pragma once
#include "stdafx.h"
class CxImage;
class CImageCtrl
{
public:
	CImageCtrl();
	~CImageCtrl();
	
	BOOL ZoomOnPtr(CxImage *imgCur, double del, CPoint point, CxImage *imgOrg, CRect &crPrv);
	void DragPic(CxImage *imgCur, double del, CPoint point, CxImage *imgOrg, CRect &crPrv);
	void onDraw(CDC* pDC, CxImage *img);
	static CImageCtrl* GetInstance();
	BOOL TransPtToImg(CView *pView, CxImage *img, CPoint &pt, UINT nType);
private:
	CDC *m_pDC;
	CxImage *m_img;
	static CImageCtrl* m_pCimgCtrl;
	CRect GetCurRect(CRect rcWin, CxImage *img);

};

