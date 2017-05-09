#include <iostream>     
#include <boost\asio.hpp>
#include <boost\thread\thread.hpp>
#include <string>

void initServer();
void initClient();
void initReceiver();

int clientNum = 1;
std::string serverIP = "0.0.0.0";

int main()
{
	boost::thread thrd2(&initServer);
	boost::thread thrd3(&initReceiver);
	Sleep(5000);
	boost::thread thrd1(&initClient);
	thrd1.join();
	thrd2.join();
	thrd3.join();

	//system("pause");
	return 0;
}

void initServer()
{
	namespace ip = boost::asio::ip;
	boost::asio::io_service io_service;

	ip::udp::socket socket(io_service, ip::udp::endpoint(ip::udp::v4(), 0));
	socket.set_option(boost::asio::socket_base::broadcast(true));
	ip::udp::endpoint broadcast_endpoint(ip::address_v4::broadcast(), 6001);

	char buf[20];
	int icount = 1;
	while (clientNum < 5)
	{
		sprintf(buf, "Message: %d", icount);
		++icount;
		socket.send_to(boost::asio::buffer(buf, strlen(buf) + 1), broadcast_endpoint);
		Sleep(2000);
	}

	socket.close();
}

void initClient()
{
	namespace ip = boost::asio::ip;
	boost::asio::io_service io_service;

	bool connected = false;

	ip::udp::socket socket(io_service, ip::udp::endpoint(ip::udp::v4(), 6001));
	ip::udp::endpoint sender_endpoint;

	char buf[50];
	while (1)
	{
		std::size_t bytes_transferred = socket.receive_from(boost::asio::buffer(buf), sender_endpoint);
		std::cout << "got " << bytes_transferred << " bytes." << std::endl << "The message: " << buf << std::endl;

		std::string teststr(buf);
		if (teststr.find("success") != std::string::npos)
		{
			connected = true;
			std::cout << "connected" << std::endl;
		}

		if (serverIP == "0.0.0.0")
		{
			serverIP = sender_endpoint.address().to_string();
			std::cout << serverIP << std::endl;

			char *message = "connect";
			ip::udp::socket sender(io_service, ip::udp::endpoint(ip::udp::v4(), 6000));
			ip::udp::endpoint server_point(sender_endpoint.address(), 6002);
			sender.send_to(boost::asio::buffer(message, strlen(message) + 1), server_point);

			sender.close();
		}
	}
	
	socket.close();
}

void initReceiver()
{
	namespace ip = boost::asio::ip;
	boost::asio::io_service io_service;
	ip::udp::socket socket(io_service, ip::udp::endpoint(ip::udp::v4(), 6002));
	ip::udp::endpoint sender_endpoint;

	char buf[20];
	char sendBuf[50];

	while (clientNum < 5)
	{
		socket.receive_from(boost::asio::buffer(buf), sender_endpoint);
		ip::udp::endpoint client_point(sender_endpoint.address(), 6001);
		++clientNum;

		sprintf(sendBuf, "successfully connected! Present player number: %d", clientNum);
		socket.send_to(boost::asio::buffer(sendBuf, strlen(sendBuf) + 1), client_point);
	}
	
	socket.close();
}