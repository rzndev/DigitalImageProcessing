
// DigitalImageProcessingDoc.cpp : реализация класса CDigitalImageProcessingDoc
//

#include "stdafx.h"
// SHARED_HANDLERS можно определить в обработчиках фильтров просмотра реализации проекта ATL, эскизов
// и поиска; позволяет совместно использовать код документа в данным проекте.
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


// создание/уничтожение CDigitalImageProcessingDoc

CDigitalImageProcessingDoc::CDigitalImageProcessingDoc()
	: m_pProcessedImage(NULL)
	, m_pTotalImageProcess(NULL)

{
	// TODO: добавьте код для одноразового вызова конструктора
}

CDigitalImageProcessingDoc::~CDigitalImageProcessingDoc()
{

}

BOOL CDigitalImageProcessingDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: добавьте код повторной инициализации
	// (Документы SDI будут повторно использовать этот документ)

	return TRUE;
}




// сериализация CDigitalImageProcessingDoc

void CDigitalImageProcessingDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: добавьте код сохранения
	}
	else
	{
		// TODO: добавьте код загрузки
	}
}

#ifdef SHARED_HANDLERS

// Поддержка для эскизов
void CDigitalImageProcessingDoc::OnDrawThumbnail(CDC& dc, LPRECT lprcBounds)
{
	// Измените этот код для отображения данных документа
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

// Поддержка обработчиков поиска
void CDigitalImageProcessingDoc::InitializeSearchContent()
{
	CString strSearchContent;
	// Задайте содержимое поиска из данных документа. 
	// Части содержимого должны разделяться точкой с запятой ";"

	// Например:  strSearchContent = _T("точка;прямоугольник;круг;объект ole;");
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

// диагностика CDigitalImageProcessingDoc

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


// команды CDigitalImageProcessingDoc


BOOL CDigitalImageProcessingDoc::OnOpenDocument(LPCTSTR lpszPathName)
{
	/*if (!CDocument::OnOpenDocument(lpszPathName))
		return FALSE;*/
	DeleteContents();
	BeginWaitCursor();
	if (false == m_LoadedImage.ReadDIBFile(lpszPathName))
	{
		// пытаемся открыть файл в других форматах
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


// обработать изображение
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
	if ((m_LoadedImage.m_lWidth == 0) || (m_LoadedImage.m_lHeight == 0)) return; // Если изображение имеет не правильные размеры - не обрабатываем его
	region.right = m_LoadedImage.m_lWidth - 1;
	region.bottom = m_LoadedImage.m_lHeight - 1;
	
	m_pProcessedImage = m_LoadedImage.GetImageFromRegion(region);
	m_pProcessedImage->Process(m_pTotalImageProcess); // сначала применяем функцию обработки для всего изображения
	// обрабатываем каждый выделенный регион в соответствии с заданными для региона параметрами и видом обработки
	vector<CDataProcess*>::iterator it_processData;
	vector<CRectTracker>::iterator it_track;
	double scale_x, scale_y; // масштабные коэффициенты
	scale_x = (double)m_rectScale.cx / (double)m_LoadedImage.m_lWidth;
	scale_y = (double)m_rectScale.cy / (double)m_LoadedImage.m_lHeight;
	it_processData = m_regionProcess.begin();
	it_track = m_trackers.begin();
	while (true)
	{
		if (it_processData == m_regionProcess.end()) break;
		if (it_track == m_trackers.end()) break;
		// определяем регион для изменения
		CRect rect = it_track->m_rect;
		rect.bottom = (int)((double)rect.bottom / scale_y);
		rect.top = (int)((double)rect.top / scale_y);
		rect.left = (int)((double)rect.left / scale_x);
		rect.right = (int)((double)rect.right / scale_x);
		// формируем изображение для обработки
		CImageData* img = m_pProcessedImage->GetImageFromRegion(rect);
		// обрабатываем фрагмент изображения
		img->Process(*it_processData);
		// вносим изменения в обработанное изображение
		m_pProcessedImage->CombineImage(img);
		// освобождаем память, выделенную для обработки фрагмента изображения
		delete img;
		it_processData++;
		it_track++;
	}
	UpdateAllViews(NULL);
}


// добавить обрабатываемый регион
void CDigitalImageProcessingDoc::AddRegion(CRectTracker& tracker)
{
	CString str;
	m_trackers.push_back(tracker);
	CDataProcess *process = new CDataProcess();//CNegativeDataProcess();
	m_regionProcess.push_back(process); // для вновь добавленных регионов устанавливаем указатель на обработчик, не приводящий к изменению яркости изображения
}


// установить новый обработчик яркости изображения
void CDigitalImageProcessingDoc::ChangeMainDataProcess(CDataProcess* dataProcess)
{
	if (m_pTotalImageProcess == dataProcess) return; // если обработчики совпадают - изменений не вносим
	if (m_pTotalImageProcess != NULL)
	{
		delete m_pTotalImageProcess;
	}
	m_pTotalImageProcess = dataProcess;
}


// Очистить сведения об обработке данных
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
	// запись осуществляется в формате bmp. m_pProcessedImage
	if (NULL == m_pProcessedImage) return TRUE;
	if ((m_pProcessedImage->m_lHeight == 0)  || (m_pProcessedImage->m_lWidth == 0)) return TRUE;
	m_pProcessedImage->CreateDIBFile(lpszPathName);
	return TRUE;
	//return CDocument::OnSaveDocument(lpszPathName);
}
