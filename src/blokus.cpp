#include <iostream>
#include "block.h"
#include "util.h"


void block_gen(std::vector<Block> &blocks, Block block, Position &pos)
{
	block.coords.push_back(pos);
	if (block.coords.size() == block.size)
	{
		blocks.push_back(block);
		return;
	}

	block.coords.reserve(block.size);
	for (std::vector<Position>::iterator iter = block.coords.begin(); iter != block.coords.end(); ++iter)
	{
		for (int idx = 0; idx < NELEMENTS(siblings); idx++)
		{
			Position newpos = *iter + siblings[idx];
			if (newpos != orignal_left && newpos != orignal_down && !block.has_pos(newpos))
			{
				block_gen(blocks, block, newpos);
			}
		}
	}
}

void blokus_tst(void)
{
	Block block;
	block.size = 3;
	std::vector<Block> blocks;
	
	block_gen(blocks, block, Position(0, 0));
	for (std::vector<Block>::iterator iter = blocks.begin(); iter != blocks.end(); ++iter)
		iter->show();

	delete_same_block(blocks);
    std::cout << "After delete" << std::endl;
	for (std::vector<Block>::iterator iter = blocks.begin(); iter != blocks.end(); ++iter)
		iter->show();
}

void blokus_tst1(void)
{
	Block a, b;
	a.size = 3;
	b.size = 3;

	a.coords.push_back(Position(0, 0));
	a.coords.push_back(Position(1, 0));
	a.coords.push_back(Position(2, 0));

	b.coords.push_back(Position(0, 0));
	b.coords.push_back(Position(0, 1));
	b.coords.push_back(Position(0, 2));
	a.same_with(b);
}

void blokus_tst2(void)
{
	Block a, b;
	a.size = 3;
	b.size = 3;

	a.coords.push_back(Position(0, 0));
	a.coords.push_back(Position(1, 0));
	a.coords.push_back(Position(0, 1));

	b.coords.push_back(Position(0, 0));
	b.coords.push_back(Position(0, 1));
	b.coords.push_back(Position(0, 2));
	a.same_with(b);
}

