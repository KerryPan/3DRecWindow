// Dlg_BundlerMatcher.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "3DRecWindow.h"
#include "Dlg_BundlerMatcher.h"
#include "afxdialogex.h"

#include "3DRecWindowDlg.h""


// Dlg_BundlerMatcher �Ի���

IMPLEMENT_DYNAMIC(Dlg_BundlerMatcher, CDialogEx)

Dlg_BundlerMatcher::Dlg_BundlerMatcher(CWnd* pParent /*=NULL*/)
	: CDialogEx(Dlg_BundlerMatcher::IDD, pParent)
{

}

Dlg_BundlerMatcher::~Dlg_BundlerMatcher()
{
}

void Dlg_BundlerMatcher::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(Dlg_BundlerMatcher, CDialogEx)
	ON_BN_CLICKED(IDOK, &Dlg_BundlerMatcher::OnBnClickedOk)
END_MESSAGE_MAP()


// Dlg_BundlerMatcher ��Ϣ�������


BOOL Dlg_BundlerMatcher::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	SetDlgItemText(IDC_EDIT111, _T("0.7"));
	SetDlgItemText(IDC_EDIT112, _T("-1"));
	
	////genOption������ʼֵ
	//SetDlgItemText(IDC_EDIT6, _T("1"));
	//SetDlgItemText(IDC_EDIT7, _T("2"));
	//SetDlgItemText(IDC_EDIT8, _T("0.7"));
	//SetDlgItemText(IDC_EDIT9, _T("7"));
	//SetDlgItemText(IDC_EDIT10, _T("3"));
	//SetDlgItemText(IDC_EDIT11, _T("4"));

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��

	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣:  OCX ����ҳӦ���� FALSE
}


void Dlg_BundlerMatcher::OnBnClickedOk()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	CMy3DRecWindowDlg * parent = (CMy3DRecWindowDlg*)GetParent();

	CString strtmp;
	GetDlgItemText(IDC_EDIT111, strtmp);
	parent->m_matchthreshold = strtmp;

	GetDlgItemText(IDC_EDIT112, strtmp);
	parent->m_firstOctave = strtmp;

	//parent->m_edit.SetWindowText(this->m_value);
	CDialogEx::OnOK();
}
