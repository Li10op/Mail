// smtpSock.cpp : implementation file
//

#include "stdafx.h"
#include "Mail.h"
#include "smtpSock.h"
#include  "SendDlg.h"    //�Լ���ӵİ������
#include  "Base64.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define MAX_BUFF 20000
/////////////////////////////////////////////////////////////////////////////
// smtpSock

smtpSock::smtpSock()
{
	m_pDlg = NULL;
	state=smtpFIRST;
	//error="���Ӳ��Ϸ�����\r\n";
}

smtpSock::~smtpSock()
{
	m_pDlg = NULL;
}


// Do not edit the following lines, which are needed by ClassWizard.
#if 0
BEGIN_MESSAGE_MAP(smtpSock, CAsyncSocket)
	//{{AFX_MSG_MAP(smtpSock)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()
#endif	// 0

/////////////////////////////////////////////////////////////////////////////
// smtpSock member functions

//���׽����յ�FD_CONNECT��Ϣʱ��ִ�д˺��������������ѽ���
void smtpSock::OnConnect(int nErrorCode) 
{
	if(nErrorCode==0)  m_pDlg->Disp(S_CONNECT);
}

//���׽����յ�����������������ʱ��ִ�д˺���
void smtpSock::OnReceive(int nErrorCode) 
{
	if(nErrorCode==0) 
	{
		char buff[MAX_BUFF];             //���ջ�����
		int rec=Receive(buff,MAX_BUFF);  //���շ���������������
		buff[rec]=NULL;                  //��β��ΪNULL��
		lastMsg=buff;
		AnalyzeMsg();             //�����յ������ݣ�����ͬ�Ĵ���
	}  else  {
		error="�����ڼ䷢���˴���!\r\n";
		m_pDlg->Disp(S_CLOSE);   //��ʾ��Ϣ
	}
}

//���ر�����ʱ��ִ�д˺���
void smtpSock::OnClose(int nErrorCode) 
{
	if(nErrorCode==0)  m_pDlg->Disp(S_CLOSE);
}
//ʹ�׽�����ĶԻ���ָ�����ָ�����Ի���
void smtpSock::SetParent(SendDlg *pDlg)
{
	m_pDlg = pDlg;
}

//�˳�������
void smtpSock::Close()
{
	CString str;
	str.Format("quit%c%c",13,10);
	Send((LPCSTR)str,str.GetLength());
	m_pDlg->Disp(S_CLOSE);
	state=smtpFIRST;
	CAsyncSocket::Close();
	error="��������������Ѿ��Ͽ�\r\n";
}

//�������������������ݣ�������Ӧ�Ĵ���
void smtpSock::AnalyzeMsg()
{
	CString s;
	strstream str;
	string check;
	str<<(LPCSTR)lastMsg;
	str>>check;

	//����smtp�Ự������״̬������ͬ�Ĵ���	
	switch(state)
	{
	case smtpFIRST: //����Ѿ����ӳɹ������ʼ����ʱ��stateΪFIRST
		m_pDlg->Disp(S_RECEIVE); //������Ϣ������
		if(!(check=="220")) {}
		s.Format("EHLO %s%c%c",m_pDlg->m_Name,13,10);
		Send((LPCSTR)s,s.GetLength()); //����EHLO����
       //SMTP�ͻ�������HELO��������������ʺ򲢸�֪SMTP�������Լ������ַ����SMTP������ͨ�š�
		//�����������������ɹ��򷵻�250��Ӧ��
		state=smtpEHLO;
		break;

		//����������;���Base64����ת������û��������롣
	  //�����֤�ɹ�����������ͻ��˷���һ��OK�����ʾ���Կ�ʼ���Ĵ��䡣
	case smtpEHLO:
		m_pDlg->Disp(S_RECEIVE);
		if(!(check=="250")) {}
		s.Format("AUTH LOGIN%c%c",13,10); 
		Send((LPCSTR)s,s.GetLength()); //����AUTH LOGIN����
		state=smtpAUTH;
		break;

	case smtpAUTH:
		m_pDlg->Disp(S_RECEIVE);
		if(!(check=="334")) {}
		coder.Encode(m_pDlg->m_User);
		s.Format(_T("%s%c%c"), coder.EncodedMessage(),13,10);
		Send((LPCSTR)s,s.GetLength()); //�����û���
		state=smtpUSER; 
		break;

	case smtpUSER:
		m_pDlg->Disp(S_RECEIVE);
		if(!(check=="334")) {}
		
		coder.Encode(m_pDlg->m_Pass);
		s.Format(_T("%s%c%c"), coder.EncodedMessage(),13,10);
//	AfxMessageBox(s);
//		s.Format("NDkxMDE1%c%c",13,10); //491015
		Send((LPCSTR)s,s.GetLength()); //���Ϳ���
		state=smtpPASS;
		break;

	case smtpPASS:
		m_pDlg->Disp(S_RECEIVE);
		if(!(check=="235")) {}
		s.Format("MAIL FROM:<%s>%c%c",m_pDlg->m_Addr,13,10); 
//		s.Format("MAIL FROM:<mu_lm@sina.com>%c%c",13,10); 
		Send((LPCSTR)s,s.GetLength()); //����MAIL FROM����
		//SMTP�ͻ�������MAIL�����ʽ�����������֪�����˵ĵ����ʼ���ַ���ɹ��򷵻�250��Ӧ��
		state=smtpMAIL;
		break;
	case smtpMAIL:
		m_pDlg->Disp(S_RECEIVE);
		if(!(check=="250")) {}
//		s.Format("RCPT TO: <mu_lm@sina.com>%c%c",13,10); 
		s.Format("RCPT TO: <%s>%c%c",m_pDlg->m_Receiver,13,10); 
		Send((LPCSTR)s,s.GetLength()); //����RECP����
		//SMTP�ͻ����ٷ���RCPT�����ʽ����֪�����������˵ĵ������䣬�ɹ��򷵻�250��Ӧ��
		state=smtpRCPT;
		break;
	case smtpRCPT: 
		m_pDlg->Disp(S_RECEIVE);
		if(!(check=="250")) {}
		s.Format("DATA%c%c",13,10); 
		Send((LPCSTR)s,s.GetLength()); //����DATA����
		//SMTP�ͻ�������DATA�����ʽ����֪SMTP��������Ҫ��ʼ�����ʼ��ˣ����ɹ�����������Ӧ354��Ӧ�����ʼ�����
		state=smtpDATA;
		break;
	case smtpDATA: 
		//�ͻ������͵����ʼ������ˣ�������<CRLF>.<CRLF>����
		//���������ӵ��������󣬻��Ӧ250��Ӧ���ʶ���ͳɹ�
		m_pDlg->Disp(S_RECEIVE);
		if(!(check=="354")) {}

		m_pDlg->GetHeader();
		state=smtpQUIT;
		break;
	case smtpQUIT: 
		//����QUIT�����ʽ�����Ự��������һ����221��Ӧ��ȷ����ֹ�����ر�TCP����
		m_pDlg->Disp(S_RECEIVE);
		if(!(check=="250")) {}
		s.Format("QUIT%c%c",13,10); 
		Send((LPCSTR)s,s.GetLength()); //����QUIT����
		state=smtpDATA;
		break;
	default:
		m_pDlg->Disp(S_RECEIVE);
		break;
	}
}

