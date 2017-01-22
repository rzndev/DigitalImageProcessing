// PowParamsDlg.cpp : implementation file
//

#include "stdafx.h"
#include "DigitalImageProcessing.h"
#include "PowParamsDlg.h"
#include "afxdialogex.h"


// CPowParamsDlg dialog

IMPLEMENT_DYNAMIC(CPowParamsDlg, CDialog)

CPowParamsDlg::CPowParamsDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CPowParamsDlg::IDD, pParent)
	, m_gamma(0)
	, m_C(0)
	, m_hwndParent(NULL)
{

}

CPowParamsDlg::~CPowParamsDlg()
{
}

void CPowParamsDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_POWPARAM_GAMMA, m_gamma);
	DDX_Text(pDX, IDC_EDIT_POWPARAM_C, m_C);
}


BEGIN_MESSAGE_MAP(CPowParamsDlg, CDialog)
	ON_BN_CLICKED(IDC_BUTTON_APPLY, &CPowParamsDlg::OnBnClickedButtonApply)
END_MESSAGE_MAP()


// CPowParamsDlg message handlers





void CPowParamsDlg::OnBnClickedButtonApply()
{
	UpdateData();
	::SendMessage(m_hwndParent,USER_MESSAGE_UPDATE_PARAMS,0,0);
}


// Установить параметры
void CPowParamsDlg::SetParams(double C, double gamma)
{
	m_C = C;
	m_gamma = gamma;
	UpdateData(FALSE);
}
