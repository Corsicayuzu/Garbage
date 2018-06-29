#include "Link.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlg_Start dialog


CDlg_Start::CDlg_Start(CWnd* pParent /*=NULL*/)
	: CDialog(CDlg_Start::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlg_Start)
	m_ServerIP = _T("");
	m_Client = _T("���ڴ���������IP��ַ");
	m_Name = _T("");
	m_Face_ID = 0;
	m_Net_States = _T("");
	m_Net_Info = _T("��ǰ��û�н�����������");
	//}}AFX_DATA_INIT
}


void CDlg_Start::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlg_Start)
	DDX_Text(pDX, EDIT_SERVER, m_ServerIP);
	DDX_Text(pDX, EDIT_CLIENT, m_Client);
	DDX_Text(pDX, EDIT_NAME, m_Name);
	DDV_MaxChars(pDX, m_Name, 6);
	DDX_Scroll(pDX, SCROLLBAR_FACE, m_Face_ID);
	DDX_Text(pDX, STATIC_NET_STATES, m_Net_Info);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlg_Start, CDialog)
	//{{AFX_MSG_MAP(CDlg_Start)
	ON_BN_CLICKED(IDB_EXIT, OnExit)
	ON_BN_CLICKED(RADIO_CLIENT, OnClient)
	ON_BN_CLICKED(RADIO_LINK, OnLink)
	ON_BN_CLICKED(RADIO_SINGLE, OnSingle)
	ON_BN_CLICKED(RADIO_SERVER, OnServer)
	ON_WM_HSCROLL()
	ON_BN_CLICKED(IDB_START, OnStart)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDB_GAME_MAKER, OnGameMaker)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlg_Start message handlers
//�������˽��պ���


DWORD WINAPI CDlg_Start::Server_Login(LPVOID pParam)
{
	CString str;
	char net_info[100];

	CLink listen;
	listen.SocketInit();
	listen.Create();
	listen.Bind();
	listen.Listen();

	s_m_szNetInfo.Format("%s�Ѿ�����������,�ȴ���һλ��Ҽ���...",pCenterPlayer->m_szName);
	
	//���տͻ�������;
	listen.Accept(pServerLink1);
	listen.Close();
	pServerLink1->Recv(pRightPlayer,sizeof(CPlayerInfo));	



	//����������Ϣ;
	sprintf(net_info,s_m_szNetInfo);
	pServerLink1->Send(net_info,sizeof(char)*100);
			
	
	s_m_szNetInfo.Format("%s�Ѿ�����������\n%s������Ϸ\n%s������Ϸ\n��ʼ��Ϸ!!",\
		pCenterPlayer->m_szName,pRightPlayer->m_szName);
	sprintf(net_info,s_m_szNetInfo);
	
    //�ٷ���һ��������Ϣ;
	pServerLink1->Send(net_info,sizeof(char)*100);

	
	//���ͻ������ͻ�����ͷ��ID��;
	pServerLink1->Send(pRightPlayer,sizeof(CPlayerInfo));
	pServerLink1->Send(pCenterPlayer,sizeof(CPlayerInfo));

	s_m_bOK = TRUE;
	return 1;
}

	
//�ͻ��˺���;
DWORD WINAPI CDlg_Start::Client_Login(LPVOID pParam)
{	
	char net_info[100];
	//���͵�½��Ϣ��S
	pClientLink->Send(pCenterPlayer,sizeof(CPlayerInfo));
	//����S������Ϣ;
	pClientLink->Recv(net_info,sizeof(char)*100);
	s_m_szNetInfo = net_info;
	::Sleep(300);
	//�ٽ���һ��������Ϣ;
	pClientLink->Recv(net_info,sizeof(char)*100);
	s_m_szNetInfo = net_info;

	//���������Ϣ;
	pClientLink->Recv(pRightPlayer,sizeof(CPlayerInfo));
	
	s_m_bOK = TRUE;

	return 1;
}

CString CDlg_Start::s_m_szNetInfo = _T("");
int CDlg_Start::s_m_bOK = FALSE;

void CDlg_Start::OnStart() 
{
	UpdateData(TRUE);
	if( m_Name == _T("") )
	{
		AfxMessageBox("�������������(�ǳ�)!");
		GetDlgItem(EDIT_NAME)->SetFocus();
		return ;
	}

	GetDlgItem(IDB_START)->EnableWindow(FALSE);
	//�������;
	sprintf(pCenterPlayer->m_szName,m_Name);

	if( ((CButton* )GetDlgItem(RADIO_SINGLE))->GetCheck() )
	{		
		pCenterPlayer->m_nPlayerType = 0;	//����0��ʾ������Ϸ;
		//�����ʼ��:
		sprintf(pRightPlayer->m_szName,"���1");
		
		CDialog::OnOK();
	}
	else
	{
		//������;
		if( ((CButton* )GetDlgItem(RADIO_SERVER))->GetCheck() )
		{
			pCenterPlayer->m_nPlayerType = 1;	//����1��ʾ�ͻ���;   		

			pServerLink1 = new CLink;  //���ӿͻ�1;
			

			m_hThread = ::CreateThread(NULL,0,Server_Login,0,0,&m_dwThreadId);
			SetTimer(1,200,NULL);  //���ڸ��µļ�ʱ��;
		}
		//�ͻ���;
		else
		{
			pCenterPlayer->m_nPlayerType = 2;   //����2��ʾ�ͻ���;

			pClientLink = new CLink;
			pClientLink->SocketInit();
			pClientLink->Create();
			if( !pClientLink->Connect(m_Client) )
			{
				pClientLink->Close();
				delete pClientLink;
				GetDlgItem(IDB_START)->EnableWindow(TRUE);
				return ;
			}
 
			m_hThread = ::CreateThread(NULL,0,Client_Login,0,0,&m_dwThreadId);
			SetTimer(1,200,NULL);  //���ڸ��µļ�ʱ��; 
		}
	}//end if~
}

void CDlg_Start::OnExit() 
{
	CDialog::OnCancel();
}

BOOL CDlg_Start::OnInitDialog() 
{
	CDialog::OnInitDialog();

	( (CButton* )GetDlgItem(RADIO_SINGLE) )->SetCheck(TRUE);
	( (CButton* )GetDlgItem(RADIO_LINK) )->SetCheck(FALSE);
//���ù�������Χ;
	CScrollBar* pSB = (CScrollBar*) GetDlgItem(SCROLLBAR_FACE);
	pSB->SetScrollRange(0, 3);

	SendDlgItemMessage(PIC_FACE,STM_SETIMAGE,IMAGE_BITMAP,LPARAM(m_cBitmap0.m_hObject) );

	CLink::GetHostIP();   //�õ�����IP��ַ;

	pCenterPlayer = new CPlayerInfo;
	pLeftPlayer = new CPlayerInfo;
	pRightPlayer = new CPlayerInfo;

	GetDlgItem(EDIT_NAME)->SetFocus();
	return FALSE;  	          
}
//������Ϸ;
void CDlg_Start::OnSingle() 
{
	( (CButton* )GetDlgItem(RADIO_SINGLE) )->SetCheck(TRUE);
	( (CButton* )GetDlgItem(RADIO_LINK) )->SetCheck(FALSE);

	GetDlgItem(RADIO_SERVER)->EnableWindow(FALSE);
	GetDlgItem(RADIO_CLIENT)->EnableWindow(FALSE);

	
}
//���˶�ս;
void CDlg_Start::OnLink() 
{
	( (CButton* )GetDlgItem(RADIO_SINGLE) )->SetCheck(FALSE);
	( (CButton* )GetDlgItem(RADIO_LINK) )->SetCheck(TRUE);

	GetDlgItem(RADIO_SERVER)->EnableWindow();
	GetDlgItem(RADIO_CLIENT)->EnableWindow();

	GetDlgItem(EDIT_SERVER)->EnableWindow();
	GetDlgItem(EDIT_CLIENT)->EnableWindow();
	
	( (CButton* )GetDlgItem(RADIO_SERVER) )->SetCheck(TRUE);
	( (CButton* )GetDlgItem(RADIO_CLIENT) )->SetCheck(FALSE);

	m_ServerIP = "����IP��ַ:";
	m_ServerIP += CLink::m_szIpaddress;

	UpdateData(FALSE);
}
//���ӻ�(�ͻ���);
void CDlg_Start::OnClient() 
{
	( (CButton* )GetDlgItem(RADIO_SERVER) )->SetCheck(FALSE);
	( (CButton* )GetDlgItem(RADIO_CLIENT) )->SetCheck(TRUE);
	GetDlgItem(EDIT_CLIENT)->SetFocus();	
}
//������������;
void CDlg_Start::OnServer() 
{
	( (CButton* )GetDlgItem(RADIO_SERVER) )->SetCheck(TRUE);
	( (CButton* )GetDlgItem(RADIO_CLIENT) )->SetCheck(FALSE);	
}

void CDlg_Start::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	int nTemp1;

	nTemp1 = pScrollBar->GetScrollPos();
	switch(nSBCode)
	{
	case SB_LINELEFT: // left arrow button
		if(nTemp1 != 0)
		{
			pScrollBar->SetScrollPos(nTemp1-1);
		}
		break;
	case SB_LINERIGHT: // right arrow button
		if(nTemp1 < 3)
		{
			pScrollBar->SetScrollPos(nTemp1+1);
		}
		break;
	}

	switch(nTemp1)
	{
	case 0:
		SendDlgItemMessage(PIC_FACE,STM_SETIMAGE,IMAGE_BITMAP,LPARAM(m_cBitmap0.m_hObject) );
		break;
	case 1:
		SendDlgItemMessage(PIC_FACE,STM_SETIMAGE,IMAGE_BITMAP,LPARAM(m_cBitmap1.m_hObject) );
		break;
	case 2:
		SendDlgItemMessage(PIC_FACE,STM_SETIMAGE,IMAGE_BITMAP,LPARAM(m_cBitmap2.m_hObject) );
		break;
	case 3:
		SendDlgItemMessage(PIC_FACE,STM_SETIMAGE,IMAGE_BITMAP,LPARAM(m_cBitmap3.m_hObject) );
		break;
	}
}


void CDlg_Start::OnTimer(UINT nIDEvent) 
{
	m_Net_Info = s_m_szNetInfo;
	UpdateData(FALSE);

	if( s_m_bOK )
	{
		KillTimer(1);
		CDialog::OnOK();
	}

	CDialog::OnTimer(nIDEvent);
}

void CDlg_Start::OnGameMaker() 
{
	CDlg_About	dlg;
	dlg.DoModal();
}
