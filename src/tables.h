#pragma once

#include "stddef.h"

namespace tbl
{
	class vector
	{
	public:
		vector();
		~vector();

		void init(size_t _typesize, int _count = 0);
		int push() { if (buffercount == count) grow(); return count++; }
		void pop() { --count; }
		void clear() { count = 0; }
		int size() const { return count; }
		void *begin() const { return buffer; }

	private:
		vector(const vector &);
		void grow();

		void *buffer;        
		size_t typesize;        
		int buffercount;
		int count;
	};

	class table
	{
	public:
		table(const size_t *_desc, int _streamcount, int _count = 0);
		~table();

		int create();
		void destroy(int _id);
		const tbl::vector *stream(int _index) const { return streams + _index; }

		typedef void (*update)(const table *_table, int _start, int _end, void *_data);
		void parallel(update _update, int _batch, void *_data = 0, bool _parallel = true) const;

	private:
		table(const table &); 

		tbl::vector free;
		tbl::vector *streams;
		int streamcount;
	};
}

