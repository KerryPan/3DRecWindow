#pragma once


// Dlg_BundlerMatcher �Ի���

class Dlg_BundlerMatcher : public CDialogEx
{
	DECLARE_DYNAMIC(Dlg_BundlerMatcher)

public:
	Dlg_BundlerMatcher(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~Dlg_BundlerMatcher();

// �Ի�������
	enum { IDD = IDD_DIALOG_BUNDLERMATCHER };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedOk();
};
