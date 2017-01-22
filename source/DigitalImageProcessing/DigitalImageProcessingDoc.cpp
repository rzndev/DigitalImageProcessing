
// DigitalImageProcessingDoc.cpp : ���������� ������ CDigitalImageProcessingDoc
//

#include "stdafx.h"
// SHARED_HANDLERS ����� ���������� � ������������ �������� ��������� ���������� ������� ATL, �������
// � ������; ��������� ��������� ������������ ��� ��������� � ������ �������.
#ifndef SHARED_HANDLERS
#include "DigitalImageProcessing.h"
#endif

#include "DigitalImageProcessingDoc.h"
#include "NegativeDataProcess.h"
#include <propkey.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CDigitalImageProcessingDoc

IMPLEMENT_DYNCREATE(CDigitalImageProcessingDoc, CDocument)

BEGIN_MESSAGE_MAP(CDigitalImageProcessingDoc, CDocument)
END_MESSAGE_MAP()


// ��������/����������� CDigitalImageProcessingDoc

CDigitalImageProcessingDoc::CDigitalImageProcessingDoc()
	: m_pProcessedImage(NULL)
	, m_pTotalImageProcess(NULL)

{
	// TODO: �������� ��� ��� ������������ ������ ������������
}

CDigitalImageProcessingDoc::~CDigitalImageProcessingDoc()
{

}

BOOL CDigitalImageProcessingDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: �������� ��� ��������� �������������
	// (��������� SDI ����� �������� ������������ ���� ��������)

	return TRUE;
}




// ������������ CDigitalImageProcessingDoc

void CDigitalImageProcessingDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: �������� ��� ����������
	}
	else
	{
		// TODO: �������� ��� ��������
	}
}

#ifdef SHARED_HANDLERS

// ��������� ��� �������
void CDigitalImageProcessingDoc::OnDrawThumbnail(CDC& dc, LPRECT lprcBounds)
{
	// �������� ���� ��� ��� ����������� ������ ���������
	dc.FillSolidRect(lprcBounds, RGB(255, 255, 255));

	CString strText = _T("TODO: implement thumbnail drawing here");
	LOGFONT lf;

	CFont* pDefaultGUIFont = CFont::FromHandle((HFONT) GetStockObject(DEFAULT_GUI_FONT));
	pDefaultGUIFont->GetLogFont(&lf);
	lf.lfHeight = 36;

	CFont fontDraw;
	fontDraw.CreateFontIndirect(&lf);

	CFont* pOldFont = dc.SelectObject(&fontDraw);
	dc.DrawText(strText, lprcBounds, DT_CENTER | DT_WORDBREAK);
	dc.SelectObject(pOldFont);
}

// ��������� ������������ ������
void CDigitalImageProcessingDoc::InitializeSearchContent()
{
	CString strSearchContent;
	// ������� ���������� ������ �� ������ ���������. 
	// ����� ����������� ������ ����������� ������ � ������� ";"

	// ��������:  strSearchContent = _T("�����;�������������;����;������ ole;");
	SetSearchContent(strSearchContent);
}

void CDigitalImageProcessingDoc::SetSearchContent(const CString& value)
{
	if (value.IsEmpty())
	{
		RemoveChunk(PKEY_Search_Contents.fmtid, PKEY_Search_Contents.pid);
	}
	else
	{
		CMFCFilterChunkValueImpl *pChunk = NULL;
		ATLTRY(pChunk = new CMFCFilterChunkValueImpl);
		if (pChunk != NULL)
		{
			pChunk->SetTextValue(PKEY_Search_Contents, value, CHUNK_TEXT);
			SetChunkValue(pChunk);
		}
	}
}

#endif // SHARED_HANDLERS

// ����������� CDigitalImageProcessingDoc

#ifdef _DEBUG
void CDigitalImageProcessingDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CDigitalImageProcessingDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// ������� CDigitalImageProcessingDoc


BOOL CDigitalImageProcessingDoc::OnOpenDocument(LPCTSTR lpszPathName)
{
	/*if (!CDocument::OnOpenDocument(lpszPathName))
		return FALSE;*/
	DeleteContents();
	BeginWaitCursor();
	if (false == m_LoadedImage.ReadDIBFile(lpszPathName))
	{
		// �������� ������� ���� � ������ ��������
		CImage img;
		if (S_OK==img.Load(lpszPathName))
		{
			img.Save(_T("temp.bmp"));
			m_LoadedImage.ReadDIBFile(_T("temp.bmp"));
		}
	}
	EndWaitCursor();
	UpdateAllViews(NULL);
	return TRUE;
}


void CDigitalImageProcessingDoc::OnCloseDocument()
{
	if (NULL != m_pProcessedImage)
	{
		delete m_pProcessedImage;
		m_pProcessedImage = NULL;
	}
	ClearProcessData();
	CDocument::OnCloseDocument();
}


// ���������� �����������
void CDigitalImageProcessingDoc::ProcessImage(void)
{
	if (NULL != m_pProcessedImage)
	{
		delete m_pProcessedImage;
		m_pProcessedImage = NULL;
	}
	CRect region;
	region.left = 0;
	region.top = 0;
	if ((m_LoadedImage.m_lWidth == 0) || (m_LoadedImage.m_lHeight == 0)) return; // ���� ����������� ����� �� ���������� ������� - �� ������������ ���
	region.right = m_LoadedImage.m_lWidth - 1;
	region.bottom = m_LoadedImage.m_lHeight - 1;
	
	m_pProcessedImage = m_LoadedImage.GetImageFromRegion(region);
	m_pProcessedImage->Process(m_pTotalImageProcess); // ������� ��������� ������� ��������� ��� ����� �����������
	// ������������ ������ ���������� ������ � ������������ � ��������� ��� ������� ����������� � ����� ���������
	vector<CDataProcess*>::iterator it_processData;
	vector<CRectTracker>::iterator it_track;
	double scale_x, scale_y; // ���������� ������������
	scale_x = (double)m_rectScale.cx / (double)m_LoadedImage.m_lWidth;
	scale_y = (double)m_rectScale.cy / (double)m_LoadedImage.m_lHeight;
	it_processData = m_regionProcess.begin();
	it_track = m_trackers.begin();
	while (true)
	{
		if (it_processData == m_regionProcess.end()) break;
		if (it_track == m_trackers.end()) break;
		// ���������� ������ ��� ���������
		CRect rect = it_track->m_rect;
		rect.bottom = (int)((double)rect.bottom / scale_y);
		rect.top = (int)((double)rect.top / scale_y);
		rect.left = (int)((double)rect.left / scale_x);
		rect.right = (int)((double)rect.right / scale_x);
		// ��������� ����������� ��� ���������
		CImageData* img = m_pProcessedImage->GetImageFromRegion(rect);
		// ������������ �������� �����������
		img->Process(*it_processData);
		// ������ ��������� � ������������ �����������
		m_pProcessedImage->CombineImage(img);
		// ����������� ������, ���������� ��� ��������� ��������� �����������
		delete img;
		it_processData++;
		it_track++;
	}
	UpdateAllViews(NULL);
}


// �������� �������������� ������
void CDigitalImageProcessingDoc::AddRegion(CRectTracker& tracker)
{
	CString str;
	m_trackers.push_back(tracker);
	CDataProcess *process = new CDataProcess();//CNegativeDataProcess();
	m_regionProcess.push_back(process); // ��� ����� ����������� �������� ������������� ��������� �� ����������, �� ���������� � ��������� ������� �����������
}


// ���������� ����� ���������� ������� �����������
void CDigitalImageProcessingDoc::ChangeMainDataProcess(CDataProcess* dataProcess)
{
	if (m_pTotalImageProcess == dataProcess) return; // ���� ����������� ��������� - ��������� �� ������
	if (m_pTotalImageProcess != NULL)
	{
		delete m_pTotalImageProcess;
	}
	m_pTotalImageProcess = dataProcess;
}


// �������� �������� �� ��������� ������
void CDigitalImageProcessingDoc::ClearProcessData(void)
{
	vector<CDataProcess*>::iterator it;
	for(it = m_regionProcess.begin(); it != m_regionProcess.end(); it++)
	{
		delete *it;
		*it = NULL;
	}
	m_regionProcess.clear();
	m_trackers.clear();
}


void CDigitalImageProcessingDoc::DeleteContents()
{
	// TODO: Add your specialized code here and/or call the base class
	ClearProcessData();
	CDocument::DeleteContents();
}


BOOL CDigitalImageProcessingDoc::OnSaveDocument(LPCTSTR lpszPathName)
{
	// ������ �������������� � ������� bmp. m_pProcessedImage
	if (NULL == m_pProcessedImage) return TRUE;
	if ((m_pProcessedImage->m_lHeight == 0)  || (m_pProcessedImage->m_lWidth == 0)) return TRUE;
	m_pProcessedImage->CreateDIBFile(lpszPathName);
	return TRUE;
	//return CDocument::OnSaveDocument(lpszPathName);
}
