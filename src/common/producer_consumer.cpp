/*
 * Reference: https://github.com/forhappy/Cplusplus-Concurrency-In-Practice/blob/master/zh/chapter11-Application/11.1%20Producer-Consumer-solution.md
 */

#include <iostream>
#include <condition_variable>
#include <mutex>
#include <thread>
#include <vector>

#include <unistd.h>

//using namespace std;
//#define ECTEST 1

template<typename T>
class CRepository
{
public:
	CRepository(const int buf_len)
		:_buf_len(buf_len)
		,_produce_pos(0)
		,_consume_pos(0)
	{
		std::cout << "CRepository constructor, addr: " << this << std::endl;
		if(_buf_len > 0)
		{
			_pbuf = new T[buf_len];
		}
	}
	
	virtual ~CRepository()
	{
		if(_buf_len > 0)
		{
			delete  _pbuf;
		}
	}
	
	void produce(T & data)
	{
    	std::unique_lock<std::mutex> lock(_mtx);

		std::cout << "CRepository::produce 1, _produce_pos: " << _produce_pos 
				  << ", _consume_pos: " << _consume_pos 
				  << ", tid: " << std::this_thread::get_id() << std::endl;

		// waiting if buffer is full
		while(((_produce_pos + 1) % _buf_len) == _consume_pos)
		{
        	std::cout << "producer(" << std::this_thread::get_id() 
					  << ") is waiting for an empty slot ...\n";

			_cond_not_full.wait(lock);
		}

		std::cout << "CRepository::produce, _produce_pos: " << _produce_pos 
				  << ", _consume_pos: " << _consume_pos 
				  << ", tid: " << std::this_thread::get_id() << std::endl;

		// copy data
		_pbuf[_produce_pos] = data;
		_produce_pos = (_produce_pos + 1 == _buf_len) ? 0 : _produce_pos + 1;

		// not_empty notify
		_cond_not_empty.notify_one();

		lock.unlock();
	}

	T & consume()
	{
    	std::unique_lock<std::mutex> lock(_mtx);

		std::cout << "CRepository::consume 1, _produce_pos: " << _produce_pos 
				  << ", _consume_pos: " << _consume_pos 
				  << ", tid: " << std::this_thread::get_id() << std::endl;

		// waiting if buffer is empty
		while(_produce_pos == _consume_pos)
		{
        	std::cout << "consumer(" << std::this_thread::get_id() 
					  << ") is waiting for data ...\n";

			_cond_not_empty.wait(lock);
		}

		std::cout << "CRepository::consume, _produce_pos: " << _produce_pos 
				  << ", _consume_pos: " << _consume_pos 
				  << ", tid: " << std::this_thread::get_id() << std::endl;

		// get data
		int pos = _consume_pos;
		_consume_pos = (_consume_pos + 1 == _buf_len) ? 0 : _consume_pos + 1;

		// not_full notify
		_cond_not_full.notify_one();

		lock.unlock();

		return  _pbuf[pos];
	}

private:
	int _buf_len;
	T *_pbuf;

	int _produce_pos;
	int _consume_pos;

	std::mutex _mtx;
    std::condition_variable _cond_not_full;
    std::condition_variable _cond_not_empty;
};

template<typename T>
class CProducer
{
public:
	CProducer(CRepository<T> & repo)
		:_repo(repo)
		,_pthread(NULL)
	{
		std::cout << "CProducer constructor, addr: " << this << ", repo addr: " << &repo << std::endl;
	}

	virtual ~CProducer()
	{
		if(_pthread)
		{
			_pthread->join();
			delete _pthread;
		}
	}

	void start()
	{
		if(_pthread == NULL)
		{
			_pthread = new std::thread(CProducer<T>::do_produce, this);
		}
	}

	void join()
	{
		if(_pthread)
		{
			_pthread->join();
		}
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
		_repo.produce(data);
		return 0;
	}

private:
	static void do_produce(CProducer<T> * producer)
	{
		producer->produce();
	}

private:
	CRepository<T> & _repo;
    std::thread * _pthread;
};

template<typename T>
class CConsumer
{
public:
	CConsumer(CRepository<T> & repo)
		:_repo(repo)
		,_pthread(NULL)
	{
		std::cout << "CConsumer constructor, addr: " << this << ", repo addr: " << &repo << std::endl;
	}

	virtual ~CConsumer()
	{
		if(_pthread)
		{
			_pthread->join();
			delete _pthread;
		}
	}

	void start()
	{
		if(_pthread == NULL)
		{
			_pthread = new std::thread(CConsumer<T>::do_consume, this);
		}
	}

	void join()
	{
		if(_pthread)
		{
			_pthread->join();
		}
	}

	virtual int consume(T & data) 
	{
		std::cout << "do consume in CConsumer, thread id: " << std::this_thread::get_id() << std::endl;
		return 0;
	}

private:
	static void do_consume(CConsumer<T> * consumer)
	{
		//std::cout << "do_consume in " << std::this_thread::get_id() << std::endl;
		while(true)
		{
			consumer->consume();
		}
	}

	int consume()
	{
		T data = _repo.consume();
		consume(data);
	}

private:
	CRepository<T> & _repo;
    std::thread * _pthread;
};


#ifdef ECTEST

struct CItem
{
	int n;
};

#define TT CItem

class CMyProducer : public CProducer<TT>
{
public:
	CMyProducer(CRepository<TT> & repo):CProducer<TT>(repo)
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
			CProducer<TT>::produce(data);
			sleep(1);
		}
		return 0;
	}
};

class CMyConsumer : public CConsumer<TT> 
{
public:
	CMyConsumer(CRepository<TT> & repo):CConsumer<TT>(repo)
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

	CRepository<CItem> repo(10);
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
