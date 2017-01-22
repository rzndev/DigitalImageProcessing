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
	// ���������� ����, � ������� ���������� ��������� �� ���������� ������
	HWND m_hwndParent;

	double m_k;
	double m_b;
	int m_x;
	// ���������� �������� ��� �������������� � ������������
	void SetParams(CLinearDataProcess* prm);
	// ��������� ������ �����������
	void FillList(void);
	// ��������� �������-�������� �������
	CLinearDataProcess* m_prms;
	afx_msg void OnBnClickedButtonAdd();
	afx_msg void OnBnClickedButtonDelete();
};
