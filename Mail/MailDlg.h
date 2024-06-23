
// MailDlg.h : ͷ�ļ�
//

#pragma once
#include "afxwin.h"
#include "ReceiveDlg.h"
#include "SendDlg.h"


// CMailDlg �Ի���
class CMailDlg : public CDialogEx
{
// ����
public:
	CMailDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_MAIL_DIALOG };

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
	CButton m_btnReceive;
	CButton m_btnSend;
	ReceiveDlg m_receiveDlg;
	SendDlg m_sendDlg;
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedButton2();
};
