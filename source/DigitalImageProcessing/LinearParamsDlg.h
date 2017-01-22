#pragma once
#include "LinearDataProcess.h"
#include <vector>

using namespace std;
// CLinearParamsDlg dialog

class CLinearParamsDlg : public CDialog
{
	DECLARE_DYNAMIC(CLinearParamsDlg)

public:
	CLinearParamsDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CLinearParamsDlg();

// Dialog Data
	enum { IDD = IDD_LINEARPARAMSDLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	// дескриптор окна, в которое передается сообщение об обновлении данных
	HWND m_hwndParent;

	double m_k;
	double m_b;
	int m_x;
	// установить параметр для редактирования и визуализации
	void SetParams(CLinearDataProcess* prm);
	// заполнить списко параметрами
	void FillList(void);
	// Параметры кусочно-линейной функции
	CLinearDataProcess* m_prms;
	afx_msg void OnBnClickedButtonAdd();
	afx_msg void OnBnClickedButtonDelete();
};
