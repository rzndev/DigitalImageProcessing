#pragma once
#include "DataProcess.h"
// CBuildGraphic
// Класс построения графика функции.

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
	// Построить сетку графика
	void PaintGrid(CDC* pDC);
	// Построить график функции
	void PaintGraph(CDC* oDC);
private:
	// функция, для которой строится график. Допустимые значения изменения аргумента 0-255
	CDataProcess *m_dataProcess;
public:
	BOOL RegisterWindowClass(void);
	// установить класс функции, для которой строится график
	void setDataProcess(CDataProcess* dataProcess);
};


