#ifndef INCLUDED_BLOCK
#define INCLUDED_BLOCK

#include <vector>


#define INVALID_X		100
#define INVALID_Y		100

class Position {
public:
	Position() : x(INVALID_X), y(INVALID_Y) {}
	Position(int xx, int yy) : x(xx), y(yy) {}
	bool operator==(Position &rhs) { return ((x == rhs.x) && (y == rhs.y)); }
	bool operator!=(Position &rhs) { return !(this->operator==(rhs)); }
	Position operator+(Position &rhs) { return Position(x + rhs.x, y + rhs.y); }
    int x, y;
};


static Position INVALID_POS;
static Position orignal_left(-1, 0);
static Position orignal_down(0, -1);
static Position siblings[] = {
	Position(-1,  0),
	Position( 0,  1),
	Position( 1,  0),
	Position( 0, -1),
};

class Block {
public:
	Block() : id(-1), size(0) {}
	bool has_pos(Position &pos);
	void delete_pos(Position &pos);
	bool same_with(Block &rhs);
	void show();

	
    int id;
    int size;
	int directon;
	std::vector<Position> coords;
};

void delete_same_block(std::vector<Block> &blocks);


#endif // INCLUDED_BLOCK
