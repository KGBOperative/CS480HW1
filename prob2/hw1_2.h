#ifndef __HW1
#define __HW1

#define debug false

#include <iostream>
#include <vector>
#include <set>
#include <memory>
#include <functional>
#include <algorithm>


struct board {
    // pointer to the previous board in the search chain
    std::shared_ptr<board> p;
    // the curent board state
    std::vector<int> b;
    // the distance from the start state to here
    int g;
    // the estimated distance from here to the start state
    int h;
    // the number of hops remaining to check
    int d;

    // default constructor
    board();

    // copy constructor
    board(const board &other);

    // copy other into this
    void copy(const board &other);

    // inits the board with the given tiles
    void init(int *nums);

    // get the value of the tile
    int &at(int i, int j);
};

// comparison function for board state sets
bool fn_cmp(const board &l, const board &r);

// comparison funciton for the f values of each board state
bool fn_fcmp(const board& l, const board& r);

// hamming distance heuristic
int h1(board &s, board &g);

// manhattan distance heuristic
int h2(board &s, board &g);

// A* search algorithm
void a(board &s, board &g, int d, std::function<int(board&, board&)> &h);

// returns the set of children of the current board
std::vector<board> children(board &b);

// builds the path from start to final
void print_path(board &b);

// print the given board to the console
void print(board &b);

#endif  // __HW1
