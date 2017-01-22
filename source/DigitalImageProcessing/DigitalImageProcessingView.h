
// DigitalImageProcessingView.h : ��������� ������ CDigitalImageProcessingView
//

#pragma once

#include "resource.h"
#include "ImageDataDisplay.h"
#include "buildgraphic.h"
#include "afxwin.h"

class CDigitalImageProcessingView : public CFormView
{
protected: // ������� ������ �� ������������
	CDigitalImageProcessingView();
	DECLARE_DYNCREATE(CDigitalImageProcessingView)

public:
	enum{ IDD = IDD_DIGITALIMAGEPROCESSING_FORM };

// ��������
public:
	CDigitalImageProcessingDoc* GetDocument() const;

// ��������
public:

// ���������������
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // ��������� DDX/DDV
	virtual void OnInitialUpdate(); // ���������� � ������ ��� ����� ������������

// ����������
public:
	virtual ~CDigitalImageProcessingView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// ��������� ������� ����� ���������
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	virtual void OnUpdate(CView* /*pSender*/, LPARAM /*lHint*/, CObject* /*pHint*/);
	CImageDataDisplay m_ctrlImage; // �������� ����������� � ������������ ������ ��������
	// ������������ �����������
	CImageData* m_pProcessedImage;
	CImageDataDisplay m_ctrlDestImage;
	CBuildGraphic m_buildGraphic;
	// ������ ���������� ��������� �����������
	CListBox m_procList;
	afx_msg void OnBnClickedButtonProcess();
	afx_msg void OnBnClickedButtonParameters();
	// ��������� ������ �������� ��������� �����������
	void fillProcessList(void);
	// ���������� ������ ��� ������� �������������� ���������� �������
	void BuildGraph(void);
	afx_msg void OnLbnSelchangeList1();
	afx_msg void OnBnClickedButtonTheory();
	// �������� ������ ������ � ����������� �� ������
	CString GetHelpName(int val);
};

#ifndef _DEBUG  // ���������� ������ � DigitalImageProcessingView.cpp
inline CDigitalImageProcessingDoc* CDigitalImageProcessingView::GetDocument() const
   { return reinterpret_cast<CDigitalImageProcessingDoc*>(m_pDocument); }
#endif

