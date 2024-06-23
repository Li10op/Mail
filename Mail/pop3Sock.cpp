// pop3Sock.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "Mail.h"
#include "pop3Sock.h"
#include "ReceiveDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define MAX_BUFF 20000
// pop3Sock

pop3Sock::pop3Sock()
{
	m_pDlg = NULL;
	state=pop3FIRST;
	rubbish_number=0;
	error="Not connected to server\r\n";
}

pop3Sock::~pop3Sock()
{
		m_pDlg = NULL;
}


// pop3Sock ��Ա����


void pop3Sock::OnClose(int nErrorCode)
{
	// TODO: �ڴ����ר�ô����/����û���

	if(nErrorCode==0)  m_pDlg->Disp(S_CLOSE);
}


void pop3Sock::OnConnect(int nErrorCode)
{
	// TODO: �ڴ����ר�ô����/����û���
		if(nErrorCode==0)  m_pDlg->Disp(S_CONNECT);
}


void pop3Sock::OnReceive(int nErrorCode)
{
	// TODO: �ڴ����ר�ô����/����û���
	if (nErrorCode == 0)
	{
		char buff[MAX_BUFF];  // ���ջ�����
		int rec = Receive(buff, MAX_BUFF - 1);  // ���շ���������������, ����һ���ֽ����ڽ�β��NULL�ַ�

		if (rec > 0)
		{
			buff[rec] = '\0';  // ��β��ΪNULL
			lastMsg = buff;
			outputMsg = buff;
			AnalyzeMsg();  // �����յ������ݣ�����ͬ�Ĵ���
		}
		else if (rec == 0)
		{
			// ���ӹر�
			error = "�����ѹرա�\r\n";
			m_pDlg->Disp(S_CLOSE);  // ��ʾ��Ϣ
		}
		else
		{
			// ��������ʱ��������
			error = "�ڽ�������ʱ��������!\r\n";
			m_pDlg->Disp(S_CLOSE);  // ��ʾ��Ϣ
		}
	}
	else
	{
		// nErrorCode ��0����ʾ��������
		error = "�ڽ�������ʱ��������!\r\n";
		m_pDlg->Disp(S_CLOSE);  // ��ʾ��Ϣ
	}
}

//�����׽�����ĶԻ���ָ�����
void pop3Sock::SetParent(ReceiveDlg *pDlg)
{
	m_pDlg = pDlg;
}


//����ڵ���鿴�ʼ���ť������
CString pop3Sock::GetMsg_button(int i)  //0��ʼ
{
	CString ret;
    CString s;
	int where=msgs[i].text.Find("From:");  //0��ʼ 
	s.Format("From:λ�ã�%d",where);//����  �õ�λ��
	if (where!=-1)
	{
		ret+=ReadLn(where,msgs[i].text);//��src���У���index��ָ��λ�ÿ�ʼ��ȡ���ִ���ֱ����ĩ��"\r"Ϊֹ��
		ret+="\n"; //���ϻ���
		//д��ret ��󷵻صĶ���
	}
	where=msgs[i].text.Find("To:");
	s.Format("To:λ�ã�%d",where);
	if (where!=-1)
	{
		ret+=ReadLn(where,msgs[i].text);
		ret+="\n"; //���ϻ���
	}
		where=msgs[i].text.Find("Date:");
        s.Format("Date:λ�ã�%d",where);
	if (where!=-1)
	{
		ret+=ReadLn(where,msgs[i].text);	
		ret+="\n";
	}
	return ret;
}
//����ڵ�������䰴ť������
CString pop3Sock::GetRbMsg_button(int i)  //0��ʼ
{
	CString ret;
    CString s;
	int where=rbmsgs[i].text.Find("From:");  //0��ʼ 
	s.Format("From:λ�ã�%d",where);//����  �õ�λ��
	if (where!=-1)
	{
		ret+=ReadLn(where,rbmsgs[i].text);//��src���У���index��ָ��λ�ÿ�ʼ��ȡ���ִ���ֱ����ĩ��"\r"Ϊֹ��
		ret+="\n"; //���ϻ���
		//д��ret ��󷵻صĶ���
	}
	where=rbmsgs[i].text.Find("To:");
	s.Format("To:λ�ã�%d",where);
	if (where!=-1)
	{
		ret+=ReadLn(where,rbmsgs[i].text);
		ret+="\n"; //���ϻ���
	}
		where=rbmsgs[i].text.Find("Date:");
        s.Format("Date:λ�ã�%d",where);
	if (where!=-1)
	{
		ret+=ReadLn(where,rbmsgs[i].text);	
		ret+="\n";
	}
	return ret;
}


//��õ� i ���ż��ı�����Ϣ
CString pop3Sock::GetMsgStuff(int i)
{
	CString ret;
    CString s;
	int where=msgs[i].text.Find("From:");
    s.Format("From:λ�ã�%d\r\n",where);//����  �õ�λ��
	
	if (where!=-1)
	{
		ret+=ReadLn(where,msgs[i].text);//��src���У���index��ָ��λ�ÿ�ʼ��ȡ���ִ���ֱ����ĩ��"\r"Ϊֹ��
		ret+="\r\n";
		//д��ret ��󷵻صĶ���
	}

	where=msgs[i].text.Find("To:");

s.Format("To:λ�ã�%d\r\n",where);	
	if (where!=-1)
	{
		ret+=ReadLn(where,msgs[i].text);
		ret+="\r\n";
	}
	
	where=msgs[i].text.Find("Date:");

s.Format("Date:λ�ã�%d\r\n",where);
	
	if (where!=-1)
	{
		ret+=ReadLn(where,msgs[i].text);	
		ret+="\r\n";
	}
	ret+=GetMsgSubject(i);   //���ϱ��⺯��
	ret+="\r\n"; //���ϻ���
	return ret;


}

//��õ� i ���ż����ż���
CString pop3Sock::GetMsgBody(int i)
{
/*a = "123456789";
b =a.Left(4);  //ֵΪ:1234
b = a.Right(4);  //ֵΪ:6789*/
	CString ret;
	int where=msgs[i].text.Find("\r\n\r\n");  //�ҵ���ʼ�Ľ�������λ�� ��һ������  �����ż����ݵĿ�ͷ
	if(where!=-1) where+=7;     //����ҵ���+4  �ӵ�����text������
	else          where=0;      //û�о͵���0  ʲô��û��
	ret=msgs[i].text.Right(msgs[i].text.GetLength()-(where-2));      //text���ܳ��ȼ�ȥ�ż���ͷ��λ�ã����ǵõ�\r\n\r\n����ż����ݡ� ��Լ�ռ�
  //���where=0 ���ǵõ�����text  ��û��\r\n\r\n��Ҳʵ����where��=-1ʱҪȥ��\r\n\r\n�����(����û���ˣ��Ͳ���ȥ��)
	ret=ret.Left(ret.GetLength()-3);  //��ȥ2�����з���һ���س����� ��Լ�ռ�
	//where!=0��ʱ��
	return ret;
}
//��õ� i �������ż����ż���
CString pop3Sock::GetRbMsgBody(int i)
{
/*a = "123456789";
b =a.Left(4);  //ֵΪ:1234
b = a.Right(4);  //ֵΪ:6789*/
	CString ret;
	int where=rbmsgs[i].text.Find("\r\n\r\n");  //�ҵ���ʼ�Ľ�������λ�� ��һ������  �����ż����ݵĿ�ͷ
	if(where!=-1) where+=7;     //����ҵ���+  �ӵ�����text������
	else          where=0;      //û�о͵���0  ʲô��û��
	ret=rbmsgs[i].text.Right(rbmsgs[i].text.GetLength()-(where-2));      //text���ܳ��ȼ�ȥ�ż���ͷ��λ�ã����ǵõ�\r\n\r\n����ż����ݡ� ��Լ�ռ�
  //���where=0 ���ǵõ�����text  ��û��\r\n\r\n��Ҳʵ����where��=-1ʱҪȥ��\r\n\r\n�����(����û���ˣ��Ͳ���ȥ��)
	ret=ret.Left(ret.GetLength()-3);  //��ȥ2�����з���һ���س����� ��Լ�ռ�
	//where!=0��ʱ��
	return ret;
}


//��õ� i ���ż��ı���
CString pop3Sock::GetMsgSubject(int i)
{
//	CString s;
//	s.Format("%s",msgs[i].text);
//	int where=s.Find("Subject:");
	
	CString ret;
	int where=msgs[i].text.Find("Subject:");
	if (where!=-1) 	ret=ReadLn(where,msgs[i].text);
	else ret.Format("�������䣬������");
	return ret;
}
//��õ� i �������ż��ı���
CString pop3Sock::GetRbMsgSubject(int i)
{
//	CString s;
//	s.Format("%s",msgs[i].text);
//	int where=s.Find("Subject:");
	
	CString ret;
	int where=rbmsgs[i].text.Find("Subject:");
	if (where!=-1) 	ret=ReadLn(where,rbmsgs[i].text);
	else ret.Format("û���ҵ�");
	return ret;
}

//��õ� i ���������ŵ�����
CString pop3Sock::GetMsg(int i)
{
	return msgs[i].text;
}

//��src���У���index��ָ��λ�ÿ�ʼ��ȡ���ִ���ֱ����ĩ��"\r"Ϊֹ��
//dst��һ�����ĵ�ַ��ȡ������׷���ڸô���ԭ����֮��
CString pop3Sock::ReadLn(int index,CString src)
{
//	CString comp;
//	CString dst;
//	comp=src[index];
//	while(comp!="\r")
//	{
//		dst+=comp;
//		comp=src[++index];
//	}
//	return dst;

	CString ret;
	CString s;
	
	ret=src.Right(src.GetLength()-index);
	int where=ret.Find("\r");
//	s.Format("�����ڵ�λ�ã�%d",where);
//	m_pDlg->m_Info+=s;
//	m_pDlg->UpdateData(FALSE);
	if(where!=-1) where+=1;  //���ϻ��з�
	else          where=0;
//	s.Format("�����ڵ�λ�ã�%d, Length=%d ",where,ret.GetLength());
//	m_pDlg->m_Info+=s;
//	m_pDlg->UpdateData(FALSE);
	
	ret=ret.Left(where);
	return ret;

}

 
 //�˳�������
void pop3Sock::Close()
{
	CString str;
	str.Format("quit%c%c",13,10);
	Send((LPCSTR)str,str.GetLength());
	state=pop3FIRST;
	CAsyncSocket::Close();
	error="ִ��pop3Sock::Close()���\r\nNot connected to server\r\n";
	m_pDlg->Disp(S_CLOSE);
}

void pop3Sock::AnalyzeMsg()
{
	int  ix;
	CString temp1;
	CString temp2;

	CString s;
	strstream str;
	string check;
	str<<(LPCSTR)lastMsg; //���յ�����Ϣ�������ַ���������  
	str>>check;           //��ȡ���յ���Ϣ�ĵ�һ�δ���
	if(check=="-ERR")     //����д���
	{
		error="Received -ERR from server :"+lastMsg;
		Close(); //�Ͽ����ӣ�Ȼ��ر�
		return;
	}
	
	//���û�д�������ݲ�ͬ�ĻỰ�׶���Ӧ������
	switch(state) 
	{
	case pop3FIRST: //����Ѿ����ӳɹ������ʼ����ʱ��stateΪFIRST
		msgs.clear();
		//��ʾ���ӳɹ�����������ص���Ϣ
		m_pDlg->Disp(S_RECEIVE); 
		//���� user �û��� ����
		s.Format("user %s%c%c",m_pDlg->m_strUser,13,10);
		Send((LPCSTR)s,s.GetLength()); //�����û��ʺ�
		state=pop3USER;
		break;

	case pop3USER:
		//��ʾ����������user�������Ӧ
		m_pDlg->Disp(S_RECEIVE);
		//���� pass ���� �����������
		s.Format("pass %s%c%c",m_pDlg->m_strPass,13,10); 
		Send((LPCSTR)s,s.GetLength()); 
		state=pop3PASS;
		break;

	case pop3PASS:
		//��ʾ�������� pass �������Ӧ
		m_pDlg->Disp(S_RECEIVE);
		//���� stat ����������������������
		//�ʼ�����������ռ�ռ�Ĵ�С
		s.Format("stat%c%c",13,10);
		Send((LPCSTR)s,s.GetLength());
		state=pop3STAT; 
		break;

	case pop3STAT:
	{
		string s1;
		str.seekg(0);  //���ַ����ĵ�ǰָ����Ϊ0
		str>>s1>>numMsg>>sizeMsg; //��������ʹ�С
		flush(str);
		m_pDlg->Disp(S_GETNUM);
		m_pDlg->Disp(S_GETSIZE);
		if(numMsg>0) //������ʼ�������RETR����ʼ���Ϣ
		{
			rubbish_number=1;
			retrMsg=1;        //Ҫȡ��һ����
			MESSAGEPROP prop; //׼������ż��ĵط�
			prop.msgSize=0;   //��Ա�ó�ֵ
			prop.retrSize=0;
			prop.text="";
			msgs.push_back(prop); //ѹ��������
			s.Format("retr 1%c%c",13,10);
			Send((LPCSTR)s,s.GetLength()); 
			state=pop3RETR;
		}
		else //���û���ʼ�����Ͽ�
		{
			error="No new messages\r\n";
			Close();
		}
	}
	break;

	case pop3RETR:
	{
		//��ʾȡ�������ż��ı�

//s.Format("numMsg= %d  retrMsg =%d",numMsg,retrMsg);	
//m_pDlg->m_Info+=s;
		if(isrubbish(lastMsg)) {  //�Ǿ�ֱ�Ӳ�����  ��ִ�в���
				MESSAGEPROP prop;
				prop.msgSize=0;
				prop.retrSize=0;
				prop.text="";
				rbmsgs.push_back(prop);
			rbmsgs[rubbish_number-1].text+=lastMsg; 
		    rbmsgs[rubbish_number-1].retrSize+=lastMsg.GetLength();
							
	//	s.Format("�ż���%d: ,�ż���С%d, %c%c",
//		 rubbish_number,lastMsg.GetLength(),13,10);
	//	s.Format("�����ż�����ɾ�����%d,%s",rbmsgs[rubbish_number-1].retrSize,rbmsgs[rubbish_number-1].text);
m_pDlg->m_Info+=s;
m_pDlg->UpdateData(FALSE);		
m_pDlg->Disp(S_RECEIVE);
rubbish_number++;
			//s.Format("һ���ҵ�%d�������ʼ���\n",rubbish_number);
			//m_pDlg->m_Info+=s;
			//state = POP3FILTER;
		}
	  else {
/*s.Format("�ż���%d: ,�ż���С%d, %c%c",
		 retrMsg-1,lastMsg.GetLength(),13,10);
m_pDlg->m_Info+=s;*/
m_pDlg->UpdateData(FALSE);		
m_pDlg->Disp(S_RECEIVE);
		msgs[retrMsg-1].text+=lastMsg; 
		msgs[retrMsg-1].retrSize+=lastMsg.GetLength();
		}
		ix=lastMsg.Find("\r\n.\r\n");
		if (ix != -1)	//�ҵ�������ŵĽ�β  ����ֻ��ѭ������  ����������ż�����
		{	
			//����Ƿ��������ʼ�
			if(retrMsg<numMsg) //������У����������retr����
			{
				MESSAGEPROP prop;
				prop.msgSize=0;
				prop.retrSize=0;
				prop.text="";
				msgs.push_back(prop);
				retrMsg++;
				//��retr����ȡ��һ����
				s.Format("retr %d%c%c",retrMsg,13,10);
				Send((LPCSTR)s,s.GetLength());
			}  else	{
				//���ȫ���ż�������ϣ��ж��Ƿ�Ҫɾ��
				
				if(m_pDlg->m_bolDel && numMsg>0) 
				{
					state=pop3DELE;
					delMsg=1;
					s.Format("dele %d%c%c",delMsg,13,10);
					Send((LPCSTR)s,s.GetLength());
				}  else {                      //�����˳�
					state=pop3ENDRETR;
					m_pDlg->Disp(S_ENDRETR);
					error="�ż�������,��ɾ�ʼ����Ự����\r\n";
					s.Format("quit%c%c",13,10);
					Send((LPCSTR)s,s.GetLength());
					Close();

					for(int i=0;i<retrMsg;i++)
					{
						s.Format("%d:  %s",i,GetMsgSubject(i));
			//m_pDlg->m_Info+=s;
					
						m_pDlg->m_ctrList.AddString(s);
				//		m_pDlg->m_ctrList.AddString((LPCSTR)GetMsgSubject(i));
					}
			//m_pDlg->UpdateData(FALSE);
					m_pDlg->m_ctrList.SetCurSel(0);
				}
			}
		}  //DEBUG 
		//}
	}
	break;

	case pop3DELE:
	{
		//ɾ��ʣ���ʼ�
		if(delMsg<numMsg)
		{
			delMsg++;
			s.Format("dele %d%c%c",delMsg,13,10);
			Send((LPCSTR)s,s.GetLength());
		}
		else //����Ѿ�ɾ�����
		{
			m_pDlg->Disp(S_ENDRETR);
			state=pop3GOON;
			error="Deleted all messages\r\n";
			s.Format("quit%c%c",13,10);
			Send((LPCSTR)s,s.GetLength());
			Close();
		}
	}
	break;
	
	case pop3GOON:
	
	default:
		m_pDlg->Disp(S_RECEIVE);
		break;
	}
}
//RC4����
CString pop3Sock::RC4_decrypt(CString letter)     
{
     CString decrypt;
	 string str = CA2A(letter.GetString());
    unsigned char Tbox[256] = {0x00}; //ȫ����ʼ��Ϊ0
	unsigned char Sbox[256] = {0x00};//ȫ����ʼ��Ϊ0
	char *k="wjl";//ʹ����ͬ�Ľ�����Կwjl
	int len=strlen(k); //��ȡ��Կ����
    for(int i=0;i<256;i++) Sbox[i]=i; //��ʼ��S
	if(len>=256) for(int i=0;i<256;i++) Tbox[i]=k[i];   //��Կ��չ������
	if(len<256) for(int i=0;i<256;i++) Tbox[i]=k[i%len];
	int j=0; 
	unsigned char swap;
	int n;
	for(int i=0;i<256;i++)  //S��T�ĳ�ʼ�û�
	{
		n=j+(int)Sbox[i]+(int)Tbox[i]; 
		j=n%256;
		swap=Sbox[i];
		Sbox[i]=Tbox[i];
		Tbox[i]=swap;
	}
	//����
	  int i=0;
      j=0;
	  int t;
	  for (int q = 0; q != str.length(); ++q){
	  char  ch = str.at(q);
		    i=(i+1)%256;
		   j=(j+Sbox[i])%256;
		   swap=Sbox[i];
		   Sbox[i]=Sbox[j];
		   Sbox[j]=swap;
		   int t1=(int)Sbox[i]+(int)Sbox[j];
		   t=t1%256;
		   char k1=Sbox[t];    //����ܲ���ͬ������Կ
		   char cipherchar = ch^k1; //���������һ�α�õ����� 
		   str.insert(q,1,cipherchar);
		   str.erase(q + 1, 1);
	  }
	  decrypt += CA2A(str.c_str());
	  return decrypt;

}
//�ж��Ƿ��������ʼ�
bool  pop3Sock::isrubbish(CString lastMsg)
{
	int i,j;
	CString ret = "";
	CStringArray BlackList;
	CString str[][1] = {{_T("��ע")},{_T("��Ʊ")},{_T("���ϲ�")},{_T("���")},{_T("AD")},{_T("Free")}};
	for(i=0;i<sizeof(str)/sizeof(str[0]);i++){
		for(j=0;j<sizeof(str[0])/sizeof(str[0][0]);j++){
			BlackList.Add(str[i][j]);	}      //��ӹؼ���
	}

	int where=lastMsg.Find("From:");   //�����ҵ�ƥ�����ݵ�����ֵ
	if (where!=-1)
	{   
		where+=4;                            //����ǰ��4
	    ret=lastMsg.Right(lastMsg.GetLength()-where);   //�Ӻ���ǰ��ȡ����
	    ret=ret.Left(ret.GetLength()-3);              //��ǰ�����ȡ����
		for(int i = 0; i < BlackList.GetCount(); i++)
		{
			if(ret.Find(BlackList[i]) != -1)     //���ƥ�䵽�ؼ���
			{
				return true;            //����TRUEֵ���ʼ����ж�Ϊ�����ʼ�
			}
		}
	}
	return false;
}
