#include "stdafx.h"
#include "CImageCtrl.h"
#include "ximage.h"

CImageCtrl* CImageCtrl::m_pCimgCtrl = NULL;

CImageCtrl::CImageCtrl()
{
}


CImageCtrl::~CImageCtrl()
{
}

BOOL CImageCtrl::ZoomOnPtr(CxImage *imgCur, double del, CPoint point, CxImage *imgOrg, CRect &crPrv)
{
	//m_img->Copy(*img);
	m_img = new CxImage();
	m_img->Copy(*imgOrg);
	if (imgOrg->IsValid() & crPrv.Height() == -1)
	{
		CRect rc(CPoint(0, 0), CSize(imgOrg->GetWidth()*(1 - del), imgOrg->GetHeight()*(1 - del)));

		int nl = (double)point.x * (del);
		int nt = (double)point.y * (del);
		rc.MoveToXY(nl, nt);
		//int nr = point.x + (double)(rc.right - point.x)*(1 - del);
		//int nb = point.y + (double)(rc.bottom - point.y)*(1 - del);
		CString s;
		//s.Format("%d,%d,%d,%d,%d, %d", nl, nt, nr, nb, rc.right, rc.bottom);
		//if (nl == rc.left | nt == rc.top | nr == rc.right | nb == rc.bottom)
		//return;
		imgOrg->Crop(rc, imgCur);
		crPrv = new CRect(0, 0, 0, 0);
		crPrv = rc;
		if (rc.Height() == imgOrg->GetHeight() & rc.Width() == imgOrg->GetWidth())
			return false;
		else
			return true;
	}
	else
	{
		CRect rc(CPoint(0, 0), CSize(imgOrg->GetWidth()*(1 - del), imgOrg->GetHeight()*(1 - del)));

		int nl = (double)(point.x + crPrv.left) * (del);
		int nt = (double)(point.y + crPrv.top) * (del);
		CString s;
		s.Format("%d,%d", crPrv.Width(),  rc.Width());
		//if (nl == rc.left | nt == rc.top | nr == rc.right | nb == rc.bottom)
		//return;
		CRect pivot = CRect(CPoint(nl, nt), CSize(rc.Width(), rc.Height()));
		imgOrg->Crop(pivot, imgCur);
		crPrv = pivot;
	}

	
}

CImageCtrl* CImageCtrl::GetInstance()
{
	if (m_pCimgCtrl == NULL)
		m_pCimgCtrl = new CImageCtrl();
	else
		return m_pCimgCtrl;
}
void CImageCtrl::DragPic(CxImage *imgCur, double del, CPoint point, CxImage *imgOrg, CRect &crPrv)
{

	if (imgOrg->IsValid() & crPrv.Height() == -1)
	{
		CRect rc(CPoint(0, 0), CSize(imgOrg->GetWidth()*(1 - del), imgOrg->GetHeight()*(1 - del)));
		
		int nl = (double)point.x * (del);
		int nt = (double)point.y * (del);
		rc.MoveToXY(nl, nt);
		//int nr = point.x + (double)(rc.right - point.x)*(1 - del);
		//int nb = point.y + (double)(rc.bottom - point.y)*(1 - del);
		CString s;
		//s.Format("%d,%d,%d,%d,%d, %d", nl, nt, nr, nb, rc.right, rc.bottom);
		//if (nl == rc.left | nt == rc.top | nr == rc.right | nb == rc.bottom)
		//return;
		imgOrg->Crop(rc, imgCur);
		crPrv = rc;
	}
	else
	{
		if (imgOrg->IsValid())
		{
			CString s;
			crPrv.MoveToX(crPrv.left + point.x*(1-del));
			if (point.x*(1 - del) <= 1 & point.x*(1 - del) >= -1)
				crPrv.MoveToX(crPrv.left + point.x);
			if (crPrv.right > imgOrg->GetWidth())
			{
				crPrv.left = imgOrg->GetWidth() - crPrv.Width();
				crPrv.right = imgOrg->GetWidth();
			}
			if (crPrv.left < 0)
			{
				crPrv.right = crPrv.Width();
				crPrv.left = 0;
			}
			crPrv.MoveToY(crPrv.top + point.y*(1 - del));
			if(point.y*(1 - del) <= 1 & point.y*(1 - del) >= -1)
				crPrv.MoveToY(crPrv.top + point.y);
			if (crPrv.bottom > imgOrg->GetHeight())
			{
				crPrv.top = imgOrg->GetHeight() - crPrv.Height();
				crPrv.bottom = imgOrg->GetHeight();
			}
			if (crPrv.top < 0)
			{
				crPrv.bottom = crPrv.Height();
				crPrv.top = 0;
			}
			s.Format("%d, %d", point.y, point.x);
			imgOrg->Crop(crPrv, imgCur);

		}
	}
}

void CImageCtrl::onDraw(CDC* pDC, CxImage *img)
{
	CRect rc;
	GetClientRect(pDC->GetWindow()->GetSafeHwnd(), &rc);
	if (img)
		img->Draw(pDC->GetSafeHdc(), GetCurRect(rc, img));
}

CRect CImageCtrl::GetCurRect(CRect rc, CxImage *img)
{
	int nHeight = img->GetHeight() * rc.Width() / img->GetWidth();
	int nWidth = img->GetWidth() * rc.Height() / img->GetHeight();
	if (nHeight <= rc.Height())
	{
		int dy = rc.Height() - nHeight;
		rc.bottom = nHeight;
		rc.MoveToY(dy / 2);
	}
	else
	{
		int dx = rc.Width() - nWidth;
		rc.right = nWidth;
		rc.MoveToX(dx / 2);
	}
	return rc;
}


BOOL CImageCtrl::TransPtToImg(CView *pView, CxImage *img, CPoint &pt, UINT nType)
{
	CRect crWnd;
	pView->GetWindowRect(crWnd);
	CRect cr;
	pView->GetClientRect(&cr);
	CRect crImg = GetCurRect(cr, img);
	CRect crImg1 = crImg;
	if (nType == 1)
		crImg.MoveToXY(crWnd.left + crImg.left, crWnd.top + crImg.top);
	if (crImg.PtInRect(pt))
	{
		pt.x -= crImg.left;
		pt.y -= crImg.top;

		pt.x = pt.x * img->GetWidth() / crImg.Width();
		pt.y = pt.y * img->GetHeight() / crImg.Height();
		return true;
	}
	return false;
}
