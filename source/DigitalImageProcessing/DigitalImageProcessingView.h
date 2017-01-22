
// DigitalImageProcessingView.h : интерфейс класса CDigitalImageProcessingView
//

#pragma once

#include "resource.h"
#include "ImageDataDisplay.h"
#include "buildgraphic.h"
#include "afxwin.h"

class CDigitalImageProcessingView : public CFormView
{
protected: // создать только из сериализации
	CDigitalImageProcessingView();
	DECLARE_DYNCREATE(CDigitalImageProcessingView)

public:
	enum{ IDD = IDD_DIGITALIMAGEPROCESSING_FORM };

// Атрибуты
public:
	CDigitalImageProcessingDoc* GetDocument() const;

// Операции
public:

// Переопределение
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // поддержка DDX/DDV
	virtual void OnInitialUpdate(); // вызывается в первый раз после конструктора

// Реализация
public:
	virtual ~CDigitalImageProcessingView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Созданные функции схемы сообщений
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	virtual void OnUpdate(CView* /*pSender*/, LPARAM /*lHint*/, CObject* /*pHint*/);
	CImageDataDisplay m_ctrlImage; // исходное изображение с возможностью выбора регионов
	// Обработанное изображение
	CImageData* m_pProcessedImage;
	CImageDataDisplay m_ctrlDestImage;
	CBuildGraphic m_buildGraphic;
	// Список параметров обработки изображения
	CListBox m_procList;
	afx_msg void OnBnClickedButtonProcess();
	afx_msg void OnBnClickedButtonParameters();
	// Заполнить список способов обработки изображения
	void fillProcessList(void);
	// Отобразить график для функции преобразования выбранного региона
	void BuildGraph(void);
	afx_msg void OnLbnSelchangeList1();
	afx_msg void OnBnClickedButtonTheory();
	// Получить раздел помощи в зависимости от выбора
	CString GetHelpName(int val);
};

#ifndef _DEBUG  // отладочная версия в DigitalImageProcessingView.cpp
inline CDigitalImageProcessingDoc* CDigitalImageProcessingView::GetDocument() const
   { return reinterpret_cast<CDigitalImageProcessingDoc*>(m_pDocument); }
#endif

