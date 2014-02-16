#define debug false

#include <iostream>
#include <vector>
#include <set>
#include <memory>
#include <functional>
#include <algorithm>

struct board {
    std::shared_ptr<board> p;
    std::vector<int> b;
    int g;
    int h;
    int d;

    board() {
        b = std::vector<int>(9, 0);
        p = nullptr;
    }

    board(const board &other) {
        b = std::vector<int>(other.b.begin(), other.b.end());
        g = other.g;
        h = other.h;
        d = other.d;
        p = other.p;
    }

    void copy(const board &other) {
        b = std::vector<int>(other.b);
        g = other.g;
        h = other.h;
        d = other.d;
        p = other.p;
    }

    void init(int *nums) { 
        g = 0;
        h = 0;
        for (int i = 0; i < 9; i++) {
            b[i] = nums[i];
        }
    }

    int &at(int i, int j) {
        return b[3*i+j];
    }
};

// comparison function for board
bool fn_cmp(const board &l, const board &r) {
    return l.b < r.b;
}

// comparison funciton to get the next board
bool fn_fcmp(const board& l, const board& r) {
    return l.g + l.h < r.g + r.h;
}

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

int main() {
    std::function<int(board&, board&)> fh1 = &h1;
    std::function<int(board&, board&)> fh2 = &h2;

    // init goal board
    board g;
    g.init(new int[9]{ 1, 2, 3, 8, 0, 4, 7, 6, 5 });

    // init start board
    board s;
    s.init(new int[9]{ 2, 8, 3, 1, 6, 4, 7, 0, 5 }); 

    std::cout << "starting state:\n";
    print(s);
    std::cout << "goal state:\n";
    print(g);

    std::cout << "A* search with hamming distance hueristic\n";
    a(s, g, 3, fh1);

    std::cout << "A* search with manhattan distance hueristic\n";
    a(s, g, 3, fh2);


    return 0;
}

int h1(board &s, board &g) {
    int h = 0;
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            if (s.at(i,j) != 0 && s.at(i,j) != g.at(i,j)) {
                h++;
            }
        }
    }

    return h;
}

int h2(board &s, board &g) {
    int h = 0;
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            if (s.at(i,j) != 0 && s.at(i,j) != g.at(i,j)) {
                int x = s.at(i,j);

                for (int a = 0; a < 3; a++) {
                    for (int b = 0; b < 3; b++) {
                        if (x == g.at(a,b)) {
                            int di = i - a;
                            int dj = j - b;
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
    
    std::set<board, bool(*)(const board&, const board&)> closed(&fn_cmp);
    std::set<board, bool(*)(const board&, const board&)> open(&fn_cmp);
    std::set<board, bool(*)(const board&, const board&)> final_open(&fn_cmp);

    std::vector<board> from;

    s.g = 0;
    s.h = h(s, g);
    s.d = d;

    board c;
    open.insert(s);
    while (!open.empty()) {
        auto curr_it = std::min_element(open.begin(), open.end(), &fn_fcmp);
        c.copy(*curr_it);
        
        open.erase(*curr_it);
        closed.insert(c);

        for (auto n : children(c)) {
            if (debug) {
                std::cout << "testing n in children c\n";
                print(n);
                std::cout << std::endl;
            }

            n.g++;
            n.h = h(n, g);
            n.d--;
            if (closed.find(n) != closed.end()) {
                if (debug) {
                    std::cout << "found n in closed set\n";
                    print(n);
                    std::cout << std::endl;
                }
                continue;
            } else if (n.d < 0 && final_open.find(n) == final_open.end()) {
                if (debug) {
                    std::cout << "putting n into final_open set\n";
                    print(n);
                    std::cout << std::endl;
                }
                final_open.insert(n);
            } else if (open.find(n) == open.end()) {
                if (debug) {
                    std::cout << "putting n into open set\n";
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

    std::cout << "Current path == \n";
    print_path(c);
    std::cout << "...\n\nClosed set == \n";
    for (auto b : closed) {
        print(b);
        std::cout << "------\n";
    }

    std::cout << "\nOpen set == \n";
    for (auto b : final_open) {
        print(b);
        std::cout << "------\n";
    }
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
