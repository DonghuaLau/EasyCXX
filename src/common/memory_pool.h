#ifndef _MEMORY_POOL_H_
#define _MEMORY_POOL_H_

namespace easycxx
{

class EMemoryPool
{

private:
	int _capacity;

public:
	EMemoryPool():_capacity(1)
	{
	}

	EMemoryPool(int capacity):_capacity(capacity)
	{
	}

	~EMemoryPool()
	{
	}

	template <class value_type>
	value_type *construct(int size = 1)
	{
		if(size < 1 || size > _capacity)
		{
			return NULL; 
		}
		value_type *p = new value_type[size];
		return p;
	}

	template <class value_type, class param1_type>
	value_type *construct(param1_type param1, int size = 1)
	{
		if(size < 1 || size > _capacity)
		{
			return NULL; 
		}
		value_type *p = new value_type[size](param1);
		return p;
	}

	template <class value_type, class param1_type, class param2_type>
	value_type *construct(param1_type param1, param2_type param2, int size = 1)
	{
		if(size < 1 || size > _capacity)
		{
			return NULL; 
		}
		value_type *p = new value_type[size](param1, param2);
		return p;
	}

	template <class value_type, class param1_type, class param2_type, class param3_type>
	value_type *construct(param1_type param1, param2_type param2, param3_type param3, int size = 1)
	{
		if(size < 1 || size > _capacity)
		{
			return NULL; 
		}
		value_type *p = new value_type[size](param1, param2, param3);
		return p;
	}

	/*
	template <class value_type>
	value_type *resize(int old_size, int new_size)
	{
		if(size < 1 || size > _capacity)
		{
			return NULL; 
		}
		value_type *p = new value_type[size];
		return p;
	}
	*/

	template <class value_type>
	void destory(value_type *value)
	{
		delete[] value;
	}
	
};

}

#endif
