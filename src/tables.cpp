#include "tables.h"
#include <malloc.h>
#include <assert.h>

#include <vector>
#include <algorithm>
#include <execution>

#if defined(NDEBUG)
#define EXECUTION std::execution::par
#else
#define EXECUTION std::execution::seq
#endif

const int BUFFER_PADDING = 32; 

namespace tbl
{
	vector::vector()
	{
		buffer = 0;
		typesize = sizeof(int);
		buffercount = 0;
		count = 0;
	}

	vector::~vector()
	{
		free(buffer);
	} 

	void vector::init(size_t _typesize, int _count)
	{
		typesize = _typesize;
		buffercount = _count;
		count = _count;
		grow();
	}

	void vector::grow()
	{
		buffercount = buffercount + BUFFER_PADDING;
		buffer = realloc(buffer, buffercount * typesize);
	}

	table::table(const size_t *_desc, int _streamcount, int _count)
	{
		streamcount = _streamcount;
		streams = new vector[streamcount];

		free.init(sizeof(int));
		for(int i = 0; i < streamcount; ++i)
			streams[i].init(_desc[i], _count);
	}

	table::~table()
	{
		delete[] streams;
	}
		
	int table::create()
	{
		int id;
		if (free.size() > 0)
		{
			id = ((int*)free.begin())[free.size() - 1];
			free.pop();
		}
		else
		{
			for(int i = 0; i < streamcount; ++i)
				id = streams[i].push();
		}
		return id;
	}

	void table::destroy(int _id)
	{
		int id = free.push();
		((int*)free.begin())[id] = _id;
	}
		
	void table::parallel(update _update, int _batch) const
	{
		int size = streams[0].size();
		int count = size / _batch;
		int jobs = ((size % _batch) == 0) ? count : count + 1;

		std::vector<int> it(jobs);
		std::iota(std::begin(it), std::end(it), 0);
		std::for_each(EXECUTION, std::begin(it), std::end(it), [&](int i)
		{
			int start = i * _batch;
			int end = (start + _batch < size) ? start + _batch : size;
			(*_update)(this, start, end);
		});
	}		
}

