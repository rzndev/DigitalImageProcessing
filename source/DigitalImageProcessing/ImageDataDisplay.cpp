#include "StdAfx.h"
#include "ImageDataDisplay.h"
#include "DigitalImageProcessingDoc.h"

#define IMAGEDATADISPLAY_CLASSNAME    _T("CImageDataDisplay")  // Window class name


CImageDataDisplay::CImageDataDisplay(void)
	: m_image(NULL)
	,m_pView(NULL)
	,m_bSelectRectangle(FALSE)
{
	RegisterWindowClass();
}


CImageDataDisplay::~CImageDataDisplay(void)
{
}

static void SetNormalRect(CRect& rect, int left, int top, int width, int height)
{
	// set it
	rect.left = left;
	rect.top = top;
	rect.right = left + width;
	rect.bottom = top + height;

	// normalize it
	int nTemp;
	if (rect.left > rect.right)
	{
		nTemp = rect.left;
		rect.left = rect.right;
		rect.right = nTemp;
	}
	if (rect.top > rect.bottom)
	{
		nTemp = rect.top;
		rect.top = rect.bottom;
		rect.bottom = nTemp;
	}
}


void CImageDataDisplay::DisplayImage(CImageData* data)
{
	m_image = data;
	Invalidate();
}

BEGIN_MESSAGE_MAP(CImageDataDisplay, CStatic)
	ON_WM_PAINT()
	ON_WM_LBUTTONDOWN()
	ON_WM_SETCURSOR()
	ON_WM_DESTROY()
END_MESSAGE_MAP()


//void CImageDataDisplay::OnPaint()
//{
//	CPaintDC dc(this); // device context for painting
//	BOOL     bSuccess=FALSE;      // Success/fail flag
//	// TODO: Add your message handler code here
//	// Do not call CStatic::OnPaint() for painting messages
//	CRect rcDest;
//	GetClientRect(&rcDest);
//	if ((m_image != NULL) && (m_image->m_lHeight != 0))
//	{
//		m_image->CreateBitmap(); // подготавливаем изображение к отображению
//		//m_image->CreateBitmap(CSize(rcDest.Width(), rcDest.Height())); // подготавливаем изображение к отображению
//		
//		CRect rcDIB;
//		rcDIB.top = rcDIB.left = 0;
//		rcDIB.right = m_image->m_lWidth;
//		rcDIB.bottom = m_image->m_lHeight;
//		
//		 bSuccess = ::StretchDIBits(dc,                          // hDC
//							   rcDest.left,                 // DestX
//							   rcDest.top,                  // DestY
//							   rcDest.Width(),            // nDestWidth
//							   rcDest.Height(),           // nDestHeight
//							   rcDIB.left,                // SrcX
//							   rcDIB.top,                 // SrcY
//							   rcDIB.Width(),           // wSrcWidth
//							   rcDIB.Height(),          // wSrcHeight
//							   m_image->m_pBits,                      // lpBits
//							   &m_image->m_bitmapinfo,         // lpBitsInfo
//							   DIB_RGB_COLORS,                 // wUsage
//							   SRCCOPY);                       // dwROP
//	} else
//	{
//		HGDIOBJ pOldPen = NULL;
//		HGDIOBJ pOldBrush = NULL;
//		pOldPen = dc.SelectObject(GetStockObject(BLACK_PEN));
//		pOldBrush = dc.SelectObject(GetStockObject(NULL_BRUSH));
//		dc.Rectangle(rcDest);
//		dc.SelectObject(pOldBrush);
//		dc.SelectObject(pOldPen);
//	}
//	PaintRegions(&dc);
//}

void CImageDataDisplay::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	BOOL     bSuccess=FALSE;      // Success/fail flag
	// TODO: Add your message handler code here
	// Do not call CStatic::OnPaint() for painting messages
	CRect rcDest;
	GetClientRect(&rcDest);
	if ((m_image != NULL) && (m_image->m_lHeight != 0))
	{
		
		if (false == m_image->CreateSizedBitmap(CSize(rcDest.Width(), rcDest.Height()), &dc)) // подготавливаем изображение к отображению и отображаем его
		{
			HGDIOBJ pOldPen = NULL;
			HGDIOBJ pOldBrush = NULL;
			pOldPen = dc.SelectObject(GetStockObject(BLACK_PEN));
			pOldBrush = dc.SelectObject((HBRUSH) (COLOR_3DFACE + 1));
			dc.Rectangle(rcDest);
			dc.SelectObject(pOldBrush);
			dc.SelectObject(pOldPen);
		}
		
	} else
	{
		HGDIOBJ pOldPen = NULL;
		HGDIOBJ pOldBrush = NULL;
		pOldPen = dc.SelectObject(GetStockObject(BLACK_PEN));
		pOldBrush = dc.SelectObject((HBRUSH) (COLOR_3DFACE + 1));
		dc.Rectangle(rcDest);
		dc.SelectObject(pOldBrush);
		dc.SelectObject(pOldPen);
	}
	PaintRegions(&dc);
}

void CImageDataDisplay::OnLButtonDown(UINT nFlags, CPoint point)
{
	vector<CRectTracker>::iterator it;
	bool bFound = false;
	if ((NULL == m_pView) || (m_bSelectRectangle == FALSE))
	{
		CStatic::OnLButtonDown(nFlags, point);
		return;
	}

	ASSERT(m_pView->GetDocument()->IsKindOf(RUNTIME_CLASS(CDigitalImageProcessingDoc)));
	CDigitalImageProcessingDoc* pDoc = (CDigitalImageProcessingDoc*)m_pView->GetDocument();
	CRect rectSave;

	for(it = pDoc->m_trackers.begin(); it != pDoc->m_trackers.end(); it++)
	{
		if (it->HitTest(point) >= 0)
		{
			bFound = true; // один из регионов совпал с точкой, в которой находится указатель мыши
			if (it->Track(this, point, FALSE))
			{
				it->GetTrueRect(rectSave);
				pDoc->SetModifiedFlag();
				pDoc->UpdateAllViews(NULL, (LPARAM)(LPCRECT)rectSave);
				pDoc->UpdateAllViews(NULL);
			}
			break;
		}
	}

	if (false == bFound)
	{
		CRectTracker tracker;
		if (tracker.TrackRubberBand(this, point, TRUE))
		{
			CRect rectT;
			tracker.m_rect.NormalizeRect();
			bFound = false;
			//for(it = pDoc->m_trackers.begin(); it != pDoc->m_trackers.end(); it++)
			//{
			//	if (rectT.IntersectRect(tracker.m_rect, it->m_rect))
			//	{
			//		// if so, put resize handles on it (ie. select it)
			//		if (it->m_nStyle & CRectTracker::resizeInside)
			//		{
			//			// swap from resize inside to resize outside for effect
			//			it->m_nStyle &= ~CRectTracker::resizeInside;
			//			it->m_nStyle |= CRectTracker::resizeOutside;
			//		}
			//		else
			//		{
			//			// just use inside resize handles on first time
			//			it->m_nStyle &= ~CRectTracker::resizeOutside;
			//			it->m_nStyle |= CRectTracker::resizeInside;
			//		}
			//		
			//		pDoc->SetModifiedFlag();
			//		pDoc->UpdateAllViews(NULL, (LPARAM)(LPCRECT)rectSave);
			//		pDoc->UpdateAllViews(NULL);
			//		bFound = true;
			//	}
			//} 
			if (false == bFound)
			{
				pDoc->AddRegion(tracker);
				pDoc->SetModifiedFlag();
				pDoc->UpdateAllViews(NULL, (LPARAM)(LPCRECT)rectSave);
				pDoc->UpdateAllViews(NULL);
			}
		}
	}
	CStatic::OnLButtonDown(nFlags, point);
}





// Отобразить выедленные рамки выделенных областей изображения
void CImageDataDisplay::PaintRegions(CDC* pDC)
{
	int cnt=0;
	vector<CRectTracker>::iterator it;
	if ((NULL == m_pView) || (m_bSelectRectangle == FALSE)) return;

	ASSERT(m_pView->GetDocument()->IsKindOf(RUNTIME_CLASS(CDigitalImageProcessingDoc)));
	CDigitalImageProcessingDoc* pDoc = (CDigitalImageProcessingDoc*)m_pView->GetDocument();

	for(it = pDoc->m_trackers.begin(); it != pDoc->m_trackers.end(); it++)
	{
		cnt++;
		CString fmt;
		HGDIOBJ hBrushObj = NULL;
		CPen *pOldPen = NULL;
		TRY
		{
			// draw inside in various colors
			//CBrush brush1, brush2;
			CPen redPen;
			CRect rect;
			int nWidth = it->m_rect.Width();
			int nHeight = it->m_rect.Height();
			int nSgnX = nWidth != 0 ? nWidth / abs(nWidth) : 1;
			int nSgnY = nHeight != 0 ? nHeight / abs(nHeight) : 1;
			pDC->SetTextAlign(TA_CENTER);
			pDC->SetBkMode(TRANSPARENT);
			int nCenterX, nCenterY;

			TEXTMETRIC tm;
			pDC->GetTextMetrics(&tm);

			//brush1.CreateSolidBrush(RGB(255, 0, 0));
			redPen.CreatePen(PS_SOLID,1,RGB(255,0,0));
			//pOldBrush = pDC->SelectObject(&brush1);
			pOldPen = pDC->SelectObject(&redPen);
			hBrushObj = pDC->SelectObject(GetStockObject(NULL_BRUSH));
			SetNormalRect(rect, it->m_rect.left,
				it->m_rect.top, nWidth, nHeight);

			pDC->Rectangle(rect.left, rect.top, rect.right, rect.bottom);
			nCenterX = rect.left + rect.Width()/2;
			nCenterY = rect.top + rect.Height()/2 - tm.tmHeight/2;
			fmt.Format(_T("%d"),cnt);
			COLORREF bkColor = pDC->SetTextColor(RGB(255,255,255));
			pDC->ExtTextOut(nCenterX, nCenterY, ETO_CLIPPED, rect, fmt, 1, NULL);
			pDC->SetTextColor(bkColor);
		
			// совобождаем контекст DC
			pDC->SelectObject(hBrushObj);
			if (pOldPen != NULL)
				pDC->SelectObject(pOldPen);
		
			// draw tracker on outside
			it->Draw(pDC);
		}
		CATCH_ALL(e)
		{
			pDC->SelectObject(hBrushObj);
			if (pOldPen != NULL)
				pDC->SelectObject(pOldPen);
		}
		END_CATCH_ALL
	}
}


BOOL CImageDataDisplay::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message)
{
	// forward to tracker
	if ((m_pView != NULL) && (m_bSelectRectangle == TRUE))
	{
		ASSERT(m_pView->GetDocument()->IsKindOf(RUNTIME_CLASS(CDigitalImageProcessingDoc)));
		CDigitalImageProcessingDoc* pDoc = (CDigitalImageProcessingDoc*)m_pView->GetDocument();
		vector<CRectTracker>::iterator it;
		for(it = pDoc->m_trackers.begin(); it != pDoc->m_trackers.end(); it++)
		{
			if (pWnd == this && it->SetCursor(this, nHitTest))
				return TRUE;
		}
	}
	return CStatic::OnSetCursor(pWnd, nHitTest, message);
}


void CImageDataDisplay::SetView(CView* pView)
{
	m_pView = pView;
}


void CImageDataDisplay::OnDestroy()
{
	CStatic::OnDestroy();

	// TODO: Add your message handler code here
}


BOOL CImageDataDisplay::RegisterWindowClass(void)
{
    WNDCLASS wndcls;
    HINSTANCE hInst = AfxGetInstanceHandle();

    if (!(::GetClassInfo(hInst, IMAGEDATADISPLAY_CLASSNAME, &wndcls)))
    {
        // otherwise we need to register a new class
        wndcls.style            = CS_DBLCLKS | CS_HREDRAW | CS_VREDRAW;
        wndcls.lpfnWndProc      = ::DefWindowProc;
        wndcls.cbClsExtra       = wndcls.cbWndExtra = 0;
        wndcls.hInstance        = hInst;
        wndcls.hIcon            = NULL;
        wndcls.hCursor          = AfxGetApp()->LoadStandardCursor(IDC_ARROW);
        wndcls.hbrBackground    = NULL;//(HBRUSH) (COLOR_3DFACE + 1);
        wndcls.lpszMenuName     = NULL;
        wndcls.lpszClassName    = IMAGEDATADISPLAY_CLASSNAME;

        if (!AfxRegisterClass(&wndcls))
        {
            AfxThrowResourceException();
            return FALSE;
        }
    }

    return TRUE;

}






