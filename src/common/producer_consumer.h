/*
 * Reference: https://github.com/forhappy/Cplusplus-Concurrency-In-Practice/blob/master/zh/chapter11-Application/11.1%20Producer-Consumer-solution.md
 */

#ifndef _PRODUCER_CONSUMER_H_
#define _PRODUCER_CONSUMER_H_

#include <iostream>
#include <condition_variable>
#include <mutex>
#include <thread>
#include <vector>

#include <unistd.h>

namespace ec
{

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
		return consume(data);
		
	}

private:
	CRepository<T> & _repo;
    std::thread * _pthread;
};

}; // namespace ec

#endif
