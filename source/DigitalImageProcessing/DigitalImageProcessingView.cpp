
// DigitalImageProcessingView.cpp : ���������� ������ CDigitalImageProcessingView
//

#include "stdafx.h"
// SHARED_HANDLERS ����� ���������� � ������������ �������� ��������� ���������� ������� ATL, �������
// � ������; ��������� ��������� ������������ ��� ��������� � ������ �������.
#ifndef SHARED_HANDLERS
#include "DigitalImageProcessing.h"
#endif

#include "DigitalImageProcessingDoc.h"
#include "DigitalImageProcessingView.h"
#include "SelectProcess.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CDigitalImageProcessingView

IMPLEMENT_DYNCREATE(CDigitalImageProcessingView, CFormView)

BEGIN_MESSAGE_MAP(CDigitalImageProcessingView, CFormView)
//	ON_WM_LBUTTONDOWN()
//	ON_WM_SETCURSOR()
	ON_BN_CLICKED(IDC_BUTTON_PROCESS, &CDigitalImageProcessingView::OnBnClickedButtonProcess)
	ON_BN_CLICKED(IDC_BUTTON_PARAMETERS, &CDigitalImageProcessingView::OnBnClickedButtonParameters)
	ON_LBN_SELCHANGE(IDC_LIST1, &CDigitalImageProcessingView::OnLbnSelchangeList1)
	ON_BN_CLICKED(IDC_BUTTON_THEORY, &CDigitalImageProcessingView::OnBnClickedButtonTheory)
END_MESSAGE_MAP()

// ��������/����������� CDigitalImageProcessingView

CDigitalImageProcessingView::CDigitalImageProcessingView()
	: CFormView(CDigitalImageProcessingView::IDD)
	, m_pProcessedImage(NULL)
{
	// TODO: �������� ��� ��������

}

CDigitalImageProcessingView::~CDigitalImageProcessingView()
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

void CDigitalImageProcessingView::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_SOURCE_IMAGE, m_ctrlImage);
	DDX_Control(pDX, IDC_DESTINATION_IMAGE, m_ctrlDestImage);
	DDX_Control(pDX, IDC_CUSTOM2, m_buildGraphic);
	DDX_Control(pDX, IDC_LIST1, m_procList);
}

BOOL CDigitalImageProcessingView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: �������� ����� Window ��� ����� ����������� ���������
	//  CREATESTRUCT cs

	return CFormView::PreCreateWindow(cs);
}

void CDigitalImageProcessingView::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();
	GetParentFrame()->RecalcLayout();
	ResizeParentToFit();
	
	m_ctrlImage.SetView(this);
	m_ctrlImage.m_bSelectRectangle = TRUE;

	CDigitalImageProcessingDoc* pDoc = GetDocument();
	CRect rect;
	m_ctrlImage.GetClientRect(&rect);
	pDoc->m_rectScale = CSize(rect.Width(), rect.Height());
	fillProcessList();
}


// ����������� CDigitalImageProcessingView

#ifdef _DEBUG
void CDigitalImageProcessingView::AssertValid() const
{
	CFormView::AssertValid();
}

void CDigitalImageProcessingView::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}

CDigitalImageProcessingDoc* CDigitalImageProcessingView::GetDocument() const 
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CDigitalImageProcessingDoc)));
	return (CDigitalImageProcessingDoc*)m_pDocument;
}
#endif //_DEBUG


// ����������� ��������� CDigitalImageProcessingView




void CDigitalImageProcessingView::OnUpdate(CView* /*pSender*/, LPARAM lHint, CObject* /*pHint*/)
{
	CDigitalImageProcessingDoc* pDoc = GetDocument();
	if (lHint == 0)
	{
		CRect rectTrue;
		CDigitalImageProcessingDoc* pDoc = GetDocument();

		//InvalidateRect(rectTrue);
	}
	else
	{
		InvalidateRect((CRect*)lHint);
	}	
	m_ctrlImage.DisplayImage(&pDoc->m_LoadedImage);
	m_ctrlDestImage.DisplayImage(pDoc->m_pProcessedImage);
	fillProcessList();
}





void CDigitalImageProcessingView::OnBnClickedButtonProcess()
{
	CDigitalImageProcessingDoc* pDoc = GetDocument();
	pDoc->ProcessImage();
}


void CDigitalImageProcessingView::OnBnClickedButtonParameters()
{
	// ���������� ���������� ��������
	CDigitalImageProcessingDoc* pDoc = GetDocument();
	CSelectProcess dlg;
	int selected = m_procList.GetCurSel();
	if (LB_ERR == selected)
	{
		//dlg.SetCurrentProcess(NULL);
		//if (IDOK == dlg.DoModal())
		//{
		//	delete dlg.m_pParamsProcess;
		//} else
		//{
		//	delete dlg.m_pParamsProcess;
		//}
		m_buildGraphic.setDataProcess(NULL);
		return;
	}
	if (0 == selected)
	{
		dlg.SetCurrentProcess(pDoc->m_pTotalImageProcess);
		if (IDOK == dlg.DoModal())
		{
			pDoc->m_pTotalImageProcess = dlg.m_pParamsProcess;
		} else
		{
			delete dlg.m_pParamsProcess;
		}
		m_buildGraphic.setDataProcess(pDoc->m_pTotalImageProcess); // ���������� ������
		fillProcessList(); // ��������� ������ ������� ���������
		return;
	}
	selected--;
	int cnt = 0;
	vector<CDataProcess*>::iterator it;
	for(it = pDoc->m_regionProcess.begin(); it != pDoc->m_regionProcess.end(); it++)
	{
		if (selected == cnt) 
		{
			dlg.SetCurrentProcess(*it);
			if (IDOK == dlg.DoModal())
			{
				*it = dlg.m_pParamsProcess;
			} else
			{
				delete dlg.m_pParamsProcess;
			}
			m_buildGraphic.setDataProcess(*it); // ���������� ������
			fillProcessList();                  // ��������� ������ ������� ���������
			break;
		}
		cnt++;
	}
}


// ��������� ������ �������� ��������� �����������
void CDigitalImageProcessingView::fillProcessList(void)
{
	CString str;
	CDigitalImageProcessingDoc* pDoc = GetDocument();
	int selected = m_procList.GetCurSel();
	m_procList.ResetContent();
	if (NULL == pDoc->m_pTotalImageProcess)
		m_procList.AddString(_T("�������� �����������. ��� ���������."));
	else
	{
		str.Format(_T("�������� �����������. %s."), pDoc->m_pTotalImageProcess->GetInfo());
		m_procList.AddString(str);
	}
	int cnt=0;
	vector<CDataProcess*>::iterator it;
	for(it = pDoc->m_regionProcess.begin(); it != pDoc->m_regionProcess.end(); it++)
	{
		cnt++;
		if (*it == NULL)
		{
			str.Format(_T("������� %d. ��� ���������."), cnt);
		} else
		{
			str.Format(_T("������� %d. %s."), cnt, (*it)->GetInfo());
			m_procList.AddString(str);
		}
	}
	if (selected > cnt)
		selected = LB_ERR;
	m_procList.SetCurSel(selected);
}


// ���������� ������ ��� ������� �������������� ���������� �������
void CDigitalImageProcessingView::BuildGraph(void)
{
	CDigitalImageProcessingDoc* pDoc = GetDocument();
	int selected = m_procList.GetCurSel();
	if (LB_ERR == selected)
	{
		m_buildGraphic.setDataProcess(NULL);
		return;
	}
	if (0 == selected)
	{
		m_buildGraphic.setDataProcess(pDoc->m_pTotalImageProcess);
		return;
	}
	selected--;
	int cnt = 0;
	vector<CDataProcess*>::iterator it;
	for(it = pDoc->m_regionProcess.begin(); it != pDoc->m_regionProcess.end(); it++)
	{
		if (selected == cnt) 
		{
			m_buildGraphic.setDataProcess(*it);
			break;
		}
		cnt++;
	}
}


void CDigitalImageProcessingView::OnLbnSelchangeList1()
{
	BuildGraph();
}


void CDigitalImageProcessingView::OnBnClickedButtonTheory()
{
	CString file = _T("digitalimgproc.chm::/Overview.htm"); 
	CDigitalImageProcessingDoc* pDoc = GetDocument();
	int selected = m_procList.GetCurSel();
	if (LB_ERR == selected)
	{
		m_buildGraphic.setDataProcess(NULL);
		HWND hwnd =
    ::HtmlHelp(
              ::GetDesktopWindow(),
              file,
              HH_DISPLAY_TOPIC,
              NULL) ;
		return;
	}
	if (0 == selected)
	{
		file = GetHelpName((pDoc->m_pTotalImageProcess != NULL) ? pDoc->m_pTotalImageProcess->m_typeProcess : 0); 
		
	} else
	{
		selected--;
		int cnt = 0;
		vector<CDataProcess*>::iterator it;
		for(it = pDoc->m_regionProcess.begin(); it != pDoc->m_regionProcess.end(); it++)
		{
			if (selected == cnt) 
			{
				file = GetHelpName(((*it) != NULL) ? (*it)->m_typeProcess : 0);
				break;
			}
			cnt++;
		}
	}
	HWND hwnd =
    ::HtmlHelp(
              ::GetDesktopWindow(),
              file,
              HH_DISPLAY_TOPIC,
              NULL) ;

}


// �������� ������ ������ � ����������� �� ������
CString CDigitalImageProcessingView::GetHelpName(int val)
{
	switch(val)
			{
				case 0:
					return _T("digitalimgproc.chm::/Overview.htm");
					break;
				case 1:
					return _T("digitalimgproc.chm::/Neg.htm");
					break;
				case 2:
					return _T("digitalimgproc.chm::/Log.htm");
					break;
				case 3:
					return _T("digitalimgproc.chm::/Pow.htm");
					break;
				case 4:
					return _T("digitalimgproc.chm::/Linear.htm");
					break;
		}
	return _T("digitalimgproc.chm::/Overview.htm");
}
