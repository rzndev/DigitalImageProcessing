#pragma once
#include "DataProcess.h"
// CBuildGraphic
// ����� ���������� ������� �������.

class CBuildGraphic : public CWnd
{
	DECLARE_DYNAMIC(CBuildGraphic)

public:
	CBuildGraphic();
	virtual ~CBuildGraphic();

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnPaint();
	// ��������� ����� �������
	void PaintGrid(CDC* pDC);
	// ��������� ������ �������
	void PaintGraph(CDC* oDC);
private:
	// �������, ��� ������� �������� ������. ���������� �������� ��������� ��������� 0-255
	CDataProcess *m_dataProcess;
public:
	BOOL RegisterWindowClass(void);
	// ���������� ����� �������, ��� ������� �������� ������
	void setDataProcess(CDataProcess* dataProcess);
};


