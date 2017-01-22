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
	// ������������ ������
	void DisplayImage(CImageData* data);
	DECLARE_MESSAGE_MAP()
	afx_msg void OnPaint();
public:
	// ������� ������������� ������ �������� �� �����������
	BOOL m_bSelectRectangle;
private:
	// �������������, ��������� ��������� ����������
	CView *m_pView;
	// ������������ ������
	CImageData* m_image;
public:
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	// ���������� ���������� ����� ���������� �������� �����������
	void PaintRegions(CDC* pDC);
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	void SetView(CView* pView);
	afx_msg void OnDestroy();
	BOOL RegisterWindowClass(void);
	// ���������� ��������������� �����������
	void ScaleImage(void);
};

