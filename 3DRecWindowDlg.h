
#include "opencv2/core/core.hpp"  
#include "highgui.h"  
#include "opencv2/imgproc/imgproc.hpp"  
#include "opencv2/features2d/features2d.hpp"  
#include "opencv2/nonfree/nonfree.hpp"  


#include <iostream>
#include "CvvImage.h"
#include <vector>
#include "afxwin.h"

#include "PlyLoader.h"

class CMenuEx;



using namespace std;
using namespace cv;

#pragma once


// CMy3DRecWindowDlg 对话框
class CMy3DRecWindowDlg : public CDialogEx
{
// 构造
public:

	CMy3DRecWindowDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_MY3DRECWINDOW_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	CMenuEx *m_pMenuEx;

	HWND GetWindowHandleByPID(DWORD dwProcessID);
	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()

public:
	void Resize();
	POINT  old;

public:
	afx_msg void OnBnClickedButtonOpen();
	afx_msg void OnBnClickedButtonExtract();
	afx_msg void OnBnClickedButtonMatch();
	afx_msg void OnBnClickedButtonSfm();

	afx_msg void OnMenuItem(UINT uItemID);

	CFont m_editFont;
	CFont m_staticFont;
	CFont m_groupFont;
	CFont m_buttonFont;
	
	CString m_info;
	CEdit m_editctr;
	
	int imagenum ;
	int extractnum;
	int imageShowType;

	Mat afterMatchImage[100];
	Mat afterExtractImage[100];

	void DrawPicToHDC(Mat img, UINT ID);
	CString getPath();
	void get_file_names(CString dir_name, vector<CString> & names,CString &fileFormat);

public:
	//第一、二步用到的opencv的函数和变量	 
	vector<vector<KeyPoint>> key_points_for_all;
	vector<Mat> descriptor_for_all;
	vector<vector<Vec3b>> colors_for_all;
	vector<vector<DMatch>> matches_for_all;
	void drawmatch(int i);
	void extract_features(
		vector<CString>& image_names,
		vector<vector<KeyPoint>>& key_points_for_all,
		vector<Mat>& descriptor_for_all,
		vector<vector<Vec3b>>& colors_for_all
		);
	void match_features(Mat& query, Mat& train, vector<DMatch>& matches);
	void match_features(vector<Mat>& descriptor_for_all, vector<vector<DMatch>>& matches_for_all);


public:
	// 后几步用到的变量和函数

	CString mainPath;  //提前指定好
	CString picPath;   //提前指定好
	CString picPath2;   //提前指定好
	CString tmpFolderPath;
	CString outFolderPath;

	SHELLEXECUTEINFO ShellInfo;

	CStatic m_static555;
	CPLYLoader plyLoader;

	//CString cmdBinPath;
	vector<CString>  picnames;
	vector<CString>  picfullnames;

	//全局参数设置
	//bundlerMatcher
	CString  m_matchthreshold;
	CString  m_firstOctave;
	//genOption
	CString  m_lever;
	CString  m_csize;
	CString  m_threshold;
	CString  m_wsize;
	CString  m_minNbImage;
	CString  m_nbCPU;


	void resizeImage(Mat &img);
	void  path_to_txt(CString &dir, CString &filename, vector<CString> &filenames);
	CString readSparsePointNum(CString strSourceDir);

	int  appendPathName(string strFileName);
	void appendProcess(); //调用了appendPathName

	void moveFileToOtherPath(CString &oldpath, CString &newpath, CString &fengefu);
	void DeleteFolder(CString sPath);
	void DeleteFullFolder(CString sPath);
	void createPMVSFolder();

	void flushEdit1();

	void ShellRunFun(CString exedir, CString param);

	void extractFocal_exe();
	void bundlerMatcher_exe();
	void bundler_exe();
	
	void RadialUndistort_exe();
	void Bundle2PMVS_exe();
	void Bundle2Vis_exe();

	void cmvs_exe();
	void genOption_exe();
	void pmvs2_exe();

	void textureMesh_exe();
	void pointClear_exe();
	
	afx_msg void OnBnClickedCancel();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnClose();
	afx_msg void OnBnClickedButton5();
	afx_msg void OnBnClickedButton6();
	afx_msg void OnBnClickedButton7();
	
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedButtonNextimage();
	afx_msg void OnBnClickedButtonModlechange();
	afx_msg void OnBnClickedButtonPointclear();
	afx_msg void OnBnClickedButtonMatchimage();
};
