/*

   Copyright [2010] [Josko Nikolic]

   Licensed under the Apache License, Version 2.0 (the "License");
   you may not use this file except in compliance with the License.
   You may obtain a copy of the License at

       http://www.apache.org/licenses/LICENSE-2.0

   Unless required by applicable law or agreed to in writing, software
   distributed under the License is distributed on an "AS IS" BASIS,
   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
   See the License for the specific language governing permissions and
   limitations under the License.

   CODE PORTED FROM THE ORIGINAL GHOST PROJECT: http://ghost.pwner.org/

*/

#ifndef SOCKET_H
#define SOCKET_H

#ifdef WIN32
 #include <winsock2.h>
 #include <errno.h>

 #define EADDRINUSE WSAEADDRINUSE
 #define EADDRNOTAVAIL WSAEADDRNOTAVAIL
 #define EAFNOSUPPORT WSAEAFNOSUPPORT
 #define EALREADY WSAEALREADY
 #define ECONNABORTED WSAECONNABORTED
 #define ECONNREFUSED WSAECONNREFUSED
 #define ECONNRESET WSAECONNRESET
 #define EDESTADDRREQ WSAEDESTADDRREQ
 #define EDQUOT WSAEDQUOT
 #define EHOSTDOWN WSAEHOSTDOWN
 #define EHOSTUNREACH WSAEHOSTUNREACH
 #define EINPROGRESS WSAEINPROGRESS
 #define EISCONN WSAEISCONN
 #define ELOOP WSAELOOP
 #define EMSGSIZE WSAEMSGSIZE
 // #define ENAMETOOLONG WSAENAMETOOLONG
 #define ENETDOWN WSAENETDOWN
 #define ENETRESET WSAENETRESET
 #define ENETUNREACH WSAENETUNREACH
 #define ENOBUFS WSAENOBUFS
 #define ENOPROTOOPT WSAENOPROTOOPT
 #define ENOTCONN WSAENOTCONN
 // #define ENOTEMPTY WSAENOTEMPTY
 #define ENOTSOCK WSAENOTSOCK
 #define EOPNOTSUPP WSAEOPNOTSUPP
 #define EPFNOSUPPORT WSAEPFNOSUPPORT
 #define EPROTONOSUPPORT WSAEPROTONOSUPPORT
 #define EPROTOTYPE WSAEPROTOTYPE
 #define EREMOTE WSAEREMOTE
 #define ESHUTDOWN WSAESHUTDOWN
 #define ESOCKTNOSUPPORT WSAESOCKTNOSUPPORT
 #define ESTALE WSAESTALE
 #define ETIMEDOUT WSAETIMEDOUT
 #define ETOOMANYREFS WSAETOOMANYREFS
 #define EUSERS WSAEUSERS
 #define EWOULDBLOCK WSAEWOULDBLOCK
#else
 #include <arpa/inet.h>
 #include <errno.h>
 #include <fcntl.h>
 #include <netdb.h>
 #include <netinet/in.h>
 #include <netinet/tcp.h>
 #include <sys/ioctl.h>
 #include <sys/socket.h>
 #include <sys/types.h>
 #include <unistd.h>

 typedef int SOCKET;

 #define INVALID_SOCKET -1
 #define SOCKET_ERROR -1

 #define closesocket close

 extern int GetLastError( );
#endif

#ifndef INADDR_NONE
 #define INADDR_NONE -1
#endif

#ifndef MSG_NOSIGNAL
 #define MSG_NOSIGNAL 0
#endif

#ifdef WIN32
 #define SHUT_RDWR 2
#endif

//
// CSocket
//

class CSocket
{
protected:
	SOCKET m_Socket;
	struct sockaddr_in m_SIN;
	bool m_HasError;
	int m_Error;

public:
	CSocket( );
	CSocket( SOCKET nSocket, struct sockaddr_in nSIN );
	~CSocket( );

	BYTEARRAY GetPort( );
	BYTEARRAY GetIP( );
	string GetIPString( );
	bool HasError( )						{ return m_HasError; }
	int GetError( )							{ return m_Error; }
	string GetErrorString( );
	void SetFD( fd_set *fd, fd_set *send_fd, int *nfds );
	void Allocate( int type );
	void Reset( );
};

//
// CTCPSocket
//

class CTCPSocket // : public CSocket
{
protected:
        SOCKET m_Socket;
	struct sockaddr_in m_SIN;
	bool m_HasError;
	int m_Error;
	bool m_Connected;

private:
	string m_RecvBuffer;
	string m_SendBuffer;
	uint32_t m_LastRecv;
	uint32_t m_LastSend;

public:
	CTCPSocket( );
	CTCPSocket( SOCKET nSocket, struct sockaddr_in nSIN );
	~CTCPSocket( );

        BYTEARRAY GetPort( );
	BYTEARRAY GetIP( );
	string GetIPString( );
	bool HasError( )                                                { return m_HasError; }
	int GetError( )                                                 { return m_Error; }
	string GetErrorString( );
	void SetFD( fd_set *fd, fd_set *send_fd, int *nfds );
	void Allocate( int type );
	void Reset( );
	bool GetConnected( )                                        { return m_Connected; }
	string *GetBytes( )                                         { return &m_RecvBuffer; }
	void PutBytes( const string &bytes );
	void PutBytes( const BYTEARRAY &bytes );
	void ClearRecvBuffer( )                                     { m_RecvBuffer.clear( ); }
        void SubstrRecvBuffer( unsigned int i )                     { m_RecvBuffer = m_RecvBuffer.substr( i ); }
	void ClearSendBuffer( )                                     { m_SendBuffer.clear( ); }
	uint32_t GetLastRecv( )                                     { return m_LastRecv; }
	uint32_t GetLastSend( )                                     { return m_LastSend; }
	void DoRecv( fd_set *fd );
	void DoSend( fd_set *send_fd );
	void Disconnect( );
	void SetNoDelay( );
};

//
// CTCPClient
//

class CTCPClient // : public CTCPSocket
{
protected:
        SOCKET m_Socket;
	struct sockaddr_in m_SIN;
	bool m_HasError;
	int m_Error;
        bool m_Connected;
	bool m_Connecting;

private:
	string m_RecvBuffer;
	string m_SendBuffer;
	uint32_t m_LastRecv;
	uint32_t m_LastSend;

public:
	CTCPClient( );
	~CTCPClient( );

        BYTEARRAY GetPort( );
	BYTEARRAY GetIP( );
	string GetIPString( );
	bool HasError( )                                                                    { return m_HasError; }
	int GetError( )                                                                     { return m_Error; }
	string GetErrorString( );
	void SetFD( fd_set *fd, fd_set *send_fd, int *nfds );
	void Allocate( int type );
	void Reset( );
	bool GetConnected( )                                                                { return m_Connected; }
	string *GetBytes( )                                                                 { return &m_RecvBuffer; }
	void PutBytes( const string &bytes );
	void PutBytes( const BYTEARRAY &bytes );
	void ClearRecvBuffer( )                                                             { m_RecvBuffer.clear( ); }
        void SubstrRecvBuffer( unsigned int i )                                             { m_RecvBuffer = m_RecvBuffer.substr( i ); }
	void ClearSendBuffer( )                                                             { m_SendBuffer.clear( ); }
	uint32_t GetLastRecv( )                                                             { return m_LastRecv; }
	uint32_t GetLastSend( )                                                             { return m_LastSend; }
        void FlushRecv( fd_set *fd );
	void DoRecv( fd_set *fd );
	void DoSend( fd_set *send_fd );
	void SetNoDelay( );
        void SetKeepAlive( );
	void Disconnect( );
	bool GetConnecting( )                                                               { return m_Connecting; }
	void Connect( const string &localaddress, const string &address, uint16_t port );
	bool CheckConnect( );
};

//
// CTCPServer
//

class CTCPServer // : public CTCPSocket
{
protected:
        SOCKET m_Socket;
	struct sockaddr_in m_SIN;
	bool m_HasError;
	int m_Error;
        bool m_Connected;

private:
	string m_RecvBuffer;
	string m_SendBuffer;
	uint32_t m_LastRecv;
	uint32_t m_LastSend;

public:
	CTCPServer( );
	~CTCPServer( );

        BYTEARRAY GetPort( );
	BYTEARRAY GetIP( );
	string GetIPString( );
	bool HasError( )					{ return m_HasError; }
	int GetError( )						{ return m_Error; }
	string GetErrorString( );
	void SetFD( fd_set *fd, fd_set *send_fd, int *nfds );
	void Allocate( int type );
	void Reset( );
	bool GetConnected( )                                { return m_Connected; }
	string *GetBytes( )                                 { return &m_RecvBuffer; }
	void PutBytes( const string &bytes );
	void PutBytes( const BYTEARRAY &bytes );
	void ClearRecvBuffer( )                             { m_RecvBuffer.clear( ); }
        void SubstrRecvBuffer( unsigned int i )             { m_RecvBuffer = m_RecvBuffer.substr( i ); }
	void ClearSendBuffer( )                             { m_SendBuffer.clear( ); }
	uint32_t GetLastRecv( )                             { return m_LastRecv; }
	uint32_t GetLastSend( )                             { return m_LastSend; }
	void DoRecv( fd_set *fd );
	void DoSend( fd_set *send_fd );
	void Disconnect( );
	void SetNoDelay( );
	bool Listen( const string &address, uint16_t port );
	CTCPSocket *Accept( fd_set *fd );
};

//
// CUDPSocket
//

class CUDPSocket // : public CSocket
{
protected:
        SOCKET m_Socket;
	struct sockaddr_in m_SIN;
	bool m_HasError;
	int m_Error;
	struct in_addr m_BroadcastTarget;
        
public:
	CUDPSocket( );
	~CUDPSocket( );

        BYTEARRAY GetPort( );
	BYTEARRAY GetIP( );
	string GetIPString( );
	bool HasError( )						{ return m_HasError; }
	int GetError( )							{ return m_Error; }
	string GetErrorString( );
	void SetFD( fd_set *fd, fd_set *send_fd, int *nfds );
	void Allocate( int type );
	void Reset( );
	bool SendTo( struct sockaddr_in sin, const BYTEARRAY &message );
	bool SendTo( const string &address, uint16_t port, const BYTEARRAY &message );
	bool Broadcast( uint16_t port, const BYTEARRAY &message );
	void SetBroadcastTarget( const string &subnet );
	void SetDontRoute( bool dontRoute );
};

#endif
