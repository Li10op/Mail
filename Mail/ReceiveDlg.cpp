// ReceiveDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "Mail.h"
#include "ReceiveDlg.h"
#include "afxdialogex.h"


// ReceiveDlg �Ի���

IMPLEMENT_DYNAMIC(ReceiveDlg, CDialog)

ReceiveDlg::ReceiveDlg(CWnd* pParent /*=NULL*/)
	: CDialog(ReceiveDlg::IDD, pParent)
	, m_strUser(_T(""))
	, m_strServer(_T(""))
	, m_strPass(_T(""))
	, m_bolDel(FALSE)
	, m_Info(_T(""))
{

}

ReceiveDlg::~ReceiveDlg()
{
}

void ReceiveDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_USER, m_strUser);
	DDX_Text(pDX, IDC_EDIT_SERVER, m_strServer);
	DDX_Text(pDX, IDC_EDIT_PASS, m_strPass);
	DDX_Check(pDX, IDC_CHECK_DEL, m_bolDel);
	DDX_Control(pDX, IDC_COMB_LIST, m_ctrList);
	DDX_Text(pDX, IDC_RICH_INFO, m_Info);
	DDX_Control(pDX, IDC_RICH_INFO, m_ctrInfo);
}


BEGIN_MESSAGE_MAP(ReceiveDlg, CDialog)
	//ON_BN_CLICKED(IDC_BUTTON4, &ReceiveDlg::OnBnClickedButton4)
	ON_BN_CLICKED(IDC_BTN_CONN, &ReceiveDlg::OnBtnConn)
//	ON_BN_CLICKED(IDC_BTN_DISC, &ReceiveDlg::OnBtNdisc)
ON_BN_CLICKED(IDC_BTN_DISC, &ReceiveDlg::OnBtnDisc)
ON_BN_CLICKED(IDC_BTN_VIEW, &ReceiveDlg::OnBtnView)
ON_BN_CLICKED(IDC_BTN_SAVE, &ReceiveDlg::OnBtnSave)
ON_BN_CLICKED(IDC_BTN_RB, &ReceiveDlg::OnBtnRb)
END_MESSAGE_MAP()


// ReceiveDlg ��Ϣ�������


//�������¼���������������Ա�����Ĵ���
//���û����"����"��ťʱ��ִ�д˺���
void ReceiveDlg::OnBtnConn()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
		//�趨pop3��ĶԻ���ָ�������ʹָ֮�򱾶Ի����Ա㴫����Ϣ
	pop3Socket.SetParent(this); 
	UpdateData(TRUE);           //ȡ���û��ڶԻ��������������
	InitReceiveDialog();
	pop3Socket.Create();        //�����׽��ֶ���ĵײ��׽���
	pop3Socket.Connect((LPCSTR)m_strServer,110); //����pop3������
	m_Info = "";                //���ı��б�����
	GetDlgItem(IDC_RICH_INFO)->SetWindowText(_T("")); 
	((CComboBox*)GetDlgItem(IDC_COMB_LIST))->ResetContent();
	for (int i = 0; i < pop3Socket.msgs.size(); i++)
	{
		CString subject = pop3Socket.GetMsgSubject(i);
		((CComboBox*)GetDlgItem(IDC_COMB_LIST))->AddString(subject);
	}
	GetDlgItem(IDC_BTN_VIEW)->EnableWindow(TRUE);   //�鿴�ʼ���ť
	GetDlgItem(IDC_BTN_SAVE)->EnableWindow(TRUE);   //�洢��ť
	GetDlgItem(IDC_BTN_DISC)->EnableWindow(TRUE);   //�Ͽ���ť
	GetDlgItem(IDC_BTN_RB)->EnableWindow(TRUE);   //�����䰴ť
	UpdateData(FALSE);          //�����û�����
}



//���û����"�Ͽ�"��ťʱ��ִ�д˺���
void ReceiveDlg::OnBtnDisc()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
		    pop3Socket.Close();
	     //�������Ӱ�ť
		GetDlgItem(IDC_BTN_CONN)->EnableWindow(TRUE);
		//���öϿ���ť
		GetDlgItem(IDC_BTN_DISC)->EnableWindow(TRUE); 
		//���ò鿴�ʼ���ť
		GetDlgItem(IDC_BTN_VIEW)->EnableWindow(TRUE);
			GetDlgItem(IDC_BTN_RB)->EnableWindow(TRUE);   //�����䰴ť
		((CComboBox*)GetDlgItem(IDC_COMB_LIST))->ResetContent();
		GetDlgItem(IDC_RICH_INFO)->SetWindowText(_T("")); 
	   UpdateData(FALSE);
}

//���û����"�鿴�ʼ�"��ťʱ��ִ�д˺���
void ReceiveDlg::OnBtnView()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
		int i;
	i = m_ctrList.GetCurSel();  //����û�����Ͽ��е�ѡ��
	//��ò���ʾָ���ż��Ļ������Լ��ż�����
      m_Info = "";
	 UpdateData(FALSE);
	CString rb=pop3Socket.GetMsgSubject(i);
	CString s;
	s.Format("Content:");
	m_Info += pop3Socket.GetMsg_button(i);
	m_Info +=pop3Socket.GetMsgSubject(i);   //���ϱ��⺯��
	m_Info +="\n"; //���ϻ���
	m_Info += s;
	if(isEncrypt(pop3Socket.GetMsgSubject(i))) {
	s=pop3Socket.RC4_decrypt(pop3Socket.GetMsgBody(i));
	//  int index_first;
	  //int index_last;
	
	  s=s.Left(s.GetLength()-2);
	m_Info +=s;
	}
	else {
		m_Info += pop3Socket.GetMsgBody(i);
	}

	/*s.Format("�� %d ���� %c%c",i,13,10);
	  m_Info +=s;
	  m_Info += pop3Socket.msgs[i].text;*/
	UpdateData(FALSE);
}

//����洢ʱ
void ReceiveDlg::OnBtnSave()
{
    int i;
    CString s;
    i = m_ctrList.GetCurSel();
    // Get the specified email's basic properties and content
    s = pop3Socket.GetMsgStuff(i);
    s += pop3Socket.GetMsgBody(i);

    // Open a file dialog to specify the save file path
    CFileDialog dlg(FALSE, _T("txt"), NULL, OFN_OVERWRITEPROMPT, _T("Text Files (*.txt)|*.txt||"), this);
    dlg.m_ofn.lpstrInitialDir = _T("C:\\Users\\<username>\\Desktop"); // Set the default path to the desktop
    if (dlg.DoModal() == IDOK)
    {
        try
        {
            CStdioFile file(dlg.GetPathName(), CFile::modeCreate | CFile::modeWrite | CFile::typeText);
            file.WriteString(s); // Use WriteString to write CString directly
            file.Close();
        }
        catch (CFileException* e)
        {
            e->ReportError();
            e->Delete();
        }
    }
}


//�ڲ�ͬ������£����û���ʾ��ͬ����Ϣ
void ReceiveDlg::Disp(LONG flag)
{
	CString s;
	switch(flag)
	{
	case S_CONNECT:
		//�����ӵ�����������ʾ��Ϣ
		s = "�����ӵ�"+m_strServer+"...\r\n";
		m_Info+=s;
		//�������Ӱ�ť
		GetDlgItem(IDC_BTN_CONN)->EnableWindow(FALSE);
		//���öϿ���ť
		GetDlgItem(IDC_BTN_DISC)->EnableWindow(TRUE); 
		//���ò鿴�ʼ���ť
		GetDlgItem(IDC_BTN_VIEW)->EnableWindow(FALSE);
		break;
    case S_RECEIVE: //�յ���������������Ӧ����ʾ��������
		//m_Info+=pop3Socket.lastMsg;
		  
		break; 
	case S_CLOSE: //��ʾ�ر����ӵ���Ϣ
		m_Info+=pop3Socket.error;
		s = "�����Ѿ��ر�\r\n";
		m_Info+=s;
		//�������Ӱ�ť
		GetDlgItem(IDC_BTN_CONN)->EnableWindow(TRUE);
		//���öϿ���ť
	  GetDlgItem(IDC_BTN_DISC)->EnableWindow(FALSE);
		break;
	case S_GETNUM: //�Ѿ�֪�����������ż�������
		s.Format("�����й��� %d ����\r\n",pop3Socket.numMsg);
		m_Info+=s;
		break;
	case S_GETSIZE: //��ʾ�ż��Ĵ�С
		s.Format("�������ż���СΪ��%d�ֽ�\r\n",pop3Socket.sizeMsg);
		m_Info+=s;
		break;
	case S_ENDRETR: //�����ż���������ϣ���ʾ��Ϣ
		s.Format("����ȡ�� %d ���ż�\r\n",pop3Socket.msgs.size()-pop3Socket.rubbish_number);
		s.Format("�������������ʼ� %d ��\r\n",pop3Socket.rubbish_number-1);
		//pop3Socket.rubbish_number=0; //���¹�0 �´����ӿ���
		m_Info+=s;
		//������ż����أ����ò鿴�ʼ���ť
		if((pop3Socket.msgs.size())>0)
			GetDlgItem(IDC_BTN_VIEW)->EnableWindow(TRUE);
		break;
	}
	UpdateData(FALSE);   //�����û�����
}
void ReceiveDlg::InitReceiveDialog(void)
{
	m_strServer = _T("pop.163.com");   //POP3��������ַ
	m_strUser = _T("yu15139751003@163.com");               //������û���
	m_strPass = _T("FBMEKJJVRVIZKSXA");              //����
	m_Info = _T("");                       //���ı������
	m_ctrList.ResetContent();
	if(pop3Socket.msgs.empty())
	{
		pop3Socket.msgs.clear();
	}
	GetDlgItem(IDC_BTN_RB)->EnableWindow(FALSE);   //���������䰴ť
	GetDlgItem(IDC_BTN_VIEW)->EnableWindow(FALSE);   //���ò鿴�ʼ���ť
	GetDlgItem(IDC_BTN_SAVE)->EnableWindow(FALSE);   //���ô洢��ť
	GetDlgItem(IDC_BTN_DISC)->EnableWindow(FALSE);   //���öϿ���ť
	UpdateData(FALSE);
}
bool ReceiveDlg::isEncrypt(CString Msgbody)
{
	int index;

	index = Msgbody.Find("����");
	if(index != -1) {
		return true;
	}
	else return false;

}
//����������
void ReceiveDlg::OnBtnRb()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	m_Info = "";                //���ı��б�����
	 UpdateData(FALSE);
	int i=0;
	CString num;
	num.Format("���������ʼ� %d �� %c%c",pop3Socket.rubbish_number-1,13,10);
	 m_Info +=num;
	for(i=0;i<pop3Socket.rubbish_number-1;i++)
	{
	CString rb;
	  rb.Format("%c%c�� %d ���� %c%c",13,10,i+1,13,10);
	  m_Info +=rb;
	CString s;
	m_Info += pop3Socket.GetRbMsg_button(i);
	m_Info +=pop3Socket.GetRbMsgSubject(i);   //���ϱ��⺯��
	m_Info +="\n"; //���ϻ���
    	s.Format("Content:");
	m_Info += s;
		m_Info += pop3Socket.GetRbMsgBody(i);
	}
	UpdateData(FALSE);
}

