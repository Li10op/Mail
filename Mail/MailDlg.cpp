
// MailDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "Mail.h"
#include "MailDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// �Ի�������
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

// ʵ��
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedBtnSave();
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
	ON_BN_CLICKED(IDC_BTN_SAVE, &CAboutDlg::OnBnClickedBtnSave)
END_MESSAGE_MAP()


// CMailDlg �Ի���



CMailDlg::CMailDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CMailDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CMailDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_BUTTON1, m_btnReceive);
	DDX_Control(pDX, IDC_BUTTON2, m_btnSend);
}

BEGIN_MESSAGE_MAP(CMailDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON1, &CMailDlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &CMailDlg::OnBnClickedButton2)
END_MESSAGE_MAP()


// CMailDlg ��Ϣ�������

BOOL CMailDlg::OnInitDialog()
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

	// ���ô˶Ի����ͼ�ꡣ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO: �ڴ���Ӷ���ĳ�ʼ������

	m_btnSend.EnableWindow(FALSE);
	m_receiveDlg.Create(IDD_RECEIVEDLG, this);
	m_receiveDlg.SetWindowPos(NULL, 20, 48, 0, 0, SWP_NOSIZE | SWP_NOZORDER | SWP_NOACTIVATE);
	m_receiveDlg.InitReceiveDialog();
	m_receiveDlg.ShowWindow(SW_HIDE);
	m_sendDlg.Create(IDD_SENDDLG, this);
	m_sendDlg.SetWindowPos(NULL, 20, 48, 0, 0, SWP_NOSIZE | SWP_NOZORDER | SWP_NOACTIVATE);
	m_sendDlg.ShowWindow(SW_SHOW);
		m_receiveDlg.InitReceiveDialog();
			m_sendDlg.InitSendDialog();

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

void CMailDlg::OnSysCommand(UINT nID, LPARAM lParam)
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
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CMailDlg::OnPaint()
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
HCURSOR CMailDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CMailDlg::OnBnClickedButton1()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	m_btnReceive.EnableWindow(FALSE);
	m_btnSend.EnableWindow(TRUE);
	m_receiveDlg.InitReceiveDialog();
	m_sendDlg.InitSendDialog();
	m_receiveDlg.ShowWindow(SW_SHOW);
	m_sendDlg.ShowWindow(SW_HIDE);
}


void CMailDlg::OnBnClickedButton2()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	m_btnReceive.EnableWindow(TRUE);
	m_btnSend.EnableWindow(FALSE);
		m_receiveDlg.InitReceiveDialog();
	m_sendDlg.InitSendDialog();
	m_sendDlg.ShowWindow(SW_SHOW);
	m_receiveDlg.ShowWindow(SW_HIDE);
}


void CAboutDlg::OnBnClickedBtnSave()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
}
