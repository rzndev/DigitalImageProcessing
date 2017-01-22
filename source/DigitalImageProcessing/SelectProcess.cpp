// SelectProcess.cpp : implementation file
//

#include "stdafx.h"
#include "DigitalImageProcessing.h"
#include "SelectProcess.h"
#include "afxdialogex.h"


// CSelectProcess dialog

IMPLEMENT_DYNAMIC(CSelectProcess, CDialog)

CSelectProcess::CSelectProcess(CWnd* pParent /*=NULL*/)
	: CDialog(CSelectProcess::IDD, pParent)
	, m_pParamsProcess(NULL)
{

}

CSelectProcess::~CSelectProcess()
{
}

void CSelectProcess::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_CUSTOM1, m_graphic);
}


BEGIN_MESSAGE_MAP(CSelectProcess, CDialog)
	ON_CBN_SELCHANGE(IDC_COMBO_METHOD, &CSelectProcess::OnCbnSelchangeComboMethod)
	ON_MESSAGE(USER_MESSAGE_UPDATE_PARAMS, &CSelectProcess::OnUserMessageUpdateParams)
END_MESSAGE_MAP()


// ���������� ������� ������ ��������� ������


void CSelectProcess::SetCurrentProcess(CDataProcess* dataProcess)
{
	if (NULL == dataProcess)
	{
		if (NULL != m_pParamsProcess)
		{
			// ���� ������� �����-���� ��������� ���������, ���������� �����.
			delete m_pParamsProcess;
			m_pParamsProcess = NULL;
		}
	}
	else
	{
		if (m_pParamsProcess != NULL)
		{
			if (dataProcess->m_typeProcess == m_pParamsProcess->m_typeProcess) return; // ���� ������ ��� �� ������ ���������, ��� � �������, ��������� �� ������
			delete m_pParamsProcess;
			m_pParamsProcess = NULL;
		} 

		switch (dataProcess->m_typeProcess)
		{
		case 0:
			// ��� ���������
			m_pParamsProcess = new CDataProcess();
			break;
		case 1:
			// �������
			m_pParamsProcess = new CNegativeDataProcess();
			break;
		case 2:
			// ��������
			m_pParamsProcess = new CLogDataProcess((CLogDataProcess*)dataProcess);
			break;
		case 3:
			// ���������
			m_pParamsProcess = new CPowDataProcess((CPowDataProcess*)dataProcess);
			break;
		case 4:
			// �������-��������
			m_pParamsProcess = new CLinearDataProcess((CLinearDataProcess*)dataProcess);
			break;
		}
	}
}


void CSelectProcess::OnCbnSelchangeComboMethod()
{
	CComboBox* box = (CComboBox*)GetDlgItem(IDC_COMBO_METHOD);
	int sel_item = (box->GetCurSel());
	if (sel_item == CB_ERR)
	{
	} else
	{
		UserChangeCurrentProcess(sel_item);
	}
}


// ������� ����������� ������� �������� ����������, ������������� ����� ����������
void CSelectProcess::ChangeParamControl(void)
{
	CWnd* pWnd = GetDlgItem(IDC_PARAMS_DLG);
	CComboBox* box = (CComboBox*)GetDlgItem(IDC_COMBO_METHOD);

	if (NULL != pWnd) pWnd->Detach();
	if (NULL == m_pParamsProcess)
	{
		m_paramsDlg.SubclassDlgItem(IDC_PARAMS_DLG, this);
		box->SetCurSel(-1);
		return;
	}
	box->SetCurSel(m_pParamsProcess->m_typeProcess);
	switch(m_pParamsProcess->m_typeProcess)
	{
	case 0:
		// ��� ���������
	case 1:
		// �������
		m_paramsDlg.SubclassDlgItem(IDC_PARAMS_DLG, this);
		m_logParamsDlg.ShowWindow(SW_HIDE);
		m_powParamsDlg.ShowWindow(SW_HIDE);
		m_linearParamsDlg.ShowWindow(SW_HIDE);
		m_paramsDlg.ShowWindow(SW_SHOW);
		break;
	case 2:
		// ��������
		m_logParamsDlg.SubclassDlgItem(IDC_PARAMS_DLG, this);
		m_logParamsDlg.SetParams(((CLogDataProcess*)m_pParamsProcess)->m_C);
		m_logParamsDlg.ShowWindow(SW_SHOW);
		m_powParamsDlg.ShowWindow(SW_HIDE);
		m_linearParamsDlg.ShowWindow(SW_HIDE);
		m_paramsDlg.ShowWindow(SW_HIDE);
		break;
	case 3:
		// ���������
		m_powParamsDlg.SubclassDlgItem(IDC_PARAMS_DLG, this);
		m_powParamsDlg.ShowWindow(SW_SHOW);
		m_logParamsDlg.ShowWindow(SW_HIDE);
		m_linearParamsDlg.ShowWindow(SW_HIDE);
		m_paramsDlg.ShowWindow(SW_HIDE);
		m_powParamsDlg.SetParams(((CPowDataProcess*)m_pParamsProcess)->m_C,((CPowDataProcess*)m_pParamsProcess)->m_gamma);
		break;
	case 4:
		// �������-��������
		m_linearParamsDlg.SubclassDlgItem(IDC_PARAMS_DLG, this);
		m_linearParamsDlg.ShowWindow(SW_SHOW);
		m_powParamsDlg.ShowWindow(SW_HIDE);
		m_logParamsDlg.ShowWindow(SW_HIDE);
		m_paramsDlg.ShowWindow(SW_HIDE);
		m_linearParamsDlg.SetParams((CLinearDataProcess*)m_pParamsProcess);
		break;
	}

	UpdateGraphic();
}


afx_msg LRESULT CSelectProcess::OnUserMessageUpdateParams(WPARAM wParam, LPARAM lParam)
{
	UpdateGraphic();
	return 0;
}


void CSelectProcess::Initialize(void)
{
	CWnd* pWnd = GetDlgItem(IDC_PARAMS_DLG);
	if (NULL == pWnd) return;
	pWnd->GetWindowRect(&m_RectChildDlg);
	ScreenToClient(&m_RectChildDlg);
	pWnd->DestroyWindow();
	m_paramsDlg.Create(IDD_PARAMSDLG,this);
	m_paramsDlg.SetWindowPos(NULL, m_RectChildDlg.left, m_RectChildDlg.top, m_RectChildDlg.Width(), m_RectChildDlg.Height(), SWP_HIDEWINDOW);
	//m_paramsDlg.ShowWindow(SW_HIDE);
	m_logParamsDlg.Create(IDD_LOGPARAMSDLG,this);
	m_logParamsDlg.m_hwndParent = GetSafeHwnd();
	m_logParamsDlg.SetWindowPos(NULL, m_RectChildDlg.left, m_RectChildDlg.top, m_RectChildDlg.Width(), m_RectChildDlg.Height(), SWP_HIDEWINDOW);
	//m_logParamsDlg.ShowWindow(SW_HIDE);
	m_powParamsDlg.Create(IDD_POWPARAMSDLG,this);
	m_powParamsDlg.m_hwndParent = GetSafeHwnd();
	m_powParamsDlg.SetWindowPos(NULL, m_RectChildDlg.left, m_RectChildDlg.top, m_RectChildDlg.Width(), m_RectChildDlg.Height(), SWP_HIDEWINDOW);
	//m_powParamsDlg.ShowWindow(SW_HIDE);
	m_linearParamsDlg.Create(IDD_LINEARPARAMSDLG, this);
	m_linearParamsDlg.m_hwndParent = GetSafeHwnd();
	m_linearParamsDlg.SetWindowPos(NULL, m_RectChildDlg.left, m_RectChildDlg.top, m_RectChildDlg.Width(), m_RectChildDlg.Height(), SWP_HIDEWINDOW);
	//m_linearParamsDlg.ShowWindow(SW_HIDE);
	ChangeParamControl();
}


BOOL CSelectProcess::OnInitDialog()
{
	CDialog::OnInitDialog();

	Initialize();
	ChangeParamControl();
	return TRUE;  
}


void CSelectProcess::UserChangeCurrentProcess(int sel_item)
{
	
		if (m_pParamsProcess != NULL)
		{
			if (sel_item == m_pParamsProcess->m_typeProcess) return; // ���� ������ ��� �� ������ ���������, ��� � �������, ��������� �� ������
			delete m_pParamsProcess;
			m_pParamsProcess = NULL;
		} 

		switch (sel_item)
		{
		case 0:
			// ��� ���������
			m_pParamsProcess = new CDataProcess();
			break;
		case 1:
			// �������
			m_pParamsProcess = new CNegativeDataProcess();
			break;
		case 2:
			// ��������
			m_pParamsProcess = new CLogDataProcess();
			break;
		case 3:
			// ���������
			m_pParamsProcess = new CPowDataProcess();
			break;
		case 4:
			// �������-��������
			m_pParamsProcess = new CLinearDataProcess();
			break;
		}
		ChangeParamControl();
}


// �������� ������
void CSelectProcess::UpdateGraphic(void)
{
	// ��������� ����������� ��������� �� ������� � ������ ������
	if (m_pParamsProcess == NULL)
		{
			m_graphic.setDataProcess(NULL);
			return;
		} 

	switch(m_pParamsProcess->m_typeProcess)
	{
	case 2:
		// ��������
		((CLogDataProcess*)m_pParamsProcess)->m_C = m_logParamsDlg.m_C;
		break;
	case 3:
		// ���������
		((CPowDataProcess*)m_pParamsProcess)->m_C = m_powParamsDlg.m_C;
		((CPowDataProcess*)m_pParamsProcess)->m_gamma = m_powParamsDlg.m_gamma;
		break;
	case 4:
		// �������-��������
		break;
	}
	m_graphic.setDataProcess(m_pParamsProcess);
}





