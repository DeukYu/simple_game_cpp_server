#pragma once
class NetAddress
{
public:
	NetAddress() = default;
	NetAddress(SOCKADDR_IN sockAddr);
	NetAddress(const string& ip, uint16 port);

public:
	SOCKADDR_IN* GetSockAddr() { return &_sockAddr; }
	string GetIpAddress() const;
	uint16 GetPort() const;

public:
	static IN_ADDR Ip2Address(const string& ip);

private:
	SOCKADDR_IN _sockAddr = {};
};