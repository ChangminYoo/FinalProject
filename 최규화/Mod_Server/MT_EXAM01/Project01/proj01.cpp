#include <thread>
#include <condition_variable>
#include <mutex>
#include <future>
#include <iostream>
#include <chrono>
#include <queue>

using namespace std;
using namespace chrono;
using namespace this_thread;

queue<int> g_que;
mutex g_mtx;
condition_variable g_quevar;

void Print(const int& n, const int& v);

//val 시간의 차이를 두고 전역에 선언한 큐에 val + i 데이터를 집어넣음
void provider(int val)
{
	for (auto i = 0; i < 10; ++i)
	{
		{
			lock_guard<mutex> lg(g_mtx);	//잠금획득
			g_que.push(val + i);
		}									// 잠금해제

		g_quevar.notify_one();				//하나의 스레드를 깨움

		sleep_for(milliseconds(val));		//val시간 동안 대기(밀리세컨드)
	}
}

// 값을 꺼낸다
void consumer(int num)
{	
	while (true)
	{
		int val;
		{
			unique_lock<mutex> ul(g_mtx);
			g_quevar.wait(ul, []() { return !g_que.empty(); });		// 큐에 데이터가 하나라도 들어갈 때까지 기다림
			val = g_que.front();
			g_que.pop();	
		}															// 잠금해제

		Print(num,val);
	}

}

void Print(const int& n, const int& v)
{
	unique_lock<mutex> ul(g_mtx);
	if (ul.owns_lock())
	{
		cout << "consumer :" << n << " : " << v << endl;
	}
	else
	{
		cout << "잠금획득실패" << endl;
	}
}

int main()
{
	auto p1 = async(launch::async, provider, 100);
	auto p2 = async(launch::async, provider, 300);
	auto p3 = async(launch::async, provider, 500);

	auto c1 = async(launch::async, consumer, 1);
	auto c2 = async(launch::async, consumer, 2);
	
}