#pragma once
#include "afxwin.h"
#include "ImageData.h"
#include <vector>
using namespace std;

class CImageDataDisplay :
	public CStatic
{
public:
	CImageDataDisplay(void);
	virtual ~CImageDataDisplay(void);
	// Отображаемые данные
	void DisplayImage(CImageData* data);
	DECLARE_MESSAGE_MAP()
	afx_msg void OnPaint();
public:
	// признак необходимости выбора регионов на изображении
	BOOL m_bSelectRectangle;
private:
	// представление, владеющее элементом управления
	CView *m_pView;
	// отображаемые данные
	CImageData* m_image;
public:
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	// Отобразить выедленные рамки выделенных областей изображения
	void PaintRegions(CDC* pDC);
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	void SetView(CView* pView);
	afx_msg void OnDestroy();
	BOOL RegisterWindowClass(void);
	// улучшенное масштабирование изображения
	void ScaleImage(void);
};

