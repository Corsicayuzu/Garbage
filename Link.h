// Link.h: interface for the CLink class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_LINK_H__287CDDA6_0405_4A4C_AF03_E29CB5624C55__INCLUDED_)
#define AFX_LINK_H__287CDDA6_0405_4A4C_AF03_E29CB5624C55__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class CLink  
{
private:


	char buff[256]; /* 存放服务端返回的信息 */

	unsigned long uladdr; /* 转换后的服务器地址 */

	int m_nPort;		/* 默认去连接的端口 */
	SOCKET m_hSocket;

	struct sockaddr_in cliaddr; /* 存放返回的客户地址信息 */
public:
	static CString	m_szHostname;	  //本机名;
	static CString  m_szIpaddress;  //本机IP地址;
	static int GetHostIP();
public:
	CLink();
	virtual ~CLink();

	void SocketInit();

	void Create();
	void Bind();
	void Close();

	void Listen();
	void Accept(CLink* pServer_Link);
	int Connect(CString);   //0表示失败,1表示成功;

	int Send(void* pbuffer,int nSize);
	int Recv(void* pbuffer,int nSize);
};

#endif // !defined(AFX_LINK_H__287CDDA6_0405_4A4C_AF03_E29CB5624C55__INCLUDED_)
