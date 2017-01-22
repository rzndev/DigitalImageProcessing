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
	// дескриптор окна, в которое передаетс€ сообщение об обновлении данных
	HWND m_hwndParent;
	afx_msg void OnBnClickedButtonApply();
	// ”становить параметры
	void SetParams(double C, double gamma);
};
