#include "pch.h"
#include "NetAddress.h"

NetAddress::NetAddress(SOCKADDR_IN sockAddr) : mSockAddr(sockAddr)
{
}

NetAddress::NetAddress(const string& ip, uint16 port)
{
	memset(&mSockAddr, 0, sizeof(mSockAddr));

	mSockAddr.sin_family = AF_INET;
	mSockAddr.sin_addr = Ip2Address(ip);
	mSockAddr.sin_port = htons(port);
}

string NetAddress::GetIpAddress() const
{
	char ipAddr[INET_ADDRSTRLEN];
	inet_ntop(AF_INET, &mSockAddr.sin_addr, ipAddr, sizeof(ipAddr));
	return string(ipAddr);
}

uint16 NetAddress::GetPort() const
{
	return ntohs(mSockAddr.sin_port);
}

IN_ADDR NetAddress::Ip2Address(const string& ip)
{
	IN_ADDR address;
	if (inet_pton(AF_INET, ip.c_str(), &address) != 1)
	{
		throw runtime_error("Invalid IP address format: " + ip);
	}
	return address;
}