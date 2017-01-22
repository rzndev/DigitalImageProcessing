#pragma once
// ����� ������ ������� ��������� �����������
#include "DataProcess.h"
#include "LogDataProcess.h"
#include "PowDataProcess.h"
#include "LinearDataProcess.h"
#include "NegativeDataProcess.h"
#include "buildgraphic.h"

#include "LogParamsDlg.h"
#include "PowParamsDlg.h"
#include "ParamsDlg.h"
#include "linearparamsdlg.h"
#include "atltypes.h"

// CSelectProcess dialog

class CSelectProcess : public CDialog
{
	DECLARE_DYNAMIC(CSelectProcess)

public:
	CSelectProcess(CWnd* pParent = NULL);   // standard constructor
	virtual ~CSelectProcess();

// Dialog Data
	enum { IDD = IDD_SELECTPROCESS };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	void SetCurrentProcess(CDataProcess* dataProcess);
	CBuildGraphic m_graphic;
	afx_msg void OnCbnSelchangeComboMethod();
	// ��������� � ����������� ��� ���������
	CDataProcess* m_pParamsProcess;
	// ������� ����������� ������� �������� ����������, ������������� ����� ����������
	void ChangeParamControl(void);
	CLogParamsDlg m_logParamsDlg;
	CPowParamsDlg m_powParamsDlg;
protected:
	afx_msg LRESULT OnUserMessageUpdateParams(WPARAM wParam, LPARAM lParam);
public:
	CParamsDlg m_paramsDlg;
	CLinearParamsDlg m_linearParamsDlg;
	void Initialize(void);
	virtual BOOL OnInitDialog();
	CRect m_RectChildDlg;
	void UserChangeCurrentProcess(int sel_item);
	// �������� ������
	void UpdateGraphic(void);
};
