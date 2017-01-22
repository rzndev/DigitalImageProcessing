#pragma once


// CParamsDlg dialog

class CParamsDlg : public CDialog
{
	DECLARE_DYNAMIC(CParamsDlg)

public:
	CParamsDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CParamsDlg();

// Dialog Data
	enum { IDD = IDD_PARAMSDLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
};
