/*
 * Reference: https://github.com/forhappy/Cplusplus-Concurrency-In-Practice/blob/master/zh/chapter11-Application/11.1%20Producer-Consumer-solution.md
 */

#include "producer_consumer.h"

#define ECTEST
#ifdef ECTEST

struct CItem
{
	int n;
};

#define TT CItem

class CMyProducer : public ec::CProducer<TT>
{
public:
	CMyProducer(ec::CRepository<TT> & repo):ec::CProducer<TT>(repo)
	{
		std::cout << "CMyProducer constructor, addr: " << this << std::endl;
	}

	virtual int produce()
	{
		TT data;
		while(true)
		{
			data.n = rand();
			std::cout << "do produce in CMyProducer, thread id: " << std::this_thread::get_id()
					  << ", data: " << data.n << std::endl;
			//(static_cast<CProducer<TT>*>(this))->produce(data); // 两种方法都可以, dynamic_cast也可以
			ec::CProducer<TT>::produce(data);
			sleep(1);
		}
		return 0;
	}
};

class CMyConsumer : public ec::CConsumer<TT> 
{
public:
	CMyConsumer(ec::CRepository<TT> & repo):ec::CConsumer<TT>(repo)
	{
		std::cout << "CMyConsumer constructor, addr: " << this << std::endl;
	}

	virtual int consume(TT & data)
	{
		std::cout << "do consume in CMyConsumer, thread id: " << std::this_thread::get_id()
				  << ", data: " << data.n << std::endl;
		data.n = 0; // reset
		sleep(1);
		return 0;
	}
};


int main()
{
	const int producer_count = 5;
	const int consumer_count = 3;

	ec::CRepository<CItem> repo(10);
	std::vector<CMyProducer*> prod_list(producer_count);
	std::vector<CMyConsumer*> cons_list(consumer_count);

	for(int i = 0; i < producer_count; i++)
	{
		CMyProducer * prod = new CMyProducer(repo);
		prod->start();
		prod_list[i] = prod;
	}
	for(int i = 0; i < consumer_count; i++)
	{
		CMyConsumer * cons = new CMyConsumer(repo);
		cons->start();
		cons_list[i] = cons;
	}

	for(int i = 0; i < producer_count; i++)
	{
		prod_list[i]->join();
	}
	for(int i = 0; i < consumer_count; i++)
	{
		cons_list[i]->join();
	}

	return 0;
}

#endif
