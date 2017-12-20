// Dlg_ImageMatch.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "3DRecWindow.h"
#include "Dlg_ImageMatch.h"
#include "afxdialogex.h"


// Dlg_ImageMatch �Ի���

IMPLEMENT_DYNAMIC(Dlg_ImageMatch, CDialogEx)

Dlg_ImageMatch::Dlg_ImageMatch(CWnd* pParent /*=NULL*/)
	: CDialogEx(Dlg_ImageMatch::IDD, pParent)
{

}

Dlg_ImageMatch::~Dlg_ImageMatch()
{
}

void Dlg_ImageMatch::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(Dlg_ImageMatch, CDialogEx)
	ON_STN_CLICKED(IDC_PIC66, &Dlg_ImageMatch::OnStnClickedPic66)

	ON_WM_PAINT()
	
END_MESSAGE_MAP()


// Dlg_ImageMatch ��Ϣ�������


void Dlg_ImageMatch::DrawPicToHDC(Mat img, UINT ID)
{
	CDC *pDC = GetDlgItem(ID)->GetDC();
	HDC hDC = pDC->GetSafeHdc();
	CRect rect;
	GetDlgItem(ID)->GetClientRect(&rect);
	CvvImage cimg;
	Mat destimg;
	img.copyTo(destimg);
	IplImage pimg = IplImage(destimg); //Mat convto IplImage
	cimg.CopyOf(&pimg, 1);           // ����ͼƬ
	cimg.DrawToHDC(hDC, &rect); // ��ͼƬ���Ƶ���ʾ�ؼ���ָ��������
	//ReleaseDC(pDC);
	//DeleteDC(hDC);
}



void Dlg_ImageMatch::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO:  �ڴ˴������Ϣ����������
	// ��Ϊ��ͼ��Ϣ���� CDialogEx::OnPaint()
	//DrawPicToHDC(matchImage[0], IDC_PIC66);

}


BOOL Dlg_ImageMatch::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��

	
	//DrawPicToHDC(matchImage[0], IDC_PIC66);

	matchnum = 0;

	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣:  OCX ����ҳӦ���� FALSE
}


void Dlg_ImageMatch::OnStnClickedPic66()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	int n = matchsize;
	
	if (matchnum < n)
	{
		DrawPicToHDC(matchImage[matchnum], IDC_PIC66);
		matchnum++;
	}
	else
	{
		matchnum = 0;
		DrawPicToHDC(matchImage[matchnum], IDC_PIC66);
		matchnum++;
	}

}






