#pragma once


// Dlg_genOption �Ի���

class Dlg_genOption : public CDialogEx
{
	DECLARE_DYNAMIC(Dlg_genOption)

public:
	Dlg_genOption(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~Dlg_genOption();

// �Ի�������
	enum { IDD = IDD_DIALOG_GENOPTION };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedOk();
};
