// Link.cpp: implementation of the CLink class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Link.h"
#include <atlstr.h>


#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CLink::CLink()
{
	m_hSocket = NULL;
	m_nPort = 6666;
}

CLink::~CLink()
{

}
//得到本机名和IP地址;
int CLink::GetHostIP()
{
	char szhostname[128];
    CString str;
//得到本机名和IP地址:
	if( gethostname(szhostname, 128) == 0 )
	{
		// get host adresses
		struct hostent * phost;
		int i;
 
		phost = gethostbyname(szhostname);
        m_szHostname = szhostname;

		i=0;
 		int j;
        int h_length=4;
 		for( j = 0; j<h_length; j++ )
 		{
			CString addr;
 
 			if( j > 0 )
 				str += ".";
 
 			addr.Format("%u", (unsigned int)((unsigned
		 	char*)phost->h_addr_list[i])[j]);
			str += addr;
		}  //end for~
	}
    m_szIpaddress = str;
	return 1;
}

//初始化静态变量;
CString CLink::m_szHostname = _T("");
CString CLink::m_szIpaddress = _T("");

//建立一个服务器端socket;
void CLink::SocketInit()
{
	//建立winsock联接
	CString str;

	WSADATA wsd;   //相关信息;

	if (WSAStartup(MAKEWORD(2,2), &wsd) != 0) 
	{
		AfxMessageBox("WSAStartup() failed !\n");
	}

}

//创建一个套接字;
void CLink::Create()
{	/* 创建套接字 */
	m_hSocket = socket(AF_INET, SOCK_STREAM, 0);

	if (m_hSocket == INVALID_SOCKET)
	{
		CString str;
		str.Format("m_hSocket 初始化失败: %d\n", WSAGetLastError() );
		AfxMessageBox(str);
	}
}
//关闭一个套接字;
void CLink::Close()
{
	if(m_hSocket != NULL)
	{
		if( closesocket(m_hSocket) == SOCKET_ERROR )
		{
			AfxMessageBox("关闭错误!");
		}
	}
}
//套接字绑定;
void CLink::Bind()
{
	struct sockaddr_in sa;
	/* 填充服务器地址结构 */
	memset(&sa, 0, sizeof(sa));
	sa.sin_family			= AF_INET;
	sa.sin_port				= htons(m_nPort);
	sa.sin_addr.S_un.S_addr	= htonl(INADDR_ANY);

	/* 绑定套接字到服务器地址结构 */
	if ( bind(m_hSocket, (const sockaddr *)&sa,sizeof(sa)) != 0 )
	{
		CString str;
		str.Format("Bind() failed: %d\n", WSAGetLastError() );
		AfxMessageBox(str);
	}
}
//侦听
void CLink::Listen()
{
	// 监听套接字 
	if( m_hSocket != NULL )
	{
		if( listen(m_hSocket,5) == SOCKET_ERROR )
		{
			CString str;
			str.Format("Listen() failed: %d\n", WSAGetLastError() );
			AfxMessageBox(str);
		}
	}
}

//接收客户;
void CLink::Accept(CLink* pServer_Link)
{
	struct sockaddr_in cliaddr;
	int len = sizeof(SOCKADDR);

	pServer_Link->m_hSocket = accept(m_hSocket, (struct sockaddr *)&cliaddr, &len);
	
	if ( pServer_Link->m_hSocket == INVALID_SOCKET )
	{
		CString str;
		str.Format("Accept() failed: %d\n", WSAGetLastError() );
		AfxMessageBox(str);
		closesocket(m_hSocket);
	}
}
//连接请求;
int CLink::Connect(CString addr)
{
	unsigned long Server_Addr; /* 转换后的服务器地址 */
	struct sockaddr_in sa;

	Server_Addr = inet_addr(addr);

	memset(&sa, 0, sizeof(sa));
	sa.sin_family			= AF_INET;
	sa.sin_port				= htons(m_nPort);
	sa.sin_addr.S_un.S_addr	= Server_Addr;
	//客户端无须绑定;
	if( connect(m_hSocket, (const sockaddr *)&sa,sizeof(sa)) != NULL )
	{
		CString str;
		str.Format("Connect() failed: %d\n", WSAGetLastError() );
		AfxMessageBox(str);
		return 0;
	}
	return 1;
}
//发送数据;
int CLink::Send(void* pbuffer,int nSize)
{
	int nBytesSent;
	if( (nBytesSent = send(m_hSocket,(const char*)pbuffer,nSize,0)) == SOCKET_ERROR )
	{
		AfxMessageBox("发送错误,有可能是其他玩家已经退出游戏!\n游戏结束!");
		Close();
		::ExitProcess(NULL);
		return SOCKET_ERROR;
	}
	return nBytesSent;
}
//接收数据;
int CLink::Recv(void* pbuffer,int nSize)
{
	int nBytesReceive;
	if( (nBytesReceive = recv(m_hSocket,(char*)pbuffer,nSize,0)) == SOCKET_ERROR )
	{
		AfxMessageBox("接收错误,有可能是其他玩家已经退出游戏!\n游戏结束!");
		Close();
		::ExitProcess(NULL);
		return SOCKET_ERROR;
	}
	return nBytesReceive;
}


