#pragma once
// ������, ��������������� ��� ����� ���������� ���������������� ��������������

// CLogParamsDlg dialog

class CLogParamsDlg : public CDialog
{
	DECLARE_DYNAMIC(CLogParamsDlg)

public:
	CLogParamsDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CLogParamsDlg();

// Dialog Data
	enum { IDD = IDD_LOGPARAMSDLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	double m_C;
	// ���������� ����, � ������� ���������� ��������� �� ���������� ������
	HWND m_hwndParent;
	afx_msg void OnBnClickedButtonApply();
	// ���������� ��������
	void SetParams(double C);
};
