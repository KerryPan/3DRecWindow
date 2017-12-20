// Dlg_genOption.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "3DRecWindow.h"
#include "Dlg_genOption.h"
#include "afxdialogex.h"

#include "3DRecWindowDlg.h"


// Dlg_genOption �Ի���

IMPLEMENT_DYNAMIC(Dlg_genOption, CDialogEx)

Dlg_genOption::Dlg_genOption(CWnd* pParent /*=NULL*/)
	: CDialogEx(Dlg_genOption::IDD, pParent)
{

}

Dlg_genOption::~Dlg_genOption()
{
}

void Dlg_genOption::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(Dlg_genOption, CDialogEx)
	ON_BN_CLICKED(IDOK, &Dlg_genOption::OnBnClickedOk)
END_MESSAGE_MAP()


// Dlg_genOption ��Ϣ�������


BOOL Dlg_genOption::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	//genOption������ʼֵ
	SetDlgItemText(IDC_EDIT21, _T("1"));
	SetDlgItemText(IDC_EDIT22, _T("2"));
	SetDlgItemText(IDC_EDIT23, _T("0.7"));
	SetDlgItemText(IDC_EDIT24, _T("7"));
	SetDlgItemText(IDC_EDIT25, _T("3"));
	SetDlgItemText(IDC_EDIT26, _T("32"));

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��

	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣:  OCX ����ҳӦ���� FALSE
}


void Dlg_genOption::OnBnClickedOk()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	CMy3DRecWindowDlg * parent = (CMy3DRecWindowDlg*)GetParent();

	CString strtmp;
	GetDlgItemText(IDC_EDIT21, strtmp);
	parent->m_lever = strtmp;
	GetDlgItemText(IDC_EDIT22, strtmp);
	parent->m_csize = strtmp;
	GetDlgItemText(IDC_EDIT23, strtmp);
	parent->m_threshold = strtmp;
	GetDlgItemText(IDC_EDIT24, strtmp);
	parent->m_wsize = strtmp;
	GetDlgItemText(IDC_EDIT25, strtmp);
	parent->m_minNbImage = strtmp;
	GetDlgItemText(IDC_EDIT26, strtmp);
	parent->m_nbCPU = strtmp;

	CDialogEx::OnOK();
}
