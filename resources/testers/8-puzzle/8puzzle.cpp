#include <functional>
#include <iostream>
#include <sstream>
#include <sstream>
#include <vector>
#include <array>
#include "board.h"
#include "solver.h"

using namespace std;

stringstream out;

template <typename T>
void print(string name, T value) {
    out << name << ": " << value << std::endl;
}

void print(string name, Board board) {
    out << name << ": ";
    for (size_t i = 0; i < board.size(); ++i) {
        for (size_t j = 0; j < board.size(); ++j) {
            out << board[i][j] << ' ';
        }
    }
    out << std::endl;
}

void test_board(vector<vector<unsigned>> & a)
{
    Board board(a);
    print("is_goal", board.is_goal());
    print("is_solvable", board.is_solvable());
    print("manhattan", board.manhattan());
    print("hamming", board.hamming());
    print("board", board);
}

void test_solver(vector<vector<unsigned>> & a)
{
    Board board(a);
    auto solution = Solver::solve(board);

    print("moves", solution.moves());
    vector<unsigned> dummy;

    for (const Board &i: solution) {
        // Emulate printing, because someone
        // can build board instance just-in-time,
        // when it is really needed.
        // Now it will be fair.
        dummy.push_back(i[0][0]);
    }

    // don't print moves, because there might be different paths to solve

    if (solution.begin() != solution.end() && board.is_solvable()) {
        auto it = --solution.end();
        print("board", *it);
        print("result", it->is_solvable());
    } else {
        print("result", board.is_solvable());
    }
}

int main()
{
    std::cout << "SUITE_START" << std::endl;
    std::cout << "SUITE_INIT" << std::endl;

    std::array<std::string, 3> test;
    while (std::getline(std::cin, test[0])
           && std::getline(std::cin, test[1])
           && std::getline(std::cin, test[2])) {
        //out.clear(); this doesn't work :P
        std::istringstream iss(test[0]+" "+test[1]+" "+test[2]);
        string tag;
        size_t n;

        iss >> tag >> n;

        vector<vector<unsigned>> a(n, vector<unsigned>(n));

        for (auto &i: a)
        {
            for (auto &j: i) iss >> j;
        }

        cout << "TIME_START" << std::endl;

        if (tag == "board")
        {
            test_board(a);
        }
    	else if (tag == "solver")
        {
            print("board", Board(a));
            test_solver(a);
        }

        cout << "TIME_END" << std::endl;

        cout << out.str() << std::endl;
        out.str(""); // but this works :)
        cout << "TEST_DONE" << endl;
    }
}
