
// FileEnumeratorDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "FileEnumerator.h"
#include "FileEnumeratorDlg.h"
#include "afxdialogex.h"
#include <fstream>
#include <locale> 

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


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


// CFileEnumeratorDlg 对话框



CFileEnumeratorDlg::CFileEnumeratorDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_FILEENUMERATOR_DIALOG, pParent)
	, m_strPath(_T(""))
	, m_bLabel(FALSE)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CFileEnumeratorDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_DIR, m_strPath);
	DDX_Check(pDX, IDC_CHECK1, m_bLabel);
}

BEGIN_MESSAGE_MAP(CFileEnumeratorDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDCANCEL, &CFileEnumeratorDlg::OnBnClickedCancel)
	ON_BN_CLICKED(IDC_BTN_BROWES, &CFileEnumeratorDlg::OnBnClickedBtnBrowes)
	ON_BN_CLICKED(IDC_BTN_GENERATE, &CFileEnumeratorDlg::OnBnClickedBtnGenerate)
	ON_BN_CLICKED(IDC_BTN_OPEN, &CFileEnumeratorDlg::OnBnClickedBtnOpen)
END_MESSAGE_MAP()


// CFileEnumeratorDlg 消息处理程序

BOOL CFileEnumeratorDlg::OnInitDialog()
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

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CFileEnumeratorDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CFileEnumeratorDlg::OnPaint()
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
HCURSOR CFileEnumeratorDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CFileEnumeratorDlg::OnBnClickedCancel()
{
	// TODO: 在此添加控件通知处理程序代码
	CDialogEx::OnCancel();
}


void CFileEnumeratorDlg::OnBnClickedBtnBrowes()
{
	char szPath[MAX_PATH];
	BROWSEINFO bi;
	bi.hwndOwner = m_hWnd;
	bi.pidlRoot = NULL;
	bi.pszDisplayName = (LPWSTR)szPath;
	bi.lpszTitle = _T("请选择目录：");
	bi.ulFlags = 0;
	bi.lpfn = NULL;
	bi.lParam = 0;
	bi.iImage = 0;

	LPITEMIDLIST lp = SHBrowseForFolder(&bi);

	if (lp && SHGetPathFromIDList(lp, (LPWSTR)szPath))
	{
		m_strPath.Format(_T("%s"), szPath);
	}
	UpdateData(FALSE); 
}


void CFileEnumeratorDlg::RecursiveFindFile(CString& strPath)
{
	CFileFind ff;
	BOOL bFind = ff.FindFile(strPath + _T("\\*.*"));
	while (bFind)
	{
		bFind = ff.FindNextFile();
		if (ff.IsDots() || ff.IsSystem() || ff.IsHidden())
			continue;

		if (ff.IsDirectory())
		{
			CString folder = ff.GetFilePath();
			if (folder == strPath)
			{
				continue;
			}
			else
			{
				folderIdx++;
				RecursiveFindFile(ff.GetFilePath());
			}			
		}
		else
		{
			CString strFileName = ff.GetFilePath();
			if (m_bLabel)
			{
				CString strLabel;
				strLabel.Format(_T(" %d"), folderIdx);
				strFileName = strFileName + strLabel;
			}
			filelist.push_back(strFileName);
		}
	}
}

void CFileEnumeratorDlg::OnBnClickedBtnGenerate()
{
	CString strInfo;
	UpdateData(TRUE);
	/*if (IsDirectoryExists(m_strPath))
	{
		filelist.clear();
		folderIdx = 0;
		RecursiveFindFile(m_strPath);
		CString strFileName = m_strPath + _T("\\filelist.txt");
		char* filename = (LPSTR)(LPCTSTR)strFileName.GetBuffer(100);
		std::fstream file(filename, std::fstream::out);
		if (file.is_open())
		{
			for (int i = 0; i < filelist.size(); i++)
			{
				file << filelist[i] << std::endl;
			}
		}
		file.close();
		strInfo.Format(_T("生成文件列表成功,共找到%d个文件！"), filelist.size());
	}*/
	if (IsDirectoryExists(m_strPath))
	{
		filelist.clear();
		folderIdx = -1;
		RecursiveFindFile(m_strPath);
		CString strFileName;
		GetDlgItemText(IDC_EDIT_FILENAME, strFileName);
		if (strFileName == _T(""))
		{
			strFileName = _T("filelist.txt");
		}
		else
		{
			if (strFileName.Right(4) != _T(".txt"))
			{
				strFileName = strFileName + _T(".txt");
			}
		}
		strFileName = m_strPath + _T("\\") +  strFileName;
		try
		{
			CStdioFile file;
			setlocale(LC_CTYPE, ("chs"));
			file.Open(strFileName, CFile::modeCreate | CFile::modeWrite | CFile::typeText);
			for (int i = 0; i < filelist.size(); i++)
			{
				file.WriteString(filelist[i]);
				file.WriteString(CString("\n"));
			}
			file.Close();
			strInfo.Format(_T("生成文件列表成功,共找到%d个文件！"), filelist.size());
		}
		catch (CFileException* e)
		{
			e->ReportError();
			e->Delete();
		}
	}
	else
	{
		strInfo = _T("无效目录！");
	}
	MessageBox(strInfo, _T("提示"));
}

BOOL CFileEnumeratorDlg::IsDirectoryExists(CString const& path)
{
	//判断是否存在
	if (!PathFileExists(path))
		return false;
	//判断是否为目录
	DWORD attributes = ::GetFileAttributes(path);
	attributes &= FILE_ATTRIBUTE_DIRECTORY;
	return attributes == FILE_ATTRIBUTE_DIRECTORY;
}

void CFileEnumeratorDlg::OnBnClickedBtnOpen()
{
	// TODO: 在此添加控件通知处理程序代码
	if (IsDirectoryExists(m_strPath))
	{
		ShellExecute(NULL, NULL, _T("explorer"), m_strPath, NULL, SW_SHOW);
	}
}
