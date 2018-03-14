#include <iostream>

#include <boost\asio.hpp>
#include <boost\thread.hpp>
#include <boost\bind.hpp>
#include <boost\date_time\posix_time\posix_time.hpp>

using namespace std;

boost::asio::io_service g_io;

class Printer
{
private:
	boost::asio::deadline_timer timer1_;
	boost::asio::deadline_timer timer2_;
	//boost::asio::strand strand_;
	boost::asio::io_service::strand strand_;		//멀티스레드 프로그래밍을 할 때 하나의 strand 객체를 이용해서는 그 이점을 살릴 수 없다
													//strand 사용의 이점: 컨텍스트스위칭 시 추가비용 절감, 동시성 완벽보장, 
	int count_;

public:
	Printer(boost::asio::io_service& io) : strand_(io),
		timer1_(io, boost::posix_time::seconds(1)),
		timer2_(io, boost::posix_time::seconds(2)),
		count_(0)
	{
		timer1_.async_wait(strand_.wrap(boost::bind(&Printer::Print1, this)));
		timer2_.async_wait(strand_.wrap(boost::bind(&Printer::Print2, this)));
	}

	void Print1()
	{
		if (count_ < 10)
		{
			cout << "Timer 1: " << count_ << "\n";
			++count_;

			timer1_.expires_at(timer1_.expires_at() + boost::posix_time::seconds(1));
			timer2_.async_wait(strand_.wrap(boost::bind(&Printer::Print1, this)));
		}
	}

	void Print2()
	{
		if (count_ < 10)
		{
			cout << "Timer 2: " << count_ << "\n";
			++count_;

			timer1_.expires_at(timer1_.expires_at() + boost::posix_time::seconds(1));
			timer2_.async_wait(strand_.wrap(boost::bind(&Printer::Print2, this)));
		}
	}

	~Printer()
	{
		cout << "Final count is" << count_ << "\n";
	}
};




int main()
{
	Printer p(g_io);
	boost::thread t(boost::bind(&boost::asio::io_service::run, &g_io));
	g_io.run();
	t.join();

	return 0;
}