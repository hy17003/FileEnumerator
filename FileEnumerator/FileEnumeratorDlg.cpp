
// FileEnumeratorDlg.cpp : ʵ���ļ�
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


// CFileEnumeratorDlg �Ի���



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


// CFileEnumeratorDlg ��Ϣ�������

BOOL CFileEnumeratorDlg::OnInitDialog()
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

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
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

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ  ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CFileEnumeratorDlg::OnPaint()
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
HCURSOR CFileEnumeratorDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CFileEnumeratorDlg::OnBnClickedCancel()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CDialogEx::OnCancel();
}


void CFileEnumeratorDlg::OnBnClickedBtnBrowes()
{
	char szPath[MAX_PATH];
	BROWSEINFO bi;
	bi.hwndOwner = m_hWnd;
	bi.pidlRoot = NULL;
	bi.pszDisplayName = (LPWSTR)szPath;
	bi.lpszTitle = _T("��ѡ��Ŀ¼��");
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
		strInfo.Format(_T("�����ļ��б�ɹ�,���ҵ�%d���ļ���"), filelist.size());
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
			strInfo.Format(_T("�����ļ��б�ɹ�,���ҵ�%d���ļ���"), filelist.size());
		}
		catch (CFileException* e)
		{
			e->ReportError();
			e->Delete();
		}
	}
	else
	{
		strInfo = _T("��ЧĿ¼��");
	}
	MessageBox(strInfo, _T("��ʾ"));
}

BOOL CFileEnumeratorDlg::IsDirectoryExists(CString const& path)
{
	//�ж��Ƿ����
	if (!PathFileExists(path))
		return false;
	//�ж��Ƿ�ΪĿ¼
	DWORD attributes = ::GetFileAttributes(path);
	attributes &= FILE_ATTRIBUTE_DIRECTORY;
	return attributes == FILE_ATTRIBUTE_DIRECTORY;
}

void CFileEnumeratorDlg::OnBnClickedBtnOpen()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if (IsDirectoryExists(m_strPath))
	{
		ShellExecute(NULL, NULL, _T("explorer"), m_strPath, NULL, SW_SHOW);
	}
}
