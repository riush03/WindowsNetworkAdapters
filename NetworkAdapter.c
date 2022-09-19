/****
*   A C programm to list network adapters for you machine
*  Author : Denis Muriungi

**/
#ifdef _WIN32_WINNT
#define _WIN32_WINNT 0X0600

#endif // 

//windows header files => required for listing network adapters
#include<WinSock2.h>
#include<iphlpapi.h>
#include<WS2tcpip.h>>

//standard c header files
#include<stdio.h> //for the printf() function
#include<stdlib.h> //for memory alloaction

//libraries linked with the executables
#pragma comment(lib,"ws2_32.lib")
#pragma comment(lib,"iphlpapi.lib")

int main()
{
	WSADATA d;

	if (WSAStartup(MAKEWORD(2, 2), &d))
	{
		printf("Failed to intialize.\n");
		return -1;
	}

	DWORD asize = 20000; //to store the size of our adapters adress buffer
	PIP_ADAPTER_ADDRESSES adapters;

	do
	{
		adapters = (PIP_ADAPTER_ADDRESSES)malloc(asize); //allocate 20000bytes to adapters

		if (!adapters)
		{
			printf("Could not allocate %ld bytes for adapters.\n", asize);
			WSACleanup();
			return -1;
		}
		/******
		*  AF_UNSPEC -> tells windows that we need both the IPV4 AND IPv6adreeses
		*   AF_INET  => FOR IPv4
		*   AF_INET6 => for IPv6
		* 
		* GAA_FLAG_INCLUDE_PREFIX => request list of adresses
		******/

		int z = GetAdaptersAddresses(AF_UNSPEC, GAA_FLAG_INCLUDE_PREFIX, 0, adapters, &asize);

		if (z == ERROR_BUFFER_OVERFLOW)
		{
			printf("GetAapterAdresses want %ld bytes.\n", asize);
			free(adapters);
			WSACleanup();
			return -1;
		}
	} while (!adapters);

	PIP_ADAPTER_ADDRESSES adapter = adapters;

	while (adapter)
	{
		printf("\n Adapter name: %s \n", adapter->FriendlyName);

		PIP_ADAPTER_UNICAST_ADDRESS adress = adapter->FirstUnicastAddress;
		while (adress)
		{
			printf("\t%s",adress->Address.lpSockaddr->sa_family == AF_INET ? "IPv4" : "IPv6");
			char adap[100];

			getnameinfo(adress->Address.lpSockaddr,adress->Address.iSockaddrLength, adap, sizeof(adap), 0, 0,NI_NUMERICHOST );
			printf("\t%s", adap);

			adress = adress->Next;
			
		}
		adapter = adapter->Next;
	}
	free(adapters);
	WSACleanup();
	return 0;
}
