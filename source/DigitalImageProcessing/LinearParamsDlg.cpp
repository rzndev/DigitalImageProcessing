// LinearParamsDlg.cpp : implementation file
//

#include "stdafx.h"
#include "DigitalImageProcessing.h"
#include "LinearParamsDlg.h"
#include "afxdialogex.h"


// CLinearParamsDlg dialog

IMPLEMENT_DYNAMIC(CLinearParamsDlg, CDialog)

CLinearParamsDlg::CLinearParamsDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CLinearParamsDlg::IDD, pParent)
	, m_k(0)
	, m_b(0)
	, m_x(0)
	, m_prms(NULL)
{

}

CLinearParamsDlg::~CLinearParamsDlg()
{
}

void CLinearParamsDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_K, m_k);
	DDX_Text(pDX, IDC_EDIT_B, m_b);
	DDX_Text(pDX, IDC_EDIT_X, m_x);
	DDV_MinMaxInt(pDX, m_x, 0, 255);
}


BEGIN_MESSAGE_MAP(CLinearParamsDlg, CDialog)
	
	ON_BN_CLICKED(IDC_BUTTON_ADD, &CLinearParamsDlg::OnBnClickedButtonAdd)
	ON_BN_CLICKED(IDC_BUTTON_DELETE, &CLinearParamsDlg::OnBnClickedButtonDelete)
END_MESSAGE_MAP()


// CLinearParamsDlg message handlers


// установить параметр для редактирования и визуализации
void CLinearParamsDlg::SetParams(CLinearDataProcess* prm)
{
	m_prms = prm;
	FillList();
}


// заполнить список параметрами
void CLinearParamsDlg::FillList(void)
{
	CString st;
	if (NULL == GetSafeHwnd()) return;
	CListBox* pList = (CListBox*)GetDlgItem(IDC_LIST_COEFS);
	if (NULL == pList) return;
	int cursel = pList->GetCurSel();
	pList->ResetContent();
	if (NULL == m_prms) return;
	vector<SCoefs>::iterator it;
	for (it = m_prms->m_coefs.begin(); it != m_prms->m_coefs.end(); it++)
	{
		st.Format(_T("x=%d,k=%f,b=%f"), it->x_begin, it->k, it->b);
		pList->AddString(st);
	}
	if (cursel < m_prms->m_coefs.size()) pList->SetCurSel(cursel);
}


void CLinearParamsDlg::OnBnClickedButtonAdd()
{
	UpdateData();
	SCoefs coef;
	coef.x_begin = m_x;
	coef.k = m_k;
	coef.b = m_b;
	m_prms->AddCoefs(coef);
	FillList();
	::SendMessage(m_hwndParent, USER_MESSAGE_UPDATE_PARAMS,0,0);
}


void CLinearParamsDlg::OnBnClickedButtonDelete()
{
	if (NULL == m_prms) return;
	if (NULL == GetSafeHwnd()) return;
	CListBox* pList = (CListBox*)GetDlgItem(IDC_LIST_COEFS);
	if (NULL == pList) return;
	int cursel = pList->GetCurSel();
	if (-1 == cursel) return;
	m_prms->RemoveAtID(cursel);
	FillList();
	::SendMessage(m_hwndParent, USER_MESSAGE_UPDATE_PARAMS,0,0);
}
