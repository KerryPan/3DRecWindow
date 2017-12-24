// Dlg_BundlerMatcher.cpp : 实现文件
//

#include "stdafx.h"
#include "3DRecWindow.h"
#include "Dlg_BundlerMatcher.h"
#include "afxdialogex.h"

#include "3DRecWindowDlg.h""


// Dlg_BundlerMatcher 对话框

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


// Dlg_BundlerMatcher 消息处理程序


BOOL Dlg_BundlerMatcher::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	SetDlgItemText(IDC_EDIT111, _T("0.7"));
	SetDlgItemText(IDC_EDIT112, _T("-1"));
	
	////genOption参数初始值
	//SetDlgItemText(IDC_EDIT6, _T("1"));
	//SetDlgItemText(IDC_EDIT7, _T("2"));
	//SetDlgItemText(IDC_EDIT8, _T("0.7"));
	//SetDlgItemText(IDC_EDIT9, _T("7"));
	//SetDlgItemText(IDC_EDIT10, _T("3"));
	//SetDlgItemText(IDC_EDIT11, _T("4"));

	// TODO:  在此添加额外的初始化

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常:  OCX 属性页应返回 FALSE
}


void Dlg_BundlerMatcher::OnBnClickedOk()
{
	// TODO:  在此添加控件通知处理程序代码
	CMy3DRecWindowDlg * parent = (CMy3DRecWindowDlg*)GetParent();

	CString strtmp;
	GetDlgItemText(IDC_EDIT111, strtmp);
	parent->m_matchthreshold = strtmp;

	GetDlgItemText(IDC_EDIT112, strtmp);
	parent->m_firstOctave = strtmp;

	//parent->m_edit.SetWindowText(this->m_value);
	CDialogEx::OnOK();
}
