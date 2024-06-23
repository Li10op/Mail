#pragma once
#include "afxwin.h"
#include "afxcmn.h"
#include "pop3Sock.h"

// ReceiveDlg �Ի���
class ReceiveDlg : public CDialog
{
	DECLARE_DYNAMIC(ReceiveDlg)

public:
	void Disp(LONG flag);  //�ڲ�ͬ�ĻỰ�׶���ʾ��ͬ����Ϣ
	void InitReceiveDialog(void);//��ʼ������
	ReceiveDlg(CWnd* pParent = NULL);   // ��׼���캯��
	pop3Sock pop3Socket;     //�׽��������ʵ��
	virtual ~ReceiveDlg();

// �Ի�������
	enum { IDD = IDD_RECEIVEDLG };

// ʵ��
protected:
	HICON m_hIcon;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButton4();
	CString m_strUser;
	CString m_strServer;
	CString m_strPass;
//	BOOL m_boolDel;
	CComboBox m_ctrList;
	CString m_Info;
	CRichEditCtrl m_ctrInfo;
	afx_msg void OnBtnConn();
	afx_msg void OnBtnDisc();
	afx_msg void OnBtnView();
	afx_msg void OnBtnSave();
	BOOL m_bolDel;
	bool isEncrypt(CString Msgbody);
	afx_msg void OnBtnRb();
};
