#include <iostream>
#include <condition_variable>
#include <mutex>
#include <thread>

#include <unistd.h>


struct CItem
{
	int n;
};

#define T CItem

class CProducer
{
public:
	CProducer()
	{
		std::cout << "CProducer constructor, addr: " << this << std::endl;
		//CProducer::do_produce(this);
	}

	/*CProducer(CProducer * pprod)
	{
		std::cout << "CProducer constructor, addr: " << this << std::endl;
		CProducer::do_produce(pprod);
	}*/

	virtual ~CProducer()
	{
	}

	virtual int produce() 
	{
		T data;
		while(true)
		{
			// do somthing 
			sleep(1);
			std::cout << "do produce in CProducer, thread id: " << std::this_thread::get_id() << std::endl;

			produce(data);
		}
	}

	int produce(T & data) 
	{
		std::cout << "produce data in CProducer, thread id: " << std::this_thread::get_id() << std::endl;
		return 0;
	}

	void start()
	{
		//std::cout << "do_produce in " << std::this_thread::get_id() << std::endl;
		produce();
	}

	static void do_produce(CProducer * producer)
	{
		//std::cout << "do_produce in " << std::this_thread::get_id() << std::endl;
		producer->produce();
	}
};

class CMyProducer : public CProducer
{
public:
	CMyProducer()//:CProducer(this)
	{
		std::cout << "CMyProducer constructor, addr: " << this << std::endl;
	}

	virtual int produce()
	{
		T data;
		while(true)
		{
			data.n = rand();
			std::cout << "do produce in CMyProducer, thread id: " << std::this_thread::get_id()
					  << ", data: " << data.n << std::endl;
			CProducer::produce(data);
			sleep(1);
		}
		return 0;
	}
};

int main()
{
	CMyProducer prod1;
	prod1.start();
	return 0;
}

