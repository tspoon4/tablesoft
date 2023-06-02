#include <stdio.h>
#include "tables.h"

struct example : public tbl::table
{
	enum Stream { STR_LIFE = 0, STR_XPOS, STR_COUNT };
	static constexpr size_t STREAMS[STR_COUNT] = { sizeof(float), sizeof(int) };

	example(int _count = 0) : tbl::table(STREAMS, STR_COUNT, _count) {}

	int create(float _life, int _posx)
	{
		int id = tbl::table::create();
		((float*)stream(STR_LIFE)->begin())[id] = _life;
		((int*)stream(STR_XPOS)->begin())[id] = _posx;
		return id;
	}

	static void update_range(const tbl::table *_table, int _start, int _end, void *_data)
	{		
		float* lifeptr = (float*)_table->stream(STR_LIFE)->begin() + _start;
		int* xposptr = (int*)_table->stream(STR_XPOS)->begin() + _start;

		for(int i = _start; i < _end; ++i)
		{
			*lifeptr -= 0.01f;
			*xposptr = *xposptr + 1;
			++lifeptr;
			++xposptr;
		}
	}

	static int test()
	{
		example table;
		int id1 = table.create(0.0f, 0);
		int id2 = table.create(0.0f, 0);
		table.destroy(id1);
		int id3 = table.create(2.0f, 2);

		table.parallel(&example::update_range, 12);

		printf("id: %d\n", id3);
		return 0;
	}
};

int main()
{
	return example::test();
}

