#if !defined(AFX_pop3Sock_H__0444E4E0_0C28_11DA_BB87_AF2B44E1925E__INCLUDED_)
#define AFX_pop3Sock_H__0444E4E0_0C28_11DA_BB87_AF2B44E1925E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// pop3Sock.h : header file
//
#include <vector>
#include <strstream>
#include <string>

using namespace std;

class  ReceiveDlg;

/////////////////////////////////////////////////////////////////////////////
// pop3Sock command target

//��ʾ��ͬ��Ϣ�ı�־
#define S_CLOSE   1  //���ر�����ʱ����ʾ�ر���Ϣ
#define S_CONNECT 2  //�����ӳɹ�ʱ����ʾ������Ϣ
#define S_RECEIVE 3  //���յ���������������Ϣʱ����ʾ����Ϣ
#define S_GETNUM  4 //�������ż�������Ϻ���ʾ�����е��ż�����
#define S_GETSIZE 5 //�������ż�������Ϻ���ʾ�����е��ż��ܴ�С
#define S_ENDRETR 6 //�������ż�������Ϻ���ʾ�����Ϣ

//����pop3�Ự״̬��ö������
typedef enum {pop3FIRST=0,pop3USER,pop3PASS,pop3STAT,pop3LIST,pop3RETR,pop3ENDRETR,pop3DELE,pop3GOON} pop3STATE;

//��������һ���ż���Ϣ�Ľṹ
typedef struct 
{
	CString text;  //�洢�ż����ı�
	int msgSize;   //�ż��Ĵ�С
	int	retrSize;  //�ż�ʵ�����صĴ�С�������ع����ж�̬�仯
} MESSAGEPROP;

//�Լ����׽�����
class pop3Sock : public CAsyncSocket
{
// Attributes
public:
	CString lastMsg;
	CString outputMsg;
	CString error;
	int numMsg, sizeMsg, retrMsg;
	vector<MESSAGEPROP> msgs; //ʢ���ż���Ϣ������
	vector<MESSAGEPROP> rbmsgs; //ʢ�������ż���Ϣ������

// Operations
public:
	//��û������ʼ���ͷ�ֶ�: From, To, Date, Subject, Body
	CString GetMsgStuff(int i); 
	CString GetMsgBody(int i);     //����ż���
	CString GetMsgSubject(int i);  //����ż�����
	CString GetMsg(int i);         //��������ŵ�����
	CString GetMsg_button(int i);
	CString GetRbMsg_button(int i);//��������ʼ�
	CString GetRbMsgBody(int i);     //����ż���
	CString GetRbMsgSubject(int i);  //����ż�����
	CString ReadLn(int index,CString src);
	bool isrubbish(CString lastMsg);//�ж��Ƿ��������ʼ�
	int rubbish_number;

	void Close();            //�˳�������
	void SetParent(ReceiveDlg *  pDlg);
	pop3Sock();
	virtual ~pop3Sock();
   CString RC4_decrypt(CString letter); //�ż����ܵ���Ҫ����

// Overrides
public:
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(pop3Sock)
	public:
	virtual void OnConnect(int nErrorCode);
	virtual void OnClose(int nErrorCode);
	virtual void OnReceive(int nErrorCode);
	//}}AFX_VIRTUAL

	// Generated message map functions
	//{{AFX_MSG(pop3Sock)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG

// Implementation
protected:
private:
	void AnalyzeMsg();
	ReceiveDlg* m_pDlg;
	pop3STATE state;
	int delMsg;
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_pop3Sock_H__0444E4E0_0C28_11DA_BB87_AF2B44E1925E__INCLUDED_)
