#pragma once


// CPowParamsDlg dialog

class CPowParamsDlg : public CDialog
{
	DECLARE_DYNAMIC(CPowParamsDlg)

public:
	CPowParamsDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CPowParamsDlg();

// Dialog Data
	enum { IDD = IDD_POWPARAMSDLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	double m_gamma;
	double m_C;
	// ���������� ����, � ������� ���������� ��������� �� ���������� ������
	HWND m_hwndParent;
	afx_msg void OnBnClickedButtonApply();
	// ���������� ���������
	void SetParams(double C, double gamma);
};
