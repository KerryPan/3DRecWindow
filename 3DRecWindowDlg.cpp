
// 3DRecWindowDlg.cpp : ʵ���ļ�
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



// ȫ�ֱ���

Dlg_ImageMatch dlg;

HANDLE h1 = NULL;                // �߳̾��
DWORD pid = 0;                   // ����cmd���ڵ�PID
CString inputCmd;     // ����cmd����ͷ��صĽ��
CString inputCmd2;

CString outputCmd;
CEdit *m_edit;                  // �����

int minHessian = 1000;
SurfFeatureDetector detector(minHessian);
SurfDescriptorExtractor extractor;


//sift��������
//SiftFeatureDetector detector(minHessian);
//SiftDescriptorExtractor extractor;


// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

// ʵ��
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


// CMy3DRecWindowDlg �Ի���



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


// CMy3DRecWindowDlg ��Ϣ�������

BOOL CMy3DRecWindowDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// ��������...���˵�����ӵ�ϵͳ�˵��С�

	// IDM_ABOUTBOX ������ϵͳ���Χ�ڡ�
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

	// ���ô˶Ի����ͼ�ꡣ  ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO: �ڴ���Ӷ���ĳ�ʼ������
	//����Ĭ�Ϲ��̵�Ŀ¼

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
	GetClientRect(&rect);     //ȡ�ͻ�����С    
	old.x = rect.right - rect.left;
	old.y = rect.bottom - rect.top;



	m_edit = (CEdit*)GetDlgItem(IDC_EDIT1);
	m_edit->SetLimitText(100000000);


	m_editFont.CreatePointFont(100, "����");	
	m_editctr.SetFont(&m_editFont); // ����������
	

	
	m_buttonFont.CreatePointFont(110, "����");
	GetDlgItem(IDC_BUTTON_OPEN)->SetFont(&m_buttonFont);
	GetDlgItem(IDC_BUTTON_EXTRACT)->SetFont(&m_buttonFont);
	GetDlgItem(IDC_BUTTON_MATCH)->SetFont(&m_buttonFont);
	GetDlgItem(IDC_BUTTON_SFM)->SetFont(&m_buttonFont);
	GetDlgItem(IDC_BUTTON5)->SetFont(&m_buttonFont);
	GetDlgItem(IDC_BUTTON6)->SetFont(&m_buttonFont);
	GetDlgItem(IDC_BUTTON7)->SetFont(&m_buttonFont);
	GetDlgItem(IDC_BUTTON_POINTCLEAR)->SetFont(&m_buttonFont);

	m_groupFont.CreatePointFont(120, "��������");
	GetDlgItem(IDC_GROUP1)->SetFont(&m_groupFont);
	GetDlgItem(IDC_GROUP2)->SetFont(&m_groupFont);
	GetDlgItem(IDC_GROUP3)->SetFont(&m_groupFont);

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
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

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ  ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CMy3DRecWindowDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR CMy3DRecWindowDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CMy3DRecWindowDlg::OnBnClickedCancel()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CDialogEx::OnCancel();
}


int CMy3DRecWindowDlg::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDialogEx::OnCreate(lpCreateStruct) == -1)
		return -1;

	ModifyStyle(0, WS_MAXIMIZEBOX | WS_MINIMIZEBOX, SWP_FRAMECHANGED);
	// TODO:  �ڴ������ר�õĴ�������

	return 0;
}


void CMy3DRecWindowDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);

	// TODO: �ڴ˴������Ϣ����������
	// TODO: �ڴ˴������Ϣ����������
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
	POINT Newp; //��ȡ���ڶԻ���Ĵ�С  
	CRect recta;
	GetClientRect(&recta);     //ȡ�ͻ�����С    
	Newp.x = recta.right - recta.left;
	Newp.y = recta.bottom - recta.top;
	fsp[0] = (float)Newp.x / old.x;
	fsp[1] = (float)Newp.y / old.y;
	CRect Rect;
	int woc;
	CPoint OldTLPoint, TLPoint; //���Ͻ�  
	CPoint OldBRPoint, BRPoint; //���½�  
	HWND  hwndChild = ::GetWindow(m_hWnd, GW_CHILD);  //�г����пؼ�    
	while (hwndChild)
	{
		woc = ::GetDlgCtrlID(hwndChild);//ȡ��ID  
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
/* ���ݽ���ID��PID����ȡ������                                        */
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
	// TODO:  �ڴ������Ϣ�����������/�����Ĭ��ֵ
	// TODO: Add your message handler code here and/or call default

	// �ر�CMD����
	::SendMessage(GetWindowHandleByPID(pid), WM_CLOSE, NULL, NULL);

	// �����߳�
	CloseHandle(h1);

	CDialog::OnClose();
	CDialogEx::OnClose();
}


//********************************�Զ���*****************************************************

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
	cimg.CopyOf(&pimg, 1);           // ����ͼƬ
	cimg.DrawToHDC(hDC, &rect); // ��ͼƬ���Ƶ���ʾ�ؼ���ָ��������
	//ReleaseDC(pDC);
	//DeleteDC(hDC);
}


CString CMy3DRecWindowDlg::getPath()
{
	//��ȡ��ѡ�е��ļ���·��
	CString m_strFileOut = _T("");
	TCHAR szSelected[MAX_PATH];//��������ļ���·��
	BROWSEINFO bi;
	LPITEMIDLIST pidl;
	bi.hwndOwner = this->m_hWnd;
	bi.pidlRoot = NULL;
	bi.pszDisplayName = szSelected;
	bi.lpszTitle = _T("ѡ��ͼƬ�����ļ���");
	bi.ulFlags = BIF_RETURNONLYFSDIRS;
	bi.lpfn = NULL;
	bi.lParam = NULL;
	bi.iImage = NULL;
	if ((pidl = SHBrowseForFolder(&bi)) != NULL)
	{
		if (SUCCEEDED(SHGetPathFromIDList(pidl, szSelected))) //�õ��ļ��е�ȫ·������Ҫ�Ļ���ֻ�ñ��ļ�����
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
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	
	imageShowType = 1;

	CFileDialog dlg(TRUE);

	m_info = "************ Step 1 ************";
	m_info += "\r\n";
	m_info += "��ͼƬ�����ļ��У�";
	m_info += "\r\n";
		
	picPath = getPath();   //picPath = "C:\\Users\\pan\\Desktop\\ImagesTo3D\\Imgs";
	
	picPath2 = picPath + "\\";    //  picPath2 = "C:\\Users\\pan\\Desktop\\ImagesTo3D\\Imgs\\";
	m_info += picPath2;    
	
	int iEndPos = 0;
	iEndPos = m_info.ReverseFind('\\');
	m_info = m_info.Left(iEndPos);
	m_info += "\r\n";
	

	//��ȡͼƬ��ŵ�Ŀ¼picPath�����е�png�ļ������߸ĳ�jpg��ɣ�,����·�����浽����picnames��
	CString fileFormat = "\\*.jpg";
	get_file_names(picPath, picnames, fileFormat);

	vector<CString>::iterator it = picnames.begin();
	for (it; it != picnames.end(); ++it)
	{
		CString cs = picPath + "\\" + *it;
		picfullnames.push_back(cs);
	}

	//��ͼƬ·�����浽imagelist.txt��
	CString filename = "imageslist.txt";
	path_to_txt(mainPath, filename, picfullnames);

	int picNum = picfullnames.size();
	CString Cnum;
	Cnum.Format("%d", picNum);

	
	m_info += "����⵽ͼƬ��";
	m_info += Cnum;
	m_info += "��";
	m_info += "\r\n";

	m_info += "����ͼƬ·���洢�ڣ�";
	m_info += mainPath;
	m_info += filename;
	m_info += "��";
	m_info += "\r\n";
	m_info += "\r\n";

	//�������һ����info
	CString nul = "";
	this->GetDlgItem(IDC_EDIT1)->SendMessage(EM_REPLACESEL, NULL, (LPARAM)(LPTSTR)(LPCTSTR)nul);
	flushEdit1();

	CString cs0 = picfullnames[0];
	string s0 = cs0.GetBuffer(0);
	Mat image0 = imread(s0);	
	DrawPicToHDC(image0, IDC_PIC1);

	//��ʾ��һ��ͼƬʱ��imagenum
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
		m_info = "������ȡͼ��";
		m_info += str_picnum;
		m_info += "�������㡭��";
		m_info += "\r\n";
		flushEdit1();

		vector<KeyPoint> key_points(10000);	

		Mat descriptor;
		Mat mascara;
		//ż�������ڴ����ʧ�ܵĴ���
		//sift->detectAndCompute(image, noArray(), key_points, descriptor);
		//sift1(image, mascara, key_points, descriptor);

		detector.detect(image, key_points);
		extractor.compute(image, key_points, descriptor);
	
		//��������٣����ų���ͼ��
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
		m_info = "     ����ȡ��";
		m_info += keyNum;
		m_info += "�������㣻";
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
	//��һ�Ŷ�ȡ���ǵ�2��
	extractnum = 1;
}

void CMy3DRecWindowDlg::OnBnClickedButtonNextimage()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
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


//��ʾģʽת��
void CMy3DRecWindowDlg::OnBnClickedButtonModlechange()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
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

	//ʵʱˢ����ʾ
	m_info = "************ Step 2 ************";
	m_info += "\r\n";
	m_info += "��ȡ�����㣺";
	m_info += "\r\n";

	//�������һ����info
	CString nul = "";
	this->GetDlgItem(IDC_EDIT1)->SendMessage(EM_REPLACESEL, NULL, (LPARAM)(LPTSTR)(LPCTSTR)nul);
	flushEdit1();

	long t1 = GetTickCount();
	extract_features(picfullnames, key_points_for_all, descriptor_for_all, colors_for_all);
	long t2 = GetTickCount();//����ν�����ȡ��ϵͳ����ʱ��(s)  
	CString str;
	str.Format("%.2f s", (float(t2 - t1))/1000);//ǰ��֮���������ʱ��   

	m_info = "��������ȡ������";
	m_info += "\r\n";
	m_info += "��ʱ��  ";
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

	//��ȡ����Ratio Test����Сƥ��ľ���
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
		//�ų�������Ratio Test�ĵ��ƥ��������ĵ�
		if (
			knn_matches[r][0].distance > 0.6*knn_matches[r][1].distance ||
			knn_matches[r][0].distance > 5 * max(min_dist, 10.0f)
			)
			continue;

		//����ƥ���
		matches.push_back(knn_matches[r][0]);
	}
}

void CMy3DRecWindowDlg::match_features(vector<Mat>& descriptor_for_all, vector<vector<DMatch>>& matches_for_all)
{
	matches_for_all.clear();
	// n��ͼ������˳���� n-1 ��ƥ��
	// 1��2ƥ�䣬2��3ƥ�䣬3��4ƥ�䣬�Դ�����
	for (int i = 0; i < descriptor_for_all.size() - 1; ++i)
	{
		//cout << "Matching images " << i << " - " << i + 1 << endl;
		CString str_i;
		str_i.Format("%d", i);
		CString str_ii;
		str_ii.Format("%d", i+1);


		m_info = "����ƥ��ͼ��";
		m_info += str_i;
		m_info += "----";
		m_info += str_ii;
		m_info += "����";
		m_info += "\r\n";
		flushEdit1();

		vector<DMatch> matches;
		match_features(descriptor_for_all[i], descriptor_for_all[i + 1], matches);
		matches_for_all.push_back(matches);

		CString matchNum;
		matchNum.Format("%d", matches.size());
		m_info = "     ���У�";
		m_info += matchNum;
		m_info += "��ƥ��";
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
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	dlg.DoModal();
}


void CMy3DRecWindowDlg::OnBnClickedButtonMatch()
{
	
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	//ʵʱˢ����ʾ
	m_info = "************ Step 3 ************";
	m_info += "\r\n";
	m_info += "������ƥ�䣺";
	m_info += "\r\n";

	//�������һ����info
	//GetDlgItem(IDC_EDIT1)->SetWindowText(_T(""));
	CString nul = "";
	this->GetDlgItem(IDC_EDIT1)->SendMessage(EM_REPLACESEL, NULL, (LPARAM)(LPTSTR)(LPCTSTR)nul);
	flushEdit1();

	long t1 = GetTickCount();
	match_features(descriptor_for_all, matches_for_all);

	//��match������ݸ�ImageMatch Dialog
	m_info = "match_images�����С���";
	m_info += "\r\n";
	flushEdit1();
	int n = matches_for_all.size();
	dlg.matchsize = n;
	for (int i = 0; i < n; i++)
	{
		drawmatch(i);
		dlg.matchImage[i] = this->afterMatchImage[i];
	}
	m_info = "match_images������ϣ�";
	m_info += "\r\n";
	flushEdit1();

	long t2 = GetTickCount();//����ν�����ȡ��ϵͳ����ʱ��(s)  
	CString str;
	str.Format("%.2f s", (float(t2 - t1))/1000);//ǰ��֮���������ʱ�� 

	m_info = "������ƥ�������";
	m_info += "\r\n";
	m_info += "��ʱ��  ";
	m_info += str;
	m_info += "\r\n";
	m_info += "\r\n";
	flushEdit1();

}



//**************************************************************************************************************************************************************************
//**************************************************************************************************************************************************************************




/*�������ã���ָ���ļ�ÿ��ǰ���·��
ʵ��˼·�� ���ǵ��ļ����ݿ��ܱȽϴ�
�������ȫ����ȡ���ڴ��У���Ӻ��к�֮�������
��˲��ý�����ʱ�ļ����߶���д�����ɾ��ԭ�ļ�
��������ʱ�ļ��İ취��ʵ��
����:strFileName,��Ҫת�����ļ���·��
*/
int CMy3DRecWindowDlg::appendPathName(string strFileName)
{
	//�����ļ��������ڶ�ȡ�ļ�����
	ifstream ifReader;
	//�����뷽ʽ���ļ�
	ifReader.open(strFileName.c_str(), ios::in);
	if (!ifReader.good())
	{
		//������ļ�ʧ�ܣ���ʾ�����˳�
		cout << "Can not open source file:" << strFileName << endl;
		return -1;
	}
	//����ļ���
	ofstream ofWriter;
	//ԭ�ļ��������.tmp��Ϊ��ʱ�ļ���
	string strTmpFile = strFileName + ".tmp";
	//�������ʽ����ʱ�ļ�
	ofWriter.open(strTmpFile.c_str(), ios::out);
	if (!ofWriter.good())
	{
		//�����ʧ�ܣ���ʾ����
		cout << "Can not open tmp file." << endl;
		return -1;
	}
	string strText;//���ڱ���ԭ�ļ�ÿ������
	//int i = 0;//�����кţ���0��ʼ
	//һ���ж�ȡԭ�ļ���ֱ������
	while (getline(ifReader, strText))
	{
		//��ԭ��ÿ��ǰ���кźͿո񣬲����
		ofWriter << picPath2 << strText << endl;
		//�кż�1
	//	++i;
	}
	//�ر�ԭ�ļ�����ʱ�ļ�
	ifReader.close();
	ofWriter.close();
	//����ɾ��ԭ�ļ������ʧ�ܣ���ʾ�����˳�
	if (0 != unlink(strFileName.c_str()))
	{
		cout << "Remove file failed." << endl;
		return -1;
	}
	//������������ʱ�ļ������ʧ�ܣ���ʾ�����˳�
	if (0 != rename(strTmpFile.c_str(), strFileName.c_str()))
	{
		cout << "Rename file failed." << endl;
		return -1;
	}
	return 0;
}


void CMy3DRecWindowDlg::appendProcess()
{
	
//  1����list.txtÿһ��ǰ����·�������list_focal_absolute.txt
	string fileName1 = "list.txt";
	
	if (0 != appendPathName(fileName1))
	{
		m_info = "�ļ�list.txt���·��ʧ�ܣ�";
		m_info += "\r\n";
	}
	else
	{
		m_info = "�ļ�list.txt���·���ɹ���";
		m_info += "\r\n";
	}
	flushEdit1();

	
//  2����imageslist.txtÿһ��ǰ����·�������list_absolute.txt
	string fileName2 = "imageslist.txt";

	if (0 != appendPathName(fileName2))
	{
		m_info = "�ļ�imageslist.txt���·��ʧ�ܣ�";
		m_info += "\r\n";
	}
	else
	{
		m_info = "�ļ�imageslist.txt���·���ɹ���";
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
	//�����Ѿ����ļ���������ļ�ȫ��ɾ���ˣ������Ҫ���ļ���Ҳɾ���������һ�´���  
	//  SetFileAttributes(sPath, FILE_ATTRIBUTE_NORMAL);  //�����ļ��е�����  
	//  RemoveDirectory(sPath);  //ɾ���ļ���  
}


//ɾ�������ļ��У�ע��·�����治��//
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
	//�����Ѿ����ļ���������ļ�ȫ��ɾ���ˣ������Ҫ���ļ���Ҳɾ���������һ�´���  
	SetFileAttributes(sPath, FILE_ATTRIBUTE_NORMAL);  //�����ļ��е�����  
	RemoveDirectory(sPath);  //ɾ���ļ���  
}

//��mainpath��������ĵ����ƶ���bundle_tmp�ļ����У�����������
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
	//�ر�ShellInfo
	if (ShellInfo.hProcess != NULL)
	{
		TerminateProcess(ShellInfo.hProcess, 0);
		ShellInfo.hProcess = NULL;
	}
}

//��ȡ�����ص�list.txt��
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

	//DeleteFolder(new1);//��ɾ�������ļ�
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

	//DeleteFolder(new3);//��ɾ�������ļ�
	moveFileToOtherPath(old1, new3, fen3);
	

	CString fen4 = "*.jpg";
	CString new4 = picPath2 + "pmvs\\visualize\\";

	//DeleteFolder(new4);//��ɾ�������ļ�
	moveFileToOtherPath(old1, new4, fen4);

}

void CMy3DRecWindowDlg::OnBnClickedButtonSfm()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������	

	//ʵʱˢ����ʾ
	m_info = "************ Step 4 ************";
	m_info += "\r\n";
	m_info += "ϡ��������ɣ�";
	m_info += "\r\n";
	//�������һ����info
	CString nul = "";
	this->GetDlgItem(IDC_EDIT1)->SendMessage(EM_REPLACESEL, NULL, (LPARAM)(LPTSTR)(LPCTSTR)nul);
	flushEdit1();

	//MessageBox(m_matchthreshold);

	long t1 = GetTickCount();

	//�����ļ��� bundler_tmp �� bundler_output

	bool flag = CreateDirectory("output", NULL);
	flag = CreateDirectory(picPath2 + "bundler_tmp", NULL);
	flag = CreateDirectory(picPath2 + "bundler_output", NULL);

	DeleteFolder("output\\");
	DeleteFolder(picPath2 + "bundler_tmp\\");
	DeleteFolder(picPath2 + "bundler_output\\");

	//BundlerFocalExtractor��ȡ�����ص�list.txt��
	m_info = "BundlerFocalExtractor��ȡͼƬExif��list.txt�С���";
	m_info += "\r\n";
	flushEdit1();
	extractFocal_exe();
	m_info = "BundlerFocalExtractor��ȡ���!";
	m_info += "\r\n";
	flushEdit1();

	
	//BundlerMatcher������ƥ��
	m_info = "BundlerMatcherִ���С���";
	m_info += "\r\n";
	flushEdit1();
			//�����Ի��򵯳����ò���
	Dlg_BundlerMatcher dlg;
	dlg.DoModal();
	m_info = "  ����1:matchthreshold����Ϊ:";
	m_info += m_matchthreshold;
	m_info += "\r\n";
	m_info += "  ����2:firstOctave����Ϊ:";
	m_info += m_firstOctave;
	m_info += "\r\n";
	flushEdit1();

	bundlerMatcher_exe();
	m_info = "BundlerMatcherִ����ϣ�";
	m_info += "\r\n";
	flushEdit1();


	//��list�ļ����·��
	appendProcess();
	//Sleep(1000);

	//bundler����
	m_info = "bundlerִ���С���";
	m_info += "\r\n";
	flushEdit1();
	bundler_exe();
	m_info = "bundlerִ����ϣ�";
	m_info += "\r\n";
	flushEdit1();

	Sleep(1000);

	tmpFolderPath = picPath2 + "bundler_tmp\\";
	outFolderPath = picPath2 + "bundler_output\\";

	DeleteFolder(tmpFolderPath);  //��ɾ�������ļ�
	CString fen1 = "*.txt";
	moveFileToOtherPath(mainPath, tmpFolderPath, fen1);

	DeleteFolder(outFolderPath);
	CString fen2 = "*.*";
	CString oldpt = mainPath + "output//";   //��ɾ�������ļ�
	moveFileToOtherPath(oldpt, outFolderPath, fen2);
	

	CString strSourceDir = outFolderPath + "bundle.out";
	CString sparsePointNum = readSparsePointNum(strSourceDir);

	long t2 = GetTickCount();//����ν�����ȡ��ϵͳ����ʱ��(s)  
	CString str;
	str.Format("%.2f s", (float(t2 - t1))/1000);//ǰ��֮���������ʱ��   
	m_info = "ϡ��������ɽ�����";
	m_info += "\r\n";
	m_info += "�������ؽ�����ά�㣺";
	m_info += sparsePointNum+" ��";
	m_info += "\r\n";
	m_info += "��ʱ��  ";
	m_info += str;
	m_info += "\r\n";
	m_info += "\r\n";
	flushEdit1();

}


void CMy3DRecWindowDlg::OnBnClickedButton5()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	
	//ʵʱˢ����ʾ
	m_info = "************ Step 5 ************";
	m_info += "\r\n";
	m_info += "���ܵ������ɣ�";
	m_info += "\r\n";
	//�������һ����info
	CString nul = "";
	this->GetDlgItem(IDC_EDIT1)->SendMessage(EM_REPLACESEL, NULL, (LPARAM)(LPTSTR)(LPCTSTR)nul);
	flushEdit1();
	long t1 = GetTickCount();

	m_info = "Bundle2PMVSִ���С���";
	m_info += "\r\n";
	flushEdit1();
	Bundle2PMVS_exe();
	m_info = "Bundle2PMVSִ�����!";
	m_info += "\r\n";
	flushEdit1();


	m_info = "RadialUndistortִ���С���";
	m_info += "\r\n";
	flushEdit1();
	RadialUndistort_exe();
	m_info = "RadialUndistortִ�����!";
	m_info += "\r\n";
	flushEdit1();

	
	m_info = "Bundle2Visִ���С���";
	m_info += "\r\n";
	flushEdit1();
	Bundle2Vis_exe();
	m_info = "Bundle2Visִ�����!";
	m_info += "\r\n";
	flushEdit1();

	//��IMG�ļ����´���PMVS�ļ���
	createPMVSFolder();
	Sleep(1000);
	
	m_info = "cmvsִ���С���";
	m_info += "\r\n";
	flushEdit1();
	cmvs_exe();
	m_info = "cmvsִ�����!";
	m_info += "\r\n";
	flushEdit1();


	m_info = "genOptionִ���С���";
	m_info += "\r\n";
	flushEdit1();
			//�����Ի��򵯳����ò���
	Dlg_genOption dlg2;
	dlg2.DoModal();
	m_info = "  ����1:lever����Ϊ:";
	m_info += m_lever;
	m_info += "\r\n";
	m_info += "  ����2:csize����Ϊ:";
	m_info += m_csize;
	m_info += "\r\n";
	m_info += "  ����3:threshold����Ϊ:";
	m_info += m_threshold;
	m_info += "\r\n";
	m_info += "  ����4:cwize����Ϊ:";
	m_info += m_wsize;
	m_info += "\r\n";
	m_info += "  ����5:minNbImage����Ϊ:";
	m_info += m_minNbImage;
	m_info += "\r\n";
	m_info += "  ����6:nbCPU����Ϊ:";
	m_info += m_nbCPU;
	m_info += "\r\n";
	flushEdit1();

	genOption_exe();
	m_info = "genOptionִ�����!";
	m_info += "\r\n";
	flushEdit1();


	m_info = "pmvs2ִ���С���";
	m_info += "\r\n";
	flushEdit1();
	pmvs2_exe();
	m_info = "pmvs2ִ�����!";
	m_info += "\r\n";
	flushEdit1();

	CString plyPath = picPath2 + "pmvs\\models\\option-0000.ply";
	char *ply = (LPSTR)(LPCSTR)plyPath;
	plyLoader.LoadModel(ply);
	int num = plyLoader.AllPointNums();
	CString strnump;
	strnump.Format("%d ��", num);


	long t2 = GetTickCount();//����ν�����ȡ��ϵͳ����ʱ��(s)  
	CString str;
	str.Format("%.2f s", (float(t2 - t1))/1000);//ǰ��֮���������ʱ��   



	m_info = "���ܵ������ɽ�����";
	m_info += "\r\n";
	m_info += "�������ؽ�����ά�㣺";
	m_info += strnump;
	m_info += "\r\n";
	m_info += "��ʱ��  ";
	m_info += str;
	m_info += "\r\n";	
	m_info += "\r\n";
	flushEdit1();
}


//Meshlab���ƴ����������Ҫ�ĵ��ƣ������в����ؽ����Լ���Ƭ���ز�����Ϊ��������׼������󱣴�Ϊmesh.ply
//Ϊ�˱��洦���ĵ��ƣ������ڴ�����󱣴�Ϊoption-fin.ply
//Meshlab����ľ��岽�����£�
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
10. Untick 'Binary encoding', click OK ,ע�������ǲ�ҪѡBinary encoding
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
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	//Meshlab��������

	//ʵʱˢ����ʾ
	m_info = "************ Step 6 ************";
	m_info += "\r\n";
	m_info += "��������";
	m_info += "\r\n";
	//�������һ����info
	CString nul = "";
	this->GetDlgItem(IDC_EDIT1)->SendMessage(EM_REPLACESEL, NULL, (LPARAM)(LPTSTR)(LPCTSTR)nul);
	flushEdit1();
	long t1 = GetTickCount();

	m_info = "MeshLab�ֶ���������Ͳ����ؽ��С���";
	m_info += "\r\n";
	flushEdit1();
	pointClear_exe();
	m_info = "MeshLab�ֶ��������!";
	m_info += "\r\n";
	flushEdit1();

	long t2 = GetTickCount();//����ν�����ȡ��ϵͳ����ʱ��(s)  
	CString str;
	str.Format("%.2f s", (float(t2 - t1))/1000);//ǰ��֮���������ʱ��   
	/*m_info = "��ʱ��  ";
	m_info += str;
	m_info += "\r\n";*/
	m_info = "\r\n";
	flushEdit1();	
}


//�������������һ�����ɵ�mesh.ply,���������������������
/*
-pmvs / models / output.mesh
- pmvs / models / output.point
- pmvs / models / output.obj <--Wavefront OBJ
- pmvs / models / output.mtl <--Wavefront OBJ
���У�output.mesh����ͨ��ViewMesh.exe�ۿ���output.obj����ͨ��Meshlab�ۿ�
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
	//ʵʱˢ����ʾ
	m_info = "************ Step 7 ************";
	m_info += "\r\n";
	m_info += "����ӳ�䣺";
	m_info += "\r\n";
	//�������һ����info
	CString nul = "";
	this->GetDlgItem(IDC_EDIT1)->SendMessage(EM_REPLACESEL, NULL, (LPARAM)(LPTSTR)(LPCTSTR)nul);
	flushEdit1();
	long t1 = GetTickCount();

	m_info = "TextureMeshִ���С���";
	m_info += "\r\n";
	flushEdit1();
	textureMesh_exe();
	m_info = "TextureMeshִ�����!";
	m_info += "\r\n";
	flushEdit1();

	long t2 = GetTickCount();//����ν�����ȡ��ϵͳ����ʱ��(s)  
	CString str;
	str.Format("%.2f s", (float(t2 - t1))/1000);//ǰ��֮���������ʱ��   
	m_info = "��ʱ��  ";
	m_info += str;
	m_info += "\r\n";
	m_info += "\r\n";
	flushEdit1();
}


//��ά��ʾ
void CMy3DRecWindowDlg::OnBnClickedButton7()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	
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
		if (i == 0) str = "δ���������ά��";
		else if (i == 1) str = "����������ά��";
		else if (i == 2) str = "MeshLab��ʾ��ά����"; 
	    else if (i == 3) str = "ViewMesh��ʾ��ά����";
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

		//�Զ����models\\output.mesh �ļ����˹�����Ҫ��TextMesh���������

		
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
		//�ر�ShellInfo
		if (ShellInfo.hProcess != NULL)
		{
			TerminateProcess(ShellInfo.hProcess, 0);
			ShellInfo.hProcess = NULL;
		}	
	}
}






