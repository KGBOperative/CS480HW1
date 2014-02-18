#include "hw1_2.h"

int h1(board &s, board &g) {
    int h = 0;
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            if (s.at(i,j) != 0 && s.at(i,j) != g.at(i,j)) {
                // the current tile is not where it should be
                // add one to the total
                h++;
            }
        }
    }

    return h;
}

int h2(board &s, board &g) {
    int h = 0;
    // find all non-zero x
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            if (s.at(i,j) != 0 && s.at(i,j) != g.at(i,j)) {
                int x = s.at(i,j);
                // find the position that x should be at
                for (int a = 0; a < 3; a++) {
                    for (int b = 0; b < 3; b++) {
                        if (x == g.at(a,b)) {
                            int di = i - a;
                            int dj = j - b;
                            // add the manhattan distance of x to it's goal
                            // position to total
                            h += di < 0 ? -di : di;
                            h += dj < 0 ? -dj : dj;

                        }
                    }
                }
            }
        }
    }

    return h;
}

void a(board &s, board &g, int d, std::function<int(board&, board&)> &h) {
    // the set containing the board states that have already been checked
    std::set<board, bool(*)(const board&, const board&)> closed(&fn_cmp);
    // the set containing the board states that have yet to be checked
    std::set<board, bool(*)(const board&, const board&)> open(&fn_cmp);

    // set the distance cost of the start node
    s.g = 0;
    // set the estimated cost to the goal state using the given funciton h
    s.h = h(s, g);
    // set the number of remaining node hops to check
    s.d = d;

    // a container for the current board state
    board c = s;
    open.insert(s);
    while (!open.empty() && c.d >= 0) {
        // get the board state from the open set with the smallest f value
        auto curr_it = std::min_element(open.begin(), open.end(), &fn_fcmp);
        c.copy(*curr_it);
        
        // move the current board state from the open to the closed set
        open.erase(*curr_it);
        closed.insert(c);

        // get the board states for the children of the current board state
        // check each child state individually
        for (auto n : children(c)) {
            if (debug) {
                std::cout << "testing n in children c\n";
                print(n);
                std::cout << std::endl;
            }

            // update the distance from the start state
            n.g++;
            // update the estimated distance to the goal state
            n.h = h(n, g);
            // reduce the node depth left to check
            n.d--;
            // if the current child state is in the closed set, ignore it
            if (closed.find(n) != closed.end()) {
                if (debug) {
                    std::cout << "found n in closed set\n";
                    print(n);
                    std::cout << std::endl;
                }
                continue;
            // attempt to add the current child state to the open set
            } else {
                if (debug) {
                    std::cout << "attempting to put n into open set\n";
                    print(n);
                    std::cout << std::endl;
                }
                open.insert(n);
            }
        }
        if (debug) {
            std::string tmp;
            std::cout << "open.size() = " << open.size();
            std::getline(std::cin, tmp);
        }
    }

    // all states within the max depth have been checked, output the current
    // search state to the console

    // output the path from the starting state to the current state
    std::cout << "Current path == \n";
    print_path(c);

    // print the board states that have been placed into the closed set
    std::cout << "...\n\nClosed set == \n";
    for (auto b : closed) {
        print(b);
        std::cout << "------\n";
    }

    // print the board states that are past the max depth that the search
    // algorthm has found but not checked
    std::cout << "\nOpen set == \n";
    for (auto b : open) {
        print(b);
        std::cout << "------\n";
    }
}

board::board() {
    b = std::vector<int>(9, 0);
    p = nullptr;
}

board::board(const board &other) {
    b = std::vector<int>(other.b.begin(), other.b.end());
    g = other.g;
    h = other.h;
    d = other.d;
    p = other.p;
}

void board::copy(const board &other) {
    b = std::vector<int>(other.b);
    g = other.g;
    h = other.h;
    d = other.d;
    p = other.p;
}

void board::init(int *nums) { 
    g = 0;
    h = 0;
    for (int i = 0; i < 9; i++) {
        b[i] = nums[i];
    }
}

int &board::at(int i, int j) {
    return b[3*i+j];
}

bool fn_cmp(const board &l, const board &r) {
    return l.b < r.b;
}

bool fn_fcmp(const board& l, const board& r) {
    return l.g + l.h < r.g + r.h;
}

std::vector<board> children(board &b) {
    std::vector<board> c;
    if (debug) {
        std::cout << "testing b\n";
        print(b);
        std::cout << std::endl;
    }

    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            if (b.at(i,j) == 0) {
                if (debug) std::cout << "b.at(i,j) == 0; i = " << i << "; j = " << j << std::endl;
                if (i - 1 >= 0) {
                    board n(b);
                    n.p = std::make_shared<board>(b);
                    n.at(i,j) = n.at(i-1,j);
                    n.at(i-1,j) = 0;
                    c.push_back(n);

                    if (debug) {
                        std::cout << "testing n, i-1\n";
                        print(n);
                        std::cout << std::endl;
                    }
                }
                if (i + 1 < 3) {
                    board n(b);
                    n.p = std::make_shared<board>(b);
                    n.at(i,j) = n.at(i+1,j);
                    n.at(i+1,j) = 0;
                    c.push_back(n);

                    if (debug) {
                        std::cout << "testing n, i+1\n";
                        print(n);
                        std::cout << std::endl;
                    }
                }
                if (j - 1 >= 0) {
                    board n(b);
                    n.p = std::make_shared<board>(b);
                    n.at(i,j) = n.at(i,j-1);
                    n.at(i,j-1) = 0;
                    c.push_back(n);

                    if (debug) {
                        std::cout << "testing n, j-1\n";
                        print(n);
                        std::cout << std::endl;
                    }
                }
                if (j + 1 < 3) {
                    board n(b);
                    n.p = std::make_shared<board>(b);
                    n.at(i,j) = n.at(i,j+1);
                    n.at(i,j+1) = 0;
                    c.push_back(n);

                    if (debug) {
                        std::cout << "testing n, j+1\n";
                        print(n);
                        std::cout << std::endl;
                    }
                }

                return c;
            }
        }
    }

    return c;
}

void print(board &b) {
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            if (b.at(i,j) == 0) {
                std::cout << "  ";
            }
            else {
                std::cout << b.at(i,j) << " ";
            }
        }
        std::cout << std::endl;
    }
    std::cout << "f = " << b.g << " + " <<  b.h << " = " << b.g + b.h << std::endl;
}

void print_path(board &b) {
    if (&b != nullptr) {
        print_path(*b.p);

        std::cout << std::endl;
        print(b);
        std::cout << "   ===>> ";
    }
}
