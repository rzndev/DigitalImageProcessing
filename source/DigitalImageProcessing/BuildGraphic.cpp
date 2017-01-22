// BuildGraphic.cpp : implementation file
//

#include "stdafx.h"
#include "DigitalImageProcessing.h"
#include "BuildGraphic.h"

#define BUILDGRAPHIC_CLASSNAME    _T("CBuildGraphic")  // Window class name

// CBuildGraphic

IMPLEMENT_DYNAMIC(CBuildGraphic, CWnd)

CBuildGraphic::CBuildGraphic() : m_dataProcess(NULL)
{
	RegisterWindowClass();
}

CBuildGraphic::~CBuildGraphic()
{
}


BEGIN_MESSAGE_MAP(CBuildGraphic, CWnd)
	ON_WM_PAINT()
END_MESSAGE_MAP()



// CBuildGraphic message handlers




void CBuildGraphic::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: Add your message handler code here
	// Do not call CWnd::OnPaint() for painting messages
	PaintGrid(&dc);
	PaintGraph(&dc);
}


// Построить сетку графика
void CBuildGraphic::PaintGrid(CDC* pDC)
{
	const int nVerticalLines = 10; // количество вертикальных делений
	const int nHorizontalLines = 10; // количество горизонтальных делений
	int i;
	CRect rcDest;
	GetClientRect(&rcDest);
	HGDIOBJ pOldPen = NULL;
	HGDIOBJ pOldBrush = NULL;
	pOldPen = pDC->SelectObject(GetStockObject(BLACK_PEN));
	pOldBrush = pDC->SelectObject(GetStockObject(NULL_BRUSH));
	pDC->Rectangle(rcDest); // отображаем границу графика
	// отображаем вертикальные линии сетки
	double grid_step = rcDest.right / 10;
	for(i=1; i < nVerticalLines; i++)
	{
		pDC->MoveTo(int(grid_step * (double)i),0);
		pDC->LineTo(int(grid_step * (double)i),rcDest.bottom);
	}
	// отображаем горизонтальные линии сетки
	grid_step = rcDest.bottom / 10;
	for(i=1; i < nVerticalLines; i++)
	{
		pDC->MoveTo(0,int(grid_step * (double)i));
		pDC->LineTo(rcDest.right,int(grid_step * (double)i));
	}
	pDC->SelectObject(pOldBrush);
	pDC->SelectObject(pOldPen);
}


// Построить график функции
void CBuildGraphic::PaintGraph(CDC* pDC)
{
	double x,y;              // аргумент и значение функции
	int window_x, window_y;  // координаты точки, соответствующие аргументу и значению функции
	CRect rcDest;
	CPen redPen;
	GetClientRect(&rcDest);

	if (m_dataProcess == NULL) return;
	y = m_dataProcess->Process(0.0);
	window_x = 0;
	if (y > 255.0) y = 255.0;
	if (y < 0.0) y = 0.0;
	window_y = rcDest.bottom - (int)((y/255.0) * ((double)rcDest.bottom));
	redPen.CreatePen(PS_SOLID,1,RGB(255,0,0));
	CPen* pOldPen = pDC->SelectObject(&redPen);
	pDC->MoveTo(window_x,window_y);
	for(x = 1; x < 256; x++)
	{
		window_x = (int)((x/255.0) * ((double)rcDest.right));
		y = m_dataProcess->Process(x);
		if (y > 255.0) y = 255.0;
		if (y < 0.0) y = 0.0;
		window_y = rcDest.bottom - (int)((y/255.0) * ((double)rcDest.bottom));
		pDC->LineTo(window_x,window_y);
	}
	pDC->SelectObject(pOldPen);
}


BOOL CBuildGraphic::RegisterWindowClass(void)
{
	    WNDCLASS wndcls;
    HINSTANCE hInst = AfxGetInstanceHandle();

    if (!(::GetClassInfo(hInst, BUILDGRAPHIC_CLASSNAME, &wndcls)))
    {
        // otherwise we need to register a new class
        wndcls.style            = CS_DBLCLKS | CS_HREDRAW | CS_VREDRAW;
        wndcls.lpfnWndProc      = ::DefWindowProc;
        wndcls.cbClsExtra       = wndcls.cbWndExtra = 0;
        wndcls.hInstance        = hInst;
        wndcls.hIcon            = NULL;
        wndcls.hCursor          = AfxGetApp()->LoadStandardCursor(IDC_ARROW);
        wndcls.hbrBackground    = (HBRUSH) (COLOR_3DFACE + 1);
        wndcls.lpszMenuName     = NULL;
        wndcls.lpszClassName    = BUILDGRAPHIC_CLASSNAME;

        if (!AfxRegisterClass(&wndcls))
        {
            AfxThrowResourceException();
            return FALSE;
        }
    }

    return TRUE;
}


// установить класс функции, для которой строится график
void CBuildGraphic::setDataProcess(CDataProcess* dataProcess)
{
	m_dataProcess = dataProcess;
	Invalidate();
}
