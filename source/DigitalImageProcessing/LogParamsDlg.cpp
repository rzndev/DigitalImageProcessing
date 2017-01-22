// LogParamsDlg.cpp : implementation file
//

#include "stdafx.h"
#include "DigitalImageProcessing.h"
#include "LogParamsDlg.h"
#include "afxdialogex.h"


// CLogParamsDlg dialog

IMPLEMENT_DYNAMIC(CLogParamsDlg, CDialog)

CLogParamsDlg::CLogParamsDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CLogParamsDlg::IDD, pParent)
	, m_C(0)
{

}

CLogParamsDlg::~CLogParamsDlg()
{
}

void CLogParamsDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_LOGPARAM_C, m_C);
}


BEGIN_MESSAGE_MAP(CLogParamsDlg, CDialog)
	ON_BN_CLICKED(IDC_BUTTON_APPLY, &CLogParamsDlg::OnBnClickedButtonApply)
END_MESSAGE_MAP()


// CLogParamsDlg message handlers




void CLogParamsDlg::OnBnClickedButtonApply()
{
	UpdateData();
	::SendMessage(m_hwndParent,USER_MESSAGE_UPDATE_PARAMS,0,0);
}


// установить параметр
void CLogParamsDlg::SetParams(double C)
{
	m_C = C;
	UpdateData(FALSE);
}
