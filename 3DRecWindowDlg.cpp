
// 3DRecWindowDlg.cpp : 实现文件
//
#include "stdafx.h"
#include "3DRecWindow.h"
#include "3DRecWindowDlg.h"
#include "afxdialogex.h"

#include "MenuEx.h"

#include "Dlg_BundlerMatcher.h"
#include "Dlg_genOption.h"
#include "Dlg_ImageMatch.h"

#include <string>

//#include <FSTREAM> 

#include <iostream>
using namespace std;

#include <fstream>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif



// 全局变量

Dlg_ImageMatch dlg;

HANDLE h1 = NULL;                // 线程句柄
DWORD pid = 0;                   // 保存cmd窗口的PID
CString inputCmd;     // 保存cmd命令和返回的结果
CString inputCmd2;

CString outputCmd;
CEdit *m_edit;                  // 输出框

int minHessian = 1000;
SurfFeatureDetector detector(minHessian);
SurfDescriptorExtractor extractor;


//sift出错。。。
//SiftFeatureDetector detector(minHessian);
//SiftDescriptorExtractor extractor;


// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CMy3DRecWindowDlg 对话框



CMy3DRecWindowDlg::CMy3DRecWindowDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_MY3DRECWINDOW_DIALOG, pParent)
	, m_info(_T(""))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDI_ICON3);
	m_pMenuEx = NULL;

}

void CMy3DRecWindowDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_info);
	DDX_Control(pDX, IDC_EDIT1, m_editctr);
}

BEGIN_MESSAGE_MAP(CMy3DRecWindowDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_WM_CLOSE()

	ON_BN_CLICKED(IDC_BUTTON_OPEN, &CMy3DRecWindowDlg::OnBnClickedButtonOpen)
	ON_BN_CLICKED(IDC_BUTTON_EXTRACT, &CMy3DRecWindowDlg::OnBnClickedButtonExtract)
	ON_BN_CLICKED(IDC_BUTTON_MATCH, &CMy3DRecWindowDlg::OnBnClickedButtonMatch)
	ON_BN_CLICKED(IDC_BUTTON_SFM, &CMy3DRecWindowDlg::OnBnClickedButtonSfm)
	ON_BN_CLICKED(IDC_BUTTON5, &CMy3DRecWindowDlg::OnBnClickedButton5)
	ON_BN_CLICKED(IDC_BUTTON6, &CMy3DRecWindowDlg::OnBnClickedButton6)
	ON_BN_CLICKED(IDC_BUTTON7, &CMy3DRecWindowDlg::OnBnClickedButton7)

	ON_BN_CLICKED(IDCANCEL, &CMy3DRecWindowDlg::OnBnClickedCancel)
	
	
	ON_BN_CLICKED(IDC_BUTTON_NEXTIMAGE, &CMy3DRecWindowDlg::OnBnClickedButtonNextimage)

	ON_COMMAND_RANGE(0xE00, 0xE00 + 0xFFF, OnMenuItem)
	

	ON_BN_CLICKED(IDC_BUTTON_MODLECHANGE, &CMy3DRecWindowDlg::OnBnClickedButtonModlechange)
	ON_BN_CLICKED(IDC_BUTTON_POINTCLEAR, &CMy3DRecWindowDlg::OnBnClickedButtonPointclear)
	ON_BN_CLICKED(IDC_BUTTON_MATCHIMAGE, &CMy3DRecWindowDlg::OnBnClickedButtonMatchimage)
END_MESSAGE_MAP()


// CMy3DRecWindowDlg 消息处理程序

BOOL CMy3DRecWindowDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码
	//设置默认工程的目录

	CString testWorkPath;
	TCHAR Buffer[MAX_PATH];
	GetCurrentDirectory(MAX_PATH, Buffer);
	testWorkPath.Format(_T("%s"), Buffer); //testWorkPath = "C:\\Users\\pan\\Desktop\\3DRecWindow\\3DRecWindow"

	mainPath = testWorkPath +"\\";

	//picPath = "C:\\Users\\pan\\Desktop\\ImagesTo3D\\Imgs";
	//picPath2 = "C:\\Users\\pan\\Desktop\\ImagesTo3D\\Imgs\\";

	m_matchthreshold = "0.7";
	  m_firstOctave = "-1";
	//genOption
	 m_lever = "1";
	 m_csize = "2";
	 m_threshold = "0.7";
	 m_wsize = "7";
	 m_minNbImage = "3";
	 m_nbCPU = "32";
	
	CRect rect;
	GetClientRect(&rect);     //取客户区大小    
	old.x = rect.right - rect.left;
	old.y = rect.bottom - rect.top;



	m_edit = (CEdit*)GetDlgItem(IDC_EDIT1);
	m_edit->SetLimitText(100000000);


	m_editFont.CreatePointFont(100, "楷体");	
	m_editctr.SetFont(&m_editFont); // 设置新字体
	

	
	m_buttonFont.CreatePointFont(110, "楷体");
	GetDlgItem(IDC_BUTTON_OPEN)->SetFont(&m_buttonFont);
	GetDlgItem(IDC_BUTTON_EXTRACT)->SetFont(&m_buttonFont);
	GetDlgItem(IDC_BUTTON_MATCH)->SetFont(&m_buttonFont);
	GetDlgItem(IDC_BUTTON_SFM)->SetFont(&m_buttonFont);
	GetDlgItem(IDC_BUTTON5)->SetFont(&m_buttonFont);
	GetDlgItem(IDC_BUTTON6)->SetFont(&m_buttonFont);
	GetDlgItem(IDC_BUTTON7)->SetFont(&m_buttonFont);
	GetDlgItem(IDC_BUTTON_POINTCLEAR)->SetFont(&m_buttonFont);

	m_groupFont.CreatePointFont(120, "方正舒体");
	GetDlgItem(IDC_GROUP1)->SetFont(&m_groupFont);
	GetDlgItem(IDC_GROUP2)->SetFont(&m_groupFont);
	GetDlgItem(IDC_GROUP3)->SetFont(&m_groupFont);

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CMy3DRecWindowDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CMy3DRecWindowDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CMy3DRecWindowDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CMy3DRecWindowDlg::OnBnClickedCancel()
{
	// TODO: 在此添加控件通知处理程序代码
	CDialogEx::OnCancel();
}


int CMy3DRecWindowDlg::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDialogEx::OnCreate(lpCreateStruct) == -1)
		return -1;

	ModifyStyle(0, WS_MAXIMIZEBOX | WS_MINIMIZEBOX, SWP_FRAMECHANGED);
	// TODO:  在此添加您专用的创建代码

	return 0;
}


void CMy3DRecWindowDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);

	// TODO: 在此处添加消息处理程序代码
	// TODO: 在此处添加消息处理程序代码
	CDialog::OnSize(nType, cx, cy);
	// TODO: Add your message handler code here  
	if (nType == SIZE_RESTORED || nType == SIZE_MAXIMIZED)
	{
		Resize();
	}
}

void CMy3DRecWindowDlg::Resize()
{
	float fsp[2];
	POINT Newp; //获取现在对话框的大小  
	CRect recta;
	GetClientRect(&recta);     //取客户区大小    
	Newp.x = recta.right - recta.left;
	Newp.y = recta.bottom - recta.top;
	fsp[0] = (float)Newp.x / old.x;
	fsp[1] = (float)Newp.y / old.y;
	CRect Rect;
	int woc;
	CPoint OldTLPoint, TLPoint; //左上角  
	CPoint OldBRPoint, BRPoint; //右下角  
	HWND  hwndChild = ::GetWindow(m_hWnd, GW_CHILD);  //列出所有控件    
	while (hwndChild)
	{
		woc = ::GetDlgCtrlID(hwndChild);//取得ID  
		GetDlgItem(woc)->GetWindowRect(Rect);
		ScreenToClient(Rect);
		OldTLPoint = Rect.TopLeft();
		TLPoint.x = long(OldTLPoint.x*fsp[0]);
		TLPoint.y = long(OldTLPoint.y*fsp[1]);
		OldBRPoint = Rect.BottomRight();
		BRPoint.x = long(OldBRPoint.x *fsp[0]);
		BRPoint.y = long(OldBRPoint.y *fsp[1]);
		Rect.SetRect(TLPoint, BRPoint);
		GetDlgItem(woc)->MoveWindow(Rect, TRUE);
		hwndChild = ::GetWindow(hwndChild, GW_HWNDNEXT);
	}
	old = Newp;
}


/************************************************************************/
/* 根据进程ID（PID）获取窗体句柄                                        */
/************************************************************************/
HWND CMy3DRecWindowDlg::GetWindowHandleByPID(DWORD dwProcessID)
{
	HWND h = ::GetTopWindow(0);
	while (h)
	{
		DWORD pid = 0;
		DWORD dwTheardId = GetWindowThreadProcessId(h, &pid);

		if (dwTheardId != 0)
		{
			if (pid == dwProcessID/*your process id*/)
			{
				// here h is the handle to the window
				return h;
			}
		}


		h = ::GetNextWindow(h, GW_HWNDNEXT);
	}

	return NULL;
}


void CMy3DRecWindowDlg::OnClose()
{
	// TODO:  在此添加消息处理程序代码和/或调用默认值
	// TODO: Add your message handler code here and/or call default

	// 关闭CMD窗口
	::SendMessage(GetWindowHandleByPID(pid), WM_CLOSE, NULL, NULL);

	// 销毁线程
	CloseHandle(h1);

	CDialog::OnClose();
	CDialogEx::OnClose();
}


//********************************自定义*****************************************************

void CMy3DRecWindowDlg::DrawPicToHDC(Mat img, UINT ID)
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


CString CMy3DRecWindowDlg::getPath()
{
	//获取是选中的文件的路径
	CString m_strFileOut = _T("");
	TCHAR szSelected[MAX_PATH];//用来存放文件夹路径
	BROWSEINFO bi;
	LPITEMIDLIST pidl;
	bi.hwndOwner = this->m_hWnd;
	bi.pidlRoot = NULL;
	bi.pszDisplayName = szSelected;
	bi.lpszTitle = _T("选择图片所在文件夹");
	bi.ulFlags = BIF_RETURNONLYFSDIRS;
	bi.lpfn = NULL;
	bi.lParam = NULL;
	bi.iImage = NULL;
	if ((pidl = SHBrowseForFolder(&bi)) != NULL)
	{
		if (SUCCEEDED(SHGetPathFromIDList(pidl, szSelected))) //得到文件夹的全路径，不要的话，只得本文件夹名
		{
			m_strFileOut = szSelected;
		}
		else m_strFileOut = _T("");
	}
	else m_strFileOut = _T("");

	return m_strFileOut;
}


void CMy3DRecWindowDlg::get_file_names(CString dir_name, vector<CString> & names,CString &fileFormat)
{
	dir_name += fileFormat;
	//dir_name += "\\*.JPG";
	HANDLE file;
	WIN32_FIND_DATA fileData;
	char line[1024];
	char fn[1000];
	//mbstowcs(fn,csDirPath.GetBuffer(),999);  
	file = FindFirstFile(dir_name.GetBuffer(), &fileData);
	names.push_back(fileData.cFileName);
	bool bState = false;
	bState = FindNextFile(file, &fileData);
	while (bState) {
		//wcstombs(line,(const char*)fileData.cFileName,259);  
		names.push_back(fileData.cFileName);
		bState = FindNextFile(file, &fileData);
	}
}


void  CMy3DRecWindowDlg::path_to_txt(CString &dir, CString &filename, vector<CString> &fullfilenames)
{
	
	vector<string> e;
	vector<CString>::iterator iter = fullfilenames.begin();

	for (iter; iter != fullfilenames.end(); iter++)
	{
		CString tmp = *iter;
		//string strtmp = tmp.GetBuffer(0);
		e.push_back(tmp.GetBuffer(0));
	}
	
	ofstream fout;
	CString outfile;
	CString huanhang = "\n";
	outfile = dir + filename;
	fout.open(outfile, ios_base::out);
	if (fout.is_open())
	{
		for (vector<string>::iterator i = e.begin(), end = e.end(); i != end; ++i)
		{
			fout << (*i);
			fout << huanhang;
		}
	}
	fout.close();

}


CString CMy3DRecWindowDlg::readSparsePointNum(CString strSourceDir)
{
	CStdioFile mFile;
	CString strData = "";
	//CString strSourceDir = picPath2 + "bundler_output" + "\\bundle.out";
	if (mFile.Open(strSourceDir, CFile::modeReadWrite))
	{
		mFile.ReadString(strData);
		mFile.ReadString(strData);
		mFile.Close();
	}
	vector <CString> vecString;
	int iPos = 0;
	CString strTmp;
	strTmp = strData.Tokenize(" ", iPos);
	while (strTmp.Trim() != _T(""))
	{
		vecString.push_back(strTmp);
		strTmp = strData.Tokenize(" ", iPos);
	}
	return vecString[1];
}


void CMy3DRecWindowDlg::resizeImage(Mat &img)
{
	float  resizeScale = 1.0;
	float stdWidth = 2999;//stdWidth can change
	if (img.cols > stdWidth)
	{
		//resizeScale = stdWidth / img.cols;
		resizeScale = 0.5;
		Mat imgL;
		resize(img, imgL, Size(), resizeScale, resizeScale);
	
		img = imgL.clone();
	}
}

void CMy3DRecWindowDlg::flushEdit1()
{
	this->GetDlgItem(IDC_EDIT1)->SendMessage(EM_REPLACESEL, NULL, (LPARAM)(LPTSTR)(LPCTSTR)m_info);
}

void CMy3DRecWindowDlg::OnBnClickedButtonOpen()
{
	// TODO: 在此添加控件通知处理程序代码
	
	imageShowType = 1;

	CFileDialog dlg(TRUE);

	m_info = "************ Step 1 ************";
	m_info += "\r\n";
	m_info += "打开图片所在文件夹：";
	m_info += "\r\n";
		
	picPath = getPath();   //picPath = "C:\\Users\\pan\\Desktop\\ImagesTo3D\\Imgs";
	
	picPath2 = picPath + "\\";    //  picPath2 = "C:\\Users\\pan\\Desktop\\ImagesTo3D\\Imgs\\";
	m_info += picPath2;    
	
	int iEndPos = 0;
	iEndPos = m_info.ReverseFind('\\');
	m_info = m_info.Left(iEndPos);
	m_info += "\r\n";
	

	//获取图片存放的目录picPath下所有的png文件（或者改成jpg亦可）,并把路径保存到数组picnames中
	CString fileFormat = "\\*.jpg";
	get_file_names(picPath, picnames, fileFormat);

	vector<CString>::iterator it = picnames.begin();
	for (it; it != picnames.end(); ++it)
	{
		CString cs = picPath + "\\" + *it;
		picfullnames.push_back(cs);
	}

	//把图片路径保存到imagelist.txt中
	CString filename = "imageslist.txt";
	path_to_txt(mainPath, filename, picfullnames);

	int picNum = picfullnames.size();
	CString Cnum;
	Cnum.Format("%d", picNum);

	
	m_info += "共检测到图片：";
	m_info += Cnum;
	m_info += "张";
	m_info += "\r\n";

	m_info += "所有图片路径存储在：";
	m_info += mainPath;
	m_info += filename;
	m_info += "中";
	m_info += "\r\n";
	m_info += "\r\n";

	//先清空上一步的info
	CString nul = "";
	this->GetDlgItem(IDC_EDIT1)->SendMessage(EM_REPLACESEL, NULL, (LPARAM)(LPTSTR)(LPCTSTR)nul);
	flushEdit1();

	CString cs0 = picfullnames[0];
	string s0 = cs0.GetBuffer(0);
	Mat image0 = imread(s0);	
	DrawPicToHDC(image0, IDC_PIC1);

	//显示下一张图片时的imagenum
	imagenum = 1;

}


void CMy3DRecWindowDlg::extract_features(
	vector<CString>& image_names,
	vector<vector<KeyPoint>>& key_points_for_all,
	vector<Mat>& descriptor_for_all,
	vector<vector<Vec3b>>& colors_for_all
	)
{

	key_points_for_all.clear();
	descriptor_for_all.clear();
	Mat image;
	int it_count = 0;
	
	vector<CString>::iterator it = image_names.begin();
	for (it; it != image_names.end(); ++it)
	{
		CString cs = *it;
		string s = cs.GetBuffer(0);
		image = imread(s);
		if (image.empty()) continue;
		resizeImage(image);

	  //cout << "Extracing features: " << *it << endl;

		CString str_picnum;
		str_picnum.Format("%d", it_count);
		m_info = "正在提取图像";
		m_info += str_picnum;
		m_info += "的特征点……";
		m_info += "\r\n";
		flushEdit1();

		vector<KeyPoint> key_points(10000);	

		Mat descriptor;
		Mat mascara;
		//偶尔出现内存分配失败的错误
		//sift->detectAndCompute(image, noArray(), key_points, descriptor);
		//sift1(image, mascara, key_points, descriptor);

		detector.detect(image, key_points);
		extractor.compute(image, key_points, descriptor);
	
		//特征点过少，则排除该图像
		if (key_points.size() <= 10) continue;

		key_points_for_all.push_back(key_points);
		descriptor_for_all.push_back(descriptor);

		vector<Vec3b> colors(key_points.size());
		for (int i = 0; i < key_points.size(); ++i)
		{
			Point2f& p = key_points[i].pt;
			colors[i] = image.at<Vec3b>(p.y, p.x);
		}
		colors_for_all.push_back(colors);

		CString keyNum; 
		keyNum.Format("%d", key_points.size());
		m_info = "     共提取：";
		m_info += keyNum;
		m_info += "个特征点；";
		m_info += "\r\n";
		flushEdit1();

		//drawKeypoints(image, key_points, image, Scalar(255, 255, 0),DrawMatchesFlags::DRAW_RICH_KEYPOINTS);
		drawKeypoints(image, key_points, afterExtractImage[it_count], Scalar::all(-1), DrawMatchesFlags::DRAW_RICH_KEYPOINTS);
		
		if (it_count == 0)
		{
			DrawPicToHDC(afterExtractImage[0], IDC_PIC1);
		}

		it_count++;
	}
	//下一张读取的是第2张
	extractnum = 1;
}

void CMy3DRecWindowDlg::OnBnClickedButtonNextimage()
{
	// TODO:  在此添加控件通知处理程序代码
	if (imageShowType == 1)
	{
		int n = picfullnames.size();
		CString cs1;
		string s1;
		Mat image1;
		if (imagenum < n)
		{
			cs1 = picfullnames[imagenum];
			s1 = cs1.GetBuffer(1);
			image1 = imread(s1);
			DrawPicToHDC(image1, IDC_PIC1);
			imagenum++;
		}
		else
		{
			imagenum = 0;
			cs1 = picfullnames[imagenum];
			s1 = cs1.GetBuffer(1);
			image1 = imread(s1);
			DrawPicToHDC(image1, IDC_PIC1);
			imagenum++;
		}
	}

	else if (imageShowType == 2)
	{
		int m = picfullnames.size();
		Mat image2;
		if (extractnum < m)
		{
			image2 = afterExtractImage[extractnum];
			DrawPicToHDC(image2, IDC_PIC1);
			extractnum++;
		}
		else
		{
			extractnum = 0;
			image2 = afterExtractImage[extractnum];
			DrawPicToHDC(image2, IDC_PIC1);
			extractnum++;
		}
	}

}


//显示模式转换
void CMy3DRecWindowDlg::OnBnClickedButtonModlechange()
{
	// TODO:  在此添加控件通知处理程序代码
	if (imageShowType == 1)
	{
		imageShowType = 2;
		extractnum = imagenum;
	}

	else if (imageShowType == 2)
	{
		imageShowType = 1;
		imagenum = extractnum;
	}

}



void CMy3DRecWindowDlg::OnBnClickedButtonExtract()
{
	
	imageShowType = 2;

	//实时刷新显示
	m_info = "************ Step 2 ************";
	m_info += "\r\n";
	m_info += "提取特征点：";
	m_info += "\r\n";

	//先清空上一步的info
	CString nul = "";
	this->GetDlgItem(IDC_EDIT1)->SendMessage(EM_REPLACESEL, NULL, (LPARAM)(LPTSTR)(LPCTSTR)nul);
	flushEdit1();

	long t1 = GetTickCount();
	extract_features(picfullnames, key_points_for_all, descriptor_for_all, colors_for_all);
	long t2 = GetTickCount();//程序段结束后取得系统运行时间(s)  
	CString str;
	str.Format("%.2f s", (float(t2 - t1))/1000);//前后之差即程序运行时间   

	m_info = "特征点提取结束！";
	m_info += "\r\n";
	m_info += "用时：  ";
	m_info += str;
	m_info += "\r\n";
	m_info += "\r\n";
	flushEdit1();
}


void CMy3DRecWindowDlg::match_features(Mat& query, Mat& train, vector<DMatch>& matches)
{
	vector<vector<DMatch>> knn_matches;
	BFMatcher matcher(NORM_L2);
	matcher.knnMatch(query, train, knn_matches, 2);

	//获取满足Ratio Test的最小匹配的距离
	float min_dist = FLT_MAX;
	for (int r = 0; r < knn_matches.size(); ++r)
	{
		//Ratio Test
		if (knn_matches[r][0].distance > 0.6*knn_matches[r][1].distance)
			continue;

		float dist = knn_matches[r][0].distance;
		if (dist < min_dist) min_dist = dist;
	}

	matches.clear();
	for (size_t r = 0; r < knn_matches.size(); ++r)
	{
		//排除不满足Ratio Test的点和匹配距离过大的点
		if (
			knn_matches[r][0].distance > 0.6*knn_matches[r][1].distance ||
			knn_matches[r][0].distance > 5 * max(min_dist, 10.0f)
			)
			continue;

		//保存匹配点
		matches.push_back(knn_matches[r][0]);
	}
}

void CMy3DRecWindowDlg::match_features(vector<Mat>& descriptor_for_all, vector<vector<DMatch>>& matches_for_all)
{
	matches_for_all.clear();
	// n个图像，两两顺次有 n-1 对匹配
	// 1与2匹配，2与3匹配，3与4匹配，以此类推
	for (int i = 0; i < descriptor_for_all.size() - 1; ++i)
	{
		//cout << "Matching images " << i << " - " << i + 1 << endl;
		CString str_i;
		str_i.Format("%d", i);
		CString str_ii;
		str_ii.Format("%d", i+1);


		m_info = "正在匹配图像";
		m_info += str_i;
		m_info += "----";
		m_info += str_ii;
		m_info += "……";
		m_info += "\r\n";
		flushEdit1();

		vector<DMatch> matches;
		match_features(descriptor_for_all[i], descriptor_for_all[i + 1], matches);
		matches_for_all.push_back(matches);

		CString matchNum;
		matchNum.Format("%d", matches.size());
		m_info = "     共有：";
		m_info += matchNum;
		m_info += "个匹配";
		m_info += "\r\n";
		flushEdit1();
	}

}


void CMy3DRecWindowDlg::drawmatch(int i)
{
	CString cs0 = picfullnames[i];
	string s0 = cs0.GetBuffer(0);
	Mat image0 = imread(s0);
	resizeImage(image0);

	CString cs1 = picfullnames[i+1];
	string s1 = cs1.GetBuffer(0);
	Mat image1 = imread(s1);
	resizeImage(image1);

	drawMatches(image0, key_points_for_all[i], image1, key_points_for_all[i+1], matches_for_all[i],
		afterMatchImage[i], Scalar::all(-1));
}


void CMy3DRecWindowDlg::OnBnClickedButtonMatchimage()
{
	// TODO:  在此添加控件通知处理程序代码
	dlg.DoModal();
}


void CMy3DRecWindowDlg::OnBnClickedButtonMatch()
{
	
	// TODO: 在此添加控件通知处理程序代码
	//实时刷新显示
	m_info = "************ Step 3 ************";
	m_info += "\r\n";
	m_info += "特征点匹配：";
	m_info += "\r\n";

	//先清空上一步的info
	//GetDlgItem(IDC_EDIT1)->SetWindowText(_T(""));
	CString nul = "";
	this->GetDlgItem(IDC_EDIT1)->SendMessage(EM_REPLACESEL, NULL, (LPARAM)(LPTSTR)(LPCTSTR)nul);
	flushEdit1();

	long t1 = GetTickCount();
	match_features(descriptor_for_all, matches_for_all);

	//把match结果传递给ImageMatch Dialog
	m_info = "match_images传递中……";
	m_info += "\r\n";
	flushEdit1();
	int n = matches_for_all.size();
	dlg.matchsize = n;
	for (int i = 0; i < n; i++)
	{
		drawmatch(i);
		dlg.matchImage[i] = this->afterMatchImage[i];
	}
	m_info = "match_images传递完毕！";
	m_info += "\r\n";
	flushEdit1();

	long t2 = GetTickCount();//程序段结束后取得系统运行时间(s)  
	CString str;
	str.Format("%.2f s", (float(t2 - t1))/1000);//前后之差即程序运行时间 

	m_info = "特征点匹配结束！";
	m_info += "\r\n";
	m_info += "用时：  ";
	m_info += str;
	m_info += "\r\n";
	m_info += "\r\n";
	flushEdit1();

}



//**************************************************************************************************************************************************************************
//**************************************************************************************************************************************************************************




/*函数作用：在指定文件每行前面加路径
实现思路： 考虑到文件内容可能比较大，
不大可能全部读取到内存中，添加好行号之后再输出
因此采用建立临时文件，边读边写，最后删除原文件
重命名临时文件的办法来实现
参数:strFileName,需要转化的文件名路径
*/
int CMy3DRecWindowDlg::appendPathName(string strFileName)
{
	//输入文件流，用于读取文件内容
	ifstream ifReader;
	//以输入方式打开文件
	ifReader.open(strFileName.c_str(), ios::in);
	if (!ifReader.good())
	{
		//如果打开文件失败，提示错误并退出
		cout << "Can not open source file:" << strFileName << endl;
		return -1;
	}
	//输出文件流
	ofstream ofWriter;
	//原文件名后面加.tmp作为临时文件名
	string strTmpFile = strFileName + ".tmp";
	//以输出方式打开临时文件
	ofWriter.open(strTmpFile.c_str(), ios::out);
	if (!ofWriter.good())
	{
		//如果打开失败，提示错误
		cout << "Can not open tmp file." << endl;
		return -1;
	}
	string strText;//用于保存原文件每行内容
	//int i = 0;//保存行号，从0开始
	//一行行读取原文件，直到结束
	while (getline(ifReader, strText))
	{
		//在原文每行前加行号和空格，并输出
		ofWriter << picPath2 << strText << endl;
		//行号加1
	//	++i;
	}
	//关闭原文件和临时文件
	ifReader.close();
	ofWriter.close();
	//尝试删除原文件，如果失败，提示错误并退出
	if (0 != unlink(strFileName.c_str()))
	{
		cout << "Remove file failed." << endl;
		return -1;
	}
	//尝试重命名临时文件，如果失败，提示错误并退出
	if (0 != rename(strTmpFile.c_str(), strFileName.c_str()))
	{
		cout << "Rename file failed." << endl;
		return -1;
	}
	return 0;
}


void CMy3DRecWindowDlg::appendProcess()
{
	
//  1、在list.txt每一行前加上路径，变成list_focal_absolute.txt
	string fileName1 = "list.txt";
	
	if (0 != appendPathName(fileName1))
	{
		m_info = "文件list.txt添加路径失败！";
		m_info += "\r\n";
	}
	else
	{
		m_info = "文件list.txt添加路径成功！";
		m_info += "\r\n";
	}
	flushEdit1();

	
//  2、在imageslist.txt每一行前加上路径，变成list_absolute.txt
	string fileName2 = "imageslist.txt";

	if (0 != appendPathName(fileName2))
	{
		m_info = "文件imageslist.txt添加路径失败！";
		m_info += "\r\n";
	}
	else
	{
		m_info = "文件imageslist.txt添加路径成功！";
		m_info += "\r\n";
	}
	flushEdit1();
}


void CMy3DRecWindowDlg::DeleteFolder(CString sPath)
{
	CFileFind ff;
	BOOL bFound;
	bFound = ff.FindFile(sPath + "*.*");
	while (bFound)
	{
		bFound = ff.FindNextFile();
		CString sFilePath = ff.GetFilePath();
		if (ff.IsDirectory())
		{
			if (!ff.IsDots())
			{
				DeleteFolder(sFilePath);
			}
		}
		else
		{
			if (ff.IsReadOnly())
			{
				SetFileAttributes(sFilePath, FILE_ATTRIBUTE_NORMAL);
			}
			DeleteFile(sFilePath);
		}
	}
	ff.Close();
	//上面已经把文件夹下面的文件全部删除了，如果需要把文件夹也删除掉则加上一下代码  
	//  SetFileAttributes(sPath, FILE_ATTRIBUTE_NORMAL);  //设置文件夹的属性  
	//  RemoveDirectory(sPath);  //删除文件夹  
}


//删除整个文件夹，注意路径后面不带//
void CMy3DRecWindowDlg::DeleteFullFolder(CString sPath)
{
	CFileFind ff;
	BOOL bFound;
	bFound = ff.FindFile(sPath + "//*.*");
	while (bFound)
	{
		bFound = ff.FindNextFile();
		CString sFilePath = ff.GetFilePath();
		if (ff.IsDirectory())
		{
			if (!ff.IsDots())
			{
				DeleteFolder(sFilePath);
			}
		}
		else
		{
			if (ff.IsReadOnly())
			{
				SetFileAttributes(sFilePath, FILE_ATTRIBUTE_NORMAL);
			}
			DeleteFile(sFilePath);
		}
	}
	ff.Close();
	//上面已经把文件夹下面的文件全部删除了，如果需要把文件夹也删除掉则加上一下代码  
	SetFileAttributes(sPath, FILE_ATTRIBUTE_NORMAL);  //设置文件夹的属性  
	RemoveDirectory(sPath);  //删除文件夹  
}

//把mainpath里的所有文档都移动到bundle_tmp文件夹中，看起来整洁
void CMy3DRecWindowDlg::moveFileToOtherPath(CString &oldpath, CString &newpath, CString &fengefu)
{
	CFileFind finder;
	CString szOldTextPath = oldpath + fengefu;
	CString szNewTextPath;

	BOOL bWorking = finder.FindFile(szOldTextPath);
	while (bWorking)
	{
		bWorking = finder.FindNextFile();
		CString str = finder.GetFileName();
		if (str == "options.txt" || str == "ReadMe.txt")
			continue;
		else
		{
			szNewTextPath = newpath + str;
			MoveFile(finder.GetFilePath(), szNewTextPath);
		}
	}
}

void CMy3DRecWindowDlg::ShellRunFun(CString exedir, CString param)
{
	memset(&ShellInfo, 0, sizeof(ShellInfo));
	ShellInfo.cbSize = sizeof(ShellInfo);
	ShellInfo.fMask = SEE_MASK_NOCLOSEPROCESS;
	ShellInfo.hwnd = NULL;
	//ShellInfo.lpVerb = _T("open");
	ShellInfo.lpVerb = NULL;
	ShellInfo.lpFile = exedir;
	ShellInfo.nShow = SW_SHOWNORMAL;
	//ShellInfo.lpParameters = "";
	ShellInfo.lpParameters = param;
	ShellInfo.lpDirectory = NULL;
	ShellInfo.hInstApp = NULL;
	ShellExecuteEx(&ShellInfo);
	WaitForSingleObject(ShellInfo.hProcess, INFINITE);
	//关闭ShellInfo
	if (ShellInfo.hProcess != NULL)
	{
		TerminateProcess(ShellInfo.hProcess, 0);
		ShellInfo.hProcess = NULL;
	}
}

//提取出像素到list.txt中
void CMy3DRecWindowDlg::extractFocal_exe()
{
	CString cmdstart = "cmd /c ";
	CString exedir = mainPath + "bin\\BundlerFocalExtractor\\BundlerFocalExtractor2.exe ";
	CString str1 = picPath2;  //picPath2 = "C:\\Users\\pan\\Desktop\\ImagesTo3D\\Imgs\\";

	CString str3 = " imageslist.txt";
	CString str4 = " list.txt";

	CString syscmd = cmdstart + exedir + str1 + str3 + str4;

	CString  param = str1 + str3 + str4;

	ShellRunFun(exedir, param);
	
}



void CMy3DRecWindowDlg::bundlerMatcher_exe()
{


	//var bundlerMatcherOutput = shell.run("bin\\BundlerMatcher\\BundlerMatcher.exe " + inputPath + " list.txt gpu.matches.txt " + matchThreshold + " " + firstOctave + trackingBin + sequenceMatching, windowStyle, waitOnReturn);

	CString cmdstart = "cmd /c ";
	CString exedir = mainPath + "bin\\BundlerMatcher\\BundlerMatcher.exe ";
	CString str1 = picPath2;  //picPath2 = "C:\\Users\\pan\\Desktop\\ImagesTo3D\\Imgs\\";
	CString str2 = "imageslist.txt";
	CString str3 = "gpu.matches.txt";
	/*CString str4 = "0.7 ";
	CString str5 = "-1";*/
	CString str4 = m_matchthreshold;
	CString str5 = m_firstOctave;


	//	CString syscmd = cmdstart + exedir + str1 + _T(" ") + str2 + _T(" ") + str3 + _T(" ") + str4 + _T(" ") + str5;
	//	WinExec(LPCSTR(syscmd), SW_NORMAL);
	CString  param = str1 + _T(" ") + str2 + _T(" ") + str3 + _T(" ") + str4 + _T(" ") + str5;
	ShellRunFun(exedir, param);

}

void CMy3DRecWindowDlg::bundler_exe()
{
	//	var bundlerOutput = shell.Exec("bin\\Bundler\\bundler.exe list_focal_absolute.txt --options_file options.txt");
	CString cmdstart = "cmd /c ";
	CString exedir = mainPath + "bin\\Bundler\\bundler.exe ";
	CString str1 = "list.txt ";
	CString str2 = "--options_file ";
	CString str3 = "options.txt";

	CString  param = str1 + str2 + str3;

	ShellRunFun(exedir, param);
}



void CMy3DRecWindowDlg::Bundle2PMVS_exe()
{
	//shell.run("bin\\Bundler\\Bundle2PMVS.exe " + bundler_tmp + "list_absolute.txt " + bundler_output + "bundle.out output", windowStyle, waitOnReturn);
	CString cmdstart = "cmd /c ";
	CString exedir = mainPath + "bin\\Bundler\\Bundle2PMVS.exe ";

	CString str1 = tmpFolderPath + "imageslist.txt ";
	CString str2 = outFolderPath + "bundle.out ";
	CString str3 = "output";

	CString  param = str1 + str2 + str3;

	ShellRunFun(exedir, param);
	
}

void CMy3DRecWindowDlg::RadialUndistort_exe()
{
	
	//shell.run("bin\\Bundler\\RadialUndistort.exe " + bundler_tmp + "list_absolute.txt " + bundler_output + "bundle.out output", windowStyle, waitOnReturn);
	
	CString cmdstart = "cmd /c ";
	CString exedir = mainPath + "bin\\Bundler\\RadialUndistort.exe ";

	CString str1 = tmpFolderPath + "imageslist.txt ";
	CString str2 = outFolderPath + "bundle.out ";
	CString str3 = "output";

	CString  param = str1 + str2 + str3;

	ShellRunFun(exedir, param);
}


void CMy3DRecWindowDlg::Bundle2Vis_exe()
{
//	shell.run("bin\\Bundler\\Bundle2Vis.exe output\\bundle.rd.out output\\vis.dat", windowStyle, waitOnReturn);
	CString cmdstart = "cmd /c ";
	CString exedir = mainPath + "bin\\Bundler\\Bundle2Vis.exe ";

	CString str1 = "output\\bundle.rd.out ";
	CString str2 = "output\\vis.dat";
	
	CString  param = str1 + str2 ;

	ShellRunFun(exedir, param);


}


void CMy3DRecWindowDlg::cmvs_exe()
{

//	shell.run("bin\\CMVS\\CMVS.exe " + inputPath + "pmvs\\ " + maxNbImage + " " + nbCPU, windowStyle, waitOnReturn);
	CString cmdstart = "cmd /c ";
	CString exedir = mainPath + "bin\\CMVS\\CMVS.exe ";
	
	CString str1 = picPath2 + "pmvs\\ ";
	//CString str2 = "100 32";

	CString str2 = "100 ";
	CString str3 = "32 ";	

	//CString syscmd = cmdstart + exedir + str1 + str2 + _T(" ") + str3;
	CString  param = str1 + str2 + str3;
	ShellRunFun(exedir, param);
	
}

void CMy3DRecWindowDlg::genOption_exe()
{
	//shell.run("bin\\CMVS\\GenOption.exe " + inputPath + "pmvs\\ " + level + " " + csize + " " + threshold + " " + wsize + " " + minNbImage + " " + nbCPU, windowStyle, waitOnReturn);
	CString cmdstart = "cmd /c ";
	CString exedir = mainPath + "bin\\CMVS\\GenOption.exe ";

	CString str1 = picPath2 + "pmvs\\ ";

	//CString str2 = "1 2 0.7 7 3 4";

	CString str6 = m_lever;
	CString str7 = m_csize;
	CString str8 = m_threshold;
	CString str9 = m_wsize;
	CString str10 = m_minNbImage;
	CString str11 = m_nbCPU;
	
	CString str2 = str6 + _T(" ") + str7 + _T(" ") + str8 + _T(" ") + str9 + _T(" ") + str10 + _T(" ") + str11;
	
	CString  param = str1 + str2;
	ShellRunFun(exedir, param);
	

}

void CMy3DRecWindowDlg::pmvs2_exe()
{
	
	//bin\\PMVS2\\PMVS2.exe " + inputPath + "pmvs\\ " + optionFilename
	CString cmdstart = "cmd /c ";
	CString exedir = mainPath + "bin\\PMVS2\\PMVS2.exe ";
	CString str1 = picPath2 + "pmvs\\ ";
	CString str2 = "option-0000";


	CString  param = str1 + str2;
	ShellRunFun(exedir, param);

}


void CMy3DRecWindowDlg::createPMVSFolder()
{
//	DeleteFullFolder(picPath2 + "pmvs");
	
	bool flag = CreateDirectory(picPath2+"pmvs", NULL);
		
	CString old1 = mainPath + "output\\";
	CString new1 = picPath2 + "pmvs\\";
	CString fen1 = "pmvs_options.txt";

	//DeleteFolder(new1);//先删除所有文件
	moveFileToOtherPath(old1, new1, fen1);

	flag = CreateDirectory(picPath2 + "pmvs\\txt", NULL);
	flag = CreateDirectory(picPath2 + "pmvs\\visualize", NULL);
	flag = CreateDirectory(picPath2 + "pmvs\\models", NULL);

	CString fen2 = "vis.dat";
	moveFileToOtherPath(old1, new1, fen2);

	CString fen5 = "list.rd.txt";
	moveFileToOtherPath(old1, new1, fen5);

	CString fen6 = "pmvs_options.txt";
	moveFileToOtherPath(old1, new1, fen6);

	CString fen7 = "bundle.rd.out";
	moveFileToOtherPath(old1, new1, fen7);

	CString fen3 = "*.txt";
	CString new3 = picPath2 + "pmvs\\txt\\";

	//DeleteFolder(new3);//先删除所有文件
	moveFileToOtherPath(old1, new3, fen3);
	

	CString fen4 = "*.jpg";
	CString new4 = picPath2 + "pmvs\\visualize\\";

	//DeleteFolder(new4);//先删除所有文件
	moveFileToOtherPath(old1, new4, fen4);

}

void CMy3DRecWindowDlg::OnBnClickedButtonSfm()
{
	// TODO: 在此添加控件通知处理程序代码	

	//实时刷新显示
	m_info = "************ Step 4 ************";
	m_info += "\r\n";
	m_info += "稀疏点云生成：";
	m_info += "\r\n";
	//先清空上一步的info
	CString nul = "";
	this->GetDlgItem(IDC_EDIT1)->SendMessage(EM_REPLACESEL, NULL, (LPARAM)(LPTSTR)(LPCTSTR)nul);
	flushEdit1();

	//MessageBox(m_matchthreshold);

	long t1 = GetTickCount();

	//创建文件夹 bundler_tmp 和 bundler_output

	bool flag = CreateDirectory("output", NULL);
	flag = CreateDirectory(picPath2 + "bundler_tmp", NULL);
	flag = CreateDirectory(picPath2 + "bundler_output", NULL);

	DeleteFolder("output\\");
	DeleteFolder(picPath2 + "bundler_tmp\\");
	DeleteFolder(picPath2 + "bundler_output\\");

	//BundlerFocalExtractor提取出像素到list.txt中
	m_info = "BundlerFocalExtractor提取图片Exif到list.txt中……";
	m_info += "\r\n";
	flushEdit1();
	extractFocal_exe();
	m_info = "BundlerFocalExtractor提取完毕!";
	m_info += "\r\n";
	flushEdit1();

	
	//BundlerMatcher特征点匹配
	m_info = "BundlerMatcher执行中……";
	m_info += "\r\n";
	flushEdit1();
			//参数对话框弹出设置参数
	Dlg_BundlerMatcher dlg;
	dlg.DoModal();
	m_info = "  参数1:matchthreshold设置为:";
	m_info += m_matchthreshold;
	m_info += "\r\n";
	m_info += "  参数2:firstOctave设置为:";
	m_info += m_firstOctave;
	m_info += "\r\n";
	flushEdit1();

	bundlerMatcher_exe();
	m_info = "BundlerMatcher执行完毕！";
	m_info += "\r\n";
	flushEdit1();


	//给list文件添加路径
	appendProcess();
	//Sleep(1000);

	//bundler运行
	m_info = "bundler执行中……";
	m_info += "\r\n";
	flushEdit1();
	bundler_exe();
	m_info = "bundler执行完毕！";
	m_info += "\r\n";
	flushEdit1();

	Sleep(1000);

	tmpFolderPath = picPath2 + "bundler_tmp\\";
	outFolderPath = picPath2 + "bundler_output\\";

	DeleteFolder(tmpFolderPath);  //先删除所有文件
	CString fen1 = "*.txt";
	moveFileToOtherPath(mainPath, tmpFolderPath, fen1);

	DeleteFolder(outFolderPath);
	CString fen2 = "*.*";
	CString oldpt = mainPath + "output//";   //先删除所有文件
	moveFileToOtherPath(oldpt, outFolderPath, fen2);
	

	CString strSourceDir = outFolderPath + "bundle.out";
	CString sparsePointNum = readSparsePointNum(strSourceDir);

	long t2 = GetTickCount();//程序段结束后取得系统运行时间(s)  
	CString str;
	str.Format("%.2f s", (float(t2 - t1))/1000);//前后之差即程序运行时间   
	m_info = "稀疏点云生成结束！";
	m_info += "\r\n";
	m_info += "共稠密重建出三维点：";
	m_info += sparsePointNum+" 个";
	m_info += "\r\n";
	m_info += "用时：  ";
	m_info += str;
	m_info += "\r\n";
	m_info += "\r\n";
	flushEdit1();

}


void CMy3DRecWindowDlg::OnBnClickedButton5()
{
	// TODO:  在此添加控件通知处理程序代码
	
	//实时刷新显示
	m_info = "************ Step 5 ************";
	m_info += "\r\n";
	m_info += "稠密点云生成：";
	m_info += "\r\n";
	//先清空上一步的info
	CString nul = "";
	this->GetDlgItem(IDC_EDIT1)->SendMessage(EM_REPLACESEL, NULL, (LPARAM)(LPTSTR)(LPCTSTR)nul);
	flushEdit1();
	long t1 = GetTickCount();

	m_info = "Bundle2PMVS执行中……";
	m_info += "\r\n";
	flushEdit1();
	Bundle2PMVS_exe();
	m_info = "Bundle2PMVS执行完毕!";
	m_info += "\r\n";
	flushEdit1();


	m_info = "RadialUndistort执行中……";
	m_info += "\r\n";
	flushEdit1();
	RadialUndistort_exe();
	m_info = "RadialUndistort执行完毕!";
	m_info += "\r\n";
	flushEdit1();

	
	m_info = "Bundle2Vis执行中……";
	m_info += "\r\n";
	flushEdit1();
	Bundle2Vis_exe();
	m_info = "Bundle2Vis执行完毕!";
	m_info += "\r\n";
	flushEdit1();

	//在IMG文件夹下创建PMVS文件夹
	createPMVSFolder();
	Sleep(1000);
	
	m_info = "cmvs执行中……";
	m_info += "\r\n";
	flushEdit1();
	cmvs_exe();
	m_info = "cmvs执行完毕!";
	m_info += "\r\n";
	flushEdit1();


	m_info = "genOption执行中……";
	m_info += "\r\n";
	flushEdit1();
			//参数对话框弹出设置参数
	Dlg_genOption dlg2;
	dlg2.DoModal();
	m_info = "  参数1:lever设置为:";
	m_info += m_lever;
	m_info += "\r\n";
	m_info += "  参数2:csize设置为:";
	m_info += m_csize;
	m_info += "\r\n";
	m_info += "  参数3:threshold设置为:";
	m_info += m_threshold;
	m_info += "\r\n";
	m_info += "  参数4:cwize设置为:";
	m_info += m_wsize;
	m_info += "\r\n";
	m_info += "  参数5:minNbImage设置为:";
	m_info += m_minNbImage;
	m_info += "\r\n";
	m_info += "  参数6:nbCPU设置为:";
	m_info += m_nbCPU;
	m_info += "\r\n";
	flushEdit1();

	genOption_exe();
	m_info = "genOption执行完毕!";
	m_info += "\r\n";
	flushEdit1();


	m_info = "pmvs2执行中……";
	m_info += "\r\n";
	flushEdit1();
	pmvs2_exe();
	m_info = "pmvs2执行完毕!";
	m_info += "\r\n";
	flushEdit1();

	CString plyPath = picPath2 + "pmvs\\models\\option-0000.ply";
	char *ply = (LPSTR)(LPCSTR)plyPath;
	plyLoader.LoadModel(ply);
	int num = plyLoader.AllPointNums();
	CString strnump;
	strnump.Format("%d 个", num);


	long t2 = GetTickCount();//程序段结束后取得系统运行时间(s)  
	CString str;
	str.Format("%.2f s", (float(t2 - t1))/1000);//前后之差即程序运行时间   



	m_info = "稠密点云生成结束！";
	m_info += "\r\n";
	m_info += "共稠密重建出三维点：";
	m_info += strnump;
	m_info += "\r\n";
	m_info += "用时：  ";
	m_info += str;
	m_info += "\r\n";	
	m_info += "\r\n";
	flushEdit1();
}


//Meshlab点云处理，清除不必要的点云，并进行泊松重建，以及面片的重采样，为贴纹理做准备，最后保存为mesh.ply
//为了保存处理后的点云，可以在处理完后保存为option-fin.ply
//Meshlab处理的具体步骤如下：
/*
1.[Filters] -->[Remeshing, Simplification, Reconstruction] -->[Surface reconstruction : Poisson]
2. Set Octree Depth to 8, click Apply, then Close
3.[Filters] -->[Selection] -->[Select Faces with edges longer than]
4. Set the Edge Threshold to around 0.4.You should see the large errooneous faces highlighted in red.Click Close.
5. Hit Shift + Delete, to delete the faces and vertices
6.[Filters] -->[Remeshing, Simplification, Reconstruction] -->[Quadric Edge Collapse Decimation]
7. Choose the desired 'Target number of faces'.I typically experiment with 1 % -10 % of the total faces.
8. Tick 'Preserve Topology', tick 'Planar Simplification', click Apply, click Close
9.[File] -->[Export Mesh As], save as 'mesh.ply'
10. Untick 'Binary encoding', click OK ,注意这里是不要选Binary encoding
*/

void CMy3DRecWindowDlg::pointClear_exe()
{
	CString cmdstart = "cmd /c ";
	CString exedir = mainPath + "bin\\MeshLab\\meshlab.exe ";
	
	CString str = picPath2 + "pmvs\\bundle.rd.out";

	CString  param = str;
	//ShellRunFun(exedir, param);
	
	CString syscmd1 = cmdstart + exedir + str;
	WinExec(LPCSTR(syscmd1), SW_HIDE);
	
}

void CMy3DRecWindowDlg::OnBnClickedButtonPointclear()
{
	// TODO:  在此添加控件通知处理程序代码
	//Meshlab点云清理

	//实时刷新显示
	m_info = "************ Step 6 ************";
	m_info += "\r\n";
	m_info += "点云清理：";
	m_info += "\r\n";
	//先清空上一步的info
	CString nul = "";
	this->GetDlgItem(IDC_EDIT1)->SendMessage(EM_REPLACESEL, NULL, (LPARAM)(LPTSTR)(LPCTSTR)nul);
	flushEdit1();
	long t1 = GetTickCount();

	m_info = "MeshLab手动点云清理和泊松重建中……";
	m_info += "\r\n";
	flushEdit1();
	pointClear_exe();
	m_info = "MeshLab手动操作完毕!";
	m_info += "\r\n";
	flushEdit1();

	long t2 = GetTickCount();//程序段结束后取得系统运行时间(s)  
	CString str;
	str.Format("%.2f s", (float(t2 - t1))/1000);//前后之差即程序运行时间   
	/*m_info = "用时：  ";
	m_info += str;
	m_info += "\r\n";*/
	m_info = "\r\n";
	flushEdit1();	
}


//添加纹理，根据上一步生成的mesh.ply,进行贴纹理工作，最后生成
/*
-pmvs / models / output.mesh
- pmvs / models / output.point
- pmvs / models / output.obj <--Wavefront OBJ
- pmvs / models / output.mtl <--Wavefront OBJ
其中，output.mesh可以通过ViewMesh.exe观看，output.obj可以通过Meshlab观看
*/

void CMy3DRecWindowDlg::textureMesh_exe()
{
	CString cmdstart = "cmd /c ";
	CString exedir = mainPath + "bin\\TextureMesh\\TextureMesh.exe ";
	CString str1 = picPath2 + "pmvs\\ ";

	CString  param = str1;
	ShellRunFun(exedir, param);
}

void CMy3DRecWindowDlg::OnBnClickedButton6()
{
	//实时刷新显示
	m_info = "************ Step 7 ************";
	m_info += "\r\n";
	m_info += "纹理映射：";
	m_info += "\r\n";
	//先清空上一步的info
	CString nul = "";
	this->GetDlgItem(IDC_EDIT1)->SendMessage(EM_REPLACESEL, NULL, (LPARAM)(LPTSTR)(LPCTSTR)nul);
	flushEdit1();
	long t1 = GetTickCount();

	m_info = "TextureMesh执行中……";
	m_info += "\r\n";
	flushEdit1();
	textureMesh_exe();
	m_info = "TextureMesh执行完毕!";
	m_info += "\r\n";
	flushEdit1();

	long t2 = GetTickCount();//程序段结束后取得系统运行时间(s)  
	CString str;
	str.Format("%.2f s", (float(t2 - t1))/1000);//前后之差即程序运行时间   
	m_info = "用时：  ";
	m_info += str;
	m_info += "\r\n";
	m_info += "\r\n";
	flushEdit1();
}


//三维显示
void CMy3DRecWindowDlg::OnBnClickedButton7()
{
	// TODO:  在此添加控件通知处理程序代码
	
	if (m_pMenuEx)
	{
		delete m_pMenuEx;
	}
	m_pMenuEx = new CMenuEx;

	// create menu by using default param
	m_pMenuEx->CreatePopupMenu(this, 0);

	using namespace MENU_DEF;

	MENU_ITEM item[5];

	for (int i = 0; i<sizeof(item) / sizeof(MENU_ITEM); i++)
	{
		CString str;
		//str.Format("test%d", i);
		if (i == 0) str = "未处理稠密三维点";
		else if (i == 1) str = "处理后稠密三维点";
		else if (i == 2) str = "MeshLab显示三维物体"; 
	    else if (i == 3) str = "ViewMesh显示三维物体";
		item[i] = MENU_ITEM(str, (UINT)(0xE00 + i), (BOOL)TRUE);

	}

	// insert item
	m_pMenuEx->InsertItem(item[0], 0);	// test0 -> 0
	m_pMenuEx->InsertItem(item[1], 1);	// test1 -> 1

	m_pMenuEx->InsertItem(item[2], 2);
	m_pMenuEx->InsertItem(item[3], 3);

	// set font
	m_pMenuEx->SetFont(GetDlgItem(IDC_BUTTON7)->GetFont());

	CRect rct;
	GetDlgItem(IDC_BUTTON7)->GetWindowRect(&rct);
	m_pMenuEx->ShowMenu(rct.BottomRight(), TRUE);
}

void CMy3DRecWindowDlg::OnMenuItem(UINT uItemID)
{
	CString str;
	CString exedir;
	CString param;
	//str.Format(" test%d  clicked", uItemID - 0xE00);
	if (uItemID - 0xE00 == 0)
	{
		CString cmdstart = "cmd /c ";
		exedir = mainPath + "bin\\MeshLab\\meshlab.exe ";

		CString str1 = picPath2 + "pmvs\\models\\option-0000.ply";


		CString syscmd1 = cmdstart + exedir + str1;
		WinExec(LPCSTR(syscmd1), SW_HIDE);
	}

	else if (uItemID - 0xE00 == 1)
	{
		CString cmdstart = "cmd /c ";
		exedir = mainPath + "bin\\MeshLab\\meshlab.exe ";

		CString str1 = picPath2 + "pmvs\\models\\option-fin.ply";


		CString syscmd1 = cmdstart + exedir + str1;
		WinExec(LPCSTR(syscmd1), SW_HIDE);
	}

	else if (uItemID - 0xE00 == 2)
	{
		CString cmdstart = "cmd /c ";
		exedir = mainPath + "bin\\MeshLab\\meshlab.exe ";

		//CString str2 = picPath2 + "pmvs\\models\\models.obj";
		CString str2 = picPath2 + "pmvs\\models\\output.obj";

		CString syscmd2 = cmdstart + exedir + str2;
		WinExec(LPCSTR(syscmd2), SW_HIDE);
	}	

	else if (uItemID - 0xE00 == 3)
	{
		CString cmdstart = "cmd /c ";
		exedir = mainPath + "bin\\ViewMesh\\ViewMesh.exe ";

		CString dir = mainPath + "bin\\ViewMesh\\";
		CString param = picPath2 + "pmvs\\";

		//自动添加models\\output.mesh 文件，此功能需要与TextMesh结合起来。

		
		memset(&ShellInfo, 0, sizeof(ShellInfo));
		ShellInfo.cbSize = sizeof(ShellInfo);
		ShellInfo.fMask = SEE_MASK_NOCLOSEPROCESS;
		ShellInfo.hwnd = NULL;
		//ShellInfo.lpVerb = _T("open");
		ShellInfo.lpVerb = NULL;
		ShellInfo.lpFile = exedir;
		ShellInfo.nShow = SW_SHOWNORMAL;
		//ShellInfo.lpParameters = "";
		ShellInfo.lpParameters = param;
		ShellInfo.lpDirectory = dir;
		ShellInfo.hInstApp = NULL;
		ShellExecuteEx(&ShellInfo);
		WaitForSingleObject(ShellInfo.hProcess, INFINITE);
		//关闭ShellInfo
		if (ShellInfo.hProcess != NULL)
		{
			TerminateProcess(ShellInfo.hProcess, 0);
			ShellInfo.hProcess = NULL;
		}	
	}
}






