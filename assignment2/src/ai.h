#ifndef __AI__
#define __AI__

#include <stdint.h>
#include <unistd.h>
#include "node.h"
#include "priority_queue.h"

#define init_size  1000

void initialize_ai();

move_t get_next_move( uint8_t board[SIZE][SIZE], int max_depth, propagation_t propagation );

void copy_board(uint8_t source[SIZE][SIZE],uint8_t target[SIZE][SIZE]);
#endif
