
// FileEnumeratorDlg.h : ͷ�ļ�
//

#pragma once
#include <vector>

// CFileEnumeratorDlg �Ի���
class CFileEnumeratorDlg : public CDialogEx
{
// ����
public:
	CFileEnumeratorDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_FILEENUMERATOR_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedCancel();
	afx_msg void OnBnClickedBtnBrowes();
	CString m_strPath;
	afx_msg void OnBnClickedBtnGenerate();
private:
	void RecursiveFindFile(CString& strPath);
	BOOL IsDirectoryExists(CString const& path);
private:
	std::vector<CString> filelist;
	int folderIdx;
public:
	BOOL m_bLabel;
	afx_msg void OnBnClickedBtnOpen();
};
