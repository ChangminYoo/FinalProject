#include <iostream>
//#include <boost\array.hpp>
#include <boost\asio.hpp>
#include <boost\bind.hpp>

const char SERVERIP[] = "127.0.0.1";
const unsigned short PORT = 31400;
using namespace std;

class TCP_Client
{
public:
	TCP_Client(boost::asio::io_service& io_service)
		: m_io_service(io_service),
		m_Socket(io_service),
		m_nSeqNumber(0)
	{}

	void Connect(boost::asio::ip::tcp::endpoint& endpoint)
	{
		m_Socket.async_connect(endpoint, boost::bind(&TCP_Client::handle_connect,
			this, boost::asio::placeholders::error));
	}

private:
	boost::asio::io_service& m_io_service;
	boost::asio::ip::tcp::socket m_Socket;
	int m_nSeqNumber;
	array<char, 128> m_ReceiveBuffer;
	string m_WriteMessage;

	void PostWrite()
	{
		if (m_Socket.is_open() == false)
		{
			return;
		}
		

		if (m_nSeqNumber > 7)
		{
			m_Socket.close();
			return;
		}

		++m_nSeqNumber;

		char szMessage[128] = { 0, };
		char msg[] = "- Send Message";
		char* tmp = strcat(reinterpret_cast<char*>(m_nSeqNumber), msg);
		memcpy(szMessage, tmp, sizeof(tmp));

		m_WriteMessage = szMessage;

		boost::asio::async_write(m_Socket, boost::asio::buffer(m_WriteMessage),
			boost::bind(&TCP_Client::handle_write, this, boost::asio::placeholders::error,
				boost::asio::placeholders::bytes_transferred));

		PostReceive();
	}

	void PostReceive()
	{
		memset(&m_ReceiveBuffer, '\0', sizeof(m_ReceiveBuffer));

		m_Socket.async_read_some(boost::asio::buffer(m_ReceiveBuffer),
			boost::bind(&TCP_Client::handle_receive, this, boost::asio::placeholders::error,
				boost::asio::placeholders::bytes_transferred));
	}

	void handle_connect(const boost::system::error_code& error)
	{
		if (error)
		{
			cout << " connect failed: " << error.message() << endl;
		}
		else
		{
			cout << " connected " << endl;
			PostWrite();
		}
	}

	void handle_write(const boost::system::error_code& /*error*/, size_t /*bytes_transferred*/)
	{

	}

	void handle_receive(const boost::system::error_code& error, size_t bytes_transferred)
	{
		if (error)
		{
			if (error == boost::asio::error::eof)
			{
				cout << "서버와 연결이 끊어졌습니다. " << endl;
			}
			else
			{
				cout << "error No: " << error.value() << " error Message: "
					<< error.message() << endl;
			}
		}
		else
		{
			string strRecvMessage = m_ReceiveBuffer.data();
			cout << "서버에서 받은 메시지: " << strRecvMessage << ", 받은 크기: "
				<< bytes_transferred << endl;

			PostWrite();
		}
	}
};
int main()
{
	boost::asio::io_service io_service;

	boost::asio::ip::tcp::endpoint endpoint(boost::asio::ip::address::from_string(SERVERIP), PORT);

	TCP_Client client(io_service);
	client.Connect(endpoint);
	io_service.run();
	cout << "네트워크 접속 종료" << endl;
	getchar();
	
	/*
	boost::system::error_code connected_error;
	boost::asio::ip::tcp::socket socket(io_service);
	socket.connect(endpoint, connected_error);

	if (connected_error)
	{
		cout << "연결 실패. error No: " << connected_error.value()
			<< ", Message" << connected_error.message() << endl;
		getchar();
		return 0;
	}
	else
	{
		cout << "서버에 연결 성공" << endl;
	}

	for (auto i = 0; i < 7; ++i)
	{
		char szMessage[128] = { 0, };
		printf("%d - Send Message", i);
	
		int nMsgLen = strlen(szMessage);

		boost::system::error_code ignored_error;
		socket.write_some(boost::asio::buffer(szMessage, nMsgLen), ignored_error);

		cout << "서버에 보낸 메시지: " << szMessage << endl;

		array<char, 128> buf;
		buf.assign(0);
		boost::system::error_code error;
		size_t len = socket.read_some(boost::asio::buffer(buf), error);

		if (error)
		{
			if (error == boost::asio::error::eof)
			{
				cout << "서버와 연결이 끊어졌습니다." << endl;
			}
			else
			{
				cout << "error No: " << error.value() << "error Message: "
					<< error.message().c_str() << endl;
			}
			break;
		}
		cout << " 서버로부터 받은 메시지: " << &buf[0] << endl;
	}

	if (socket.is_open())
	{
		socket.close();
	}

	getchar();
	return 0;
	*/
}

/*
using boost::asio::ip::tcp;
using namespace std;

boost::asio::io_service g_io_service;

int main(int argc, char* argv[])
{
	try
	{
		if (argc != 2)
		{
			cerr << "Usage: client <host>" << endl;
			return 1;
		}

		tcp::resolver resolver(g_io_service);
		tcp::resolver::query query(argv[1], "daytime");

		tcp::resolver::iterator endpoint_iter = resolver.resolve(query);
		tcp::resolver::iterator end;

		tcp::socket socket(g_io_service);
		boost::system::error_code error = boost::asio::error::host_not_found;
		while (error && endpoint_iter != end)
		{
			socket.close();
			socket.connect(*endpoint_iter++, error);
		}

		if (error)
			throw boost::system::system_error(error);
		for (;;)
		{
			boost::array<char, 128> buf;				//버퍼 오버런을 방지하기위해서 자동으로 배열의 크기결정
			boost::system::error_code error;

			size_t len = socket.read_some(boost::asio::buffer(buf), error);

			if (error == boost::asio::error::eof)
				break;
			else if (error)
				throw boost::system::system_error(error);

			cout.write(buf.data(), len);
		}
	}
	catch (exception& e)
	{
		cerr << e.what() << endl;
	}



}
*/