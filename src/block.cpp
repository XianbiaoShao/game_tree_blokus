#include <iostream>
#include <vector>
#include "block.h"
#include "util.h"


class BlockTree {
public:
	Block block;
	std::vector<Position> close_list;
};

typedef Position (*go_dir)(Position &cur, Position &pre);

Position go_left(Position &cur, Position &pre)
{
	// x + 1 -> y + 1
	// x - 1 -> y - 1
	// y + 1 -> x - 1
	// y - 1 -> x + 1
	return Position(cur.x - (cur.y - pre.y), cur.y + (cur.x - pre.x));
}

Position go_straight(Position &cur, Position &pre)
{
	// x + 1 -> x + 1
	// x - 1 -> x - 1
	// y + 1 -> y + 1
	// y - 1 -> y - 1
	return Position(cur.x + (cur.x - pre.x), cur.y + (cur.y - pre.y));
}

Position go_right(Position &cur, Position &pre)
{
	// x + 1 -> y - 1
	// x - 1 -> y + 1
	// y + 1 -> x + 1
	// y - 1 -> x - 1
	return Position(cur.x + (cur.y - pre.y), cur.y - (cur.x - pre.x));
}

go_dir go_directions[] = 
{
	go_left,
	go_straight,
	go_right,
};

bool __same_tree(BlockTree ltree, Position lcur, Position lpre, BlockTree rtree, Position rcur, Position rpre)
{
	if (ltree.block.coords.size() == 0)
		return true;

	for (int idx = 0; idx < NELEMENTS(go_directions); idx++)
	{
		Position lnew = go_directions[idx](lcur, lpre);
		if (ltree.block.has_pos(lnew))
		{
			Position rnew = go_directions[idx](rcur, rpre);
			if (rtree.block.has_pos(rnew))
			{
				ltree.close_list.push_back(lnew);
				ltree.block.delete_pos(lnew);
				
				rtree.close_list.push_back(rnew);
				rtree.block.delete_pos(rnew);

				if (!__same_tree(ltree, lnew, lcur, rtree, rnew, rcur))
					return false;
			}
			else
			{
				return false;
			}
		}
	}

	return true;
}

bool same_tree(Block &lblk, Position lcur, Position lpre, Block &rblk, Position rcur, Position rpre)
{
	BlockTree ltree, rtree;

	ltree.block = lblk;
	ltree.close_list.push_back(lpre);
	ltree.close_list.push_back(lcur);
	ltree.block.delete_pos(lpre);
	ltree.block.delete_pos(lcur);
	
	rtree.block = rblk;
	rtree.close_list.push_back(rpre);
	rtree.close_list.push_back(rcur);
	rtree.block.delete_pos(rpre);
	rtree.block.delete_pos(rcur);

	return __same_tree(ltree, lcur, lpre, rtree, rcur, rpre);
}

void delete_same_block(std::vector<Block> &blocks)
{
loop_start:
	for (std::vector<Block>::iterator iter = blocks.begin(); iter != blocks.end(); ++iter)
	{
		for (std::vector<Block>::iterator new_iter = iter + 1; new_iter != blocks.end(); ++new_iter)
		{
			if (iter->same_with(*new_iter)) {
				blocks.erase(new_iter);
				goto loop_start;
			}
		}
	}
}

bool Block::has_pos(Position &pos)
{
	for (std::vector<Position>::iterator iter = coords.begin(); iter != coords.end(); ++iter)
		if (iter->x == pos.x && iter->y == pos.y)
			return true;

	return false;
}

void Block::delete_pos(Position &pos)
{
	for (std::vector<Position>::iterator iter = coords.begin(); iter != coords.end(); ++iter)
	{
		if (iter->x == pos.x && iter->y == pos.y)
		{
			coords.erase(iter);
			return;
		}
	}
}

bool Block::same_with(Block &rhs)
{
	std::vector<Position> open_list, close_list;

	if (coords.size() != rhs.coords.size())
		return false;

	if (coords.size() == 0)
		return true;

	// 选取一个点为出发点
	Position lpre = *coords.begin();
	for (int idx = 0; idx < NELEMENTS(siblings); idx++)
	{
		// 依次往出发点4个方向遍历各个周边节点
		Position lcur = lpre + siblings[idx];
		if (!has_pos(lcur))
			continue;

		for (std::vector<Position>::iterator iter = rhs.coords.begin(); iter != rhs.coords.end(); ++iter)
		{
			Position rpre = *iter;
			for (int idx = 0; idx < NELEMENTS(siblings); idx++)
			{
				Position rcur = rpre + siblings[idx];
				if (!rhs.has_pos(rcur))
					continue;

				if (!same_tree(*this, lcur, lpre, rhs, rcur, rpre))
					return false;
			}
		}
	}

	return true;
}


void Block::show()
{
	for (int y = size; y > -size; y--)
	{
		for (int x = -size; x < size; x++)
		{
			if (x == 0 && y == 0)
				std::cout << "# ";
			else if (has_pos(Position(x, y)))
				std::cout << "* ";
		}
		std::cout << std::endl;
	}
}

