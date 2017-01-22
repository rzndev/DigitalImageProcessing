#pragma once
// диалог, предназначенный для ввода параметров логарифмического преобразования

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
	// дескриптор окна, в которое передается сообщение об обновлении данных
	HWND m_hwndParent;
	afx_msg void OnBnClickedButtonApply();
	// установить параметр
	void SetParams(double C);
};
