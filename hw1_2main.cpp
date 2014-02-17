#include "hw1_2.h"

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
