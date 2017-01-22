// ParamsDlg.cpp : implementation file
//

#include "stdafx.h"
#include "DigitalImageProcessing.h"
#include "ParamsDlg.h"
#include "afxdialogex.h"


// CParamsDlg dialog

IMPLEMENT_DYNAMIC(CParamsDlg, CDialog)

CParamsDlg::CParamsDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CParamsDlg::IDD, pParent)
{

}

CParamsDlg::~CParamsDlg()
{
}

void CParamsDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CParamsDlg, CDialog)
END_MESSAGE_MAP()


// CParamsDlg message handlers
