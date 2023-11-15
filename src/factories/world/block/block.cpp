#include "block.h"
#include "../world_constants.h"

Block::Block(BlockId id) : id(id) {}

Block::Block() : id(BlockId::Air) {}