
// DigitalImageProcessingDoc.h : ��������� ������ CDigitalImageProcessingDoc
//


#pragma once
#include <vector>
#include "imagedata.h"

#include "DataProcess.h"
#include "atltypes.h"

using namespace std;

class CDigitalImageProcessingDoc : public CDocument
{
protected: // ������� ������ �� ������������
	CDigitalImageProcessingDoc();
	DECLARE_DYNCREATE(CDigitalImageProcessingDoc)

// ��������
public:

// ��������
public:

// ���������������
public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
#ifdef SHARED_HANDLERS
	virtual void InitializeSearchContent();
	virtual void OnDrawThumbnail(CDC& dc, LPRECT lprcBounds);
#endif // SHARED_HANDLERS

// ����������
public:
	virtual ~CDigitalImageProcessingDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// ��������� ������� ����� ���������
protected:
	DECLARE_MESSAGE_MAP()

#ifdef SHARED_HANDLERS
	// ��������������� �������, �������� ���������� ������ ��� ����������� ������
	void SetSearchContent(const CString& value);
#endif // SHARED_HANDLERS
public:
	// ������ ��������� ��������
	vector<CRectTracker> m_trackers;
	// �������� �������������� �������
	CDataProcess* m_pTotalImageProcess;

	//CRectTracker m_tracker;
	virtual BOOL OnOpenDocument(LPCTSTR lpszPathName);
	// ����������� �����������
	CImageData m_LoadedImage;
	virtual void OnCloseDocument();
	// ������������ �����������
	CImageData* m_pProcessedImage;
	// ���������� �����������
	void ProcessImage(void);
	// �������� �������������� ������
	void AddRegion(CRectTracker& tracker);
	// ������ �������, ��������������� ��������� ���������� ��������
	vector<CDataProcess*> m_regionProcess;
	// ���������� ����� ���������� ������� �����������
	void ChangeMainDataProcess(CDataProcess* dataProcess);
	// �������� �������� �� ��������� ������
	void ClearProcessData(void);
	// ������ ������������� �����������. ������������ ��� ����������� ���������� �������������, ����������� � ���������� ��������
	CSize m_rectScale;
	virtual void DeleteContents();
	virtual BOOL OnSaveDocument(LPCTSTR lpszPathName);
};
