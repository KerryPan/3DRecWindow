// Dlg_ImageMatch.cpp : 实现文件
//

#include "stdafx.h"
#include "3DRecWindow.h"
#include "Dlg_ImageMatch.h"
#include "afxdialogex.h"


// Dlg_ImageMatch 对话框

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


// Dlg_ImageMatch 消息处理程序


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
	cimg.CopyOf(&pimg, 1);           // 复制图片
	cimg.DrawToHDC(hDC, &rect); // 将图片绘制到显示控件的指定区域内
	//ReleaseDC(pDC);
	//DeleteDC(hDC);
}



void Dlg_ImageMatch::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO:  在此处添加消息处理程序代码
	// 不为绘图消息调用 CDialogEx::OnPaint()
	//DrawPicToHDC(matchImage[0], IDC_PIC66);

}


BOOL Dlg_ImageMatch::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化

	
	//DrawPicToHDC(matchImage[0], IDC_PIC66);

	matchnum = 0;

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常:  OCX 属性页应返回 FALSE
}


void Dlg_ImageMatch::OnStnClickedPic66()
{
	// TODO:  在此添加控件通知处理程序代码
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






