#include <functional>
#include <random>
#include <tuple>
#include <iostream>
#include <chrono>

using namespace std;

mt19937 prng(
    chrono::steady_clock::now().time_since_epoch().count()
);

vector<vector<unsigned>> b4 = {
    {1,14,0,9,11,5,13,8,6,3,15,12,7,4,10,2}, // unsolvable
    {1,14,11,3,8,6,5,15,12,7,9,4,2,10,13,0}, // unsolvable
    {1,3,0,7,5,14,8,9,15,2,11,12,4,6,10,13}, // 48 moves
    {5,2,9,0,14,6,1,4,3,10,15,8,13,11,7,12}, // 39 moves
    {1,0,8,7,11,5,14,12,4,15,10,2,13,9,6,3}, // 49 moves
    {7,3,2,0,8,15,14,4,9,1,11,5,6,13,10,12}, // 41 moves
    {7,2,12,11,5,1,14,15,6,9,0,3,10,13,8,4}, // 44 moves
    {1,14,0,9,11,5,13,8,6,3,15,12,7,4,10,2}, // unsolvable
    {1,7,10,12,11,15,14,2,6,0,8,13,3,5,4,9}, // 53 moves
    {1,4,12,10,2,3,8,0,13,15,6,5,11,14,9,7}, // 42 moves
    {1,0,4,8,6,14,12,15,9,13,11,3,10,7,2,5}, // 45 moves
    {1,13,10,7,0,6,11,2,14,12,8,15,5,4,9,3}, // 47 moves
    {1,4,3,12,10,7,14,11,0,8,2,6,13,5,9,15}, // unsolvable
    {1,15,4,13,6,9,7,10,11,2,12,14,0,5,8,3}, // 53 moves
    {1,5,6,10,8,0,9,12,2,14,13,4,15,7,3,11} // unsolvable
};

template <int n, bool permute = true, bool small = false>
function make_board_test
{
    [](ostream& s)
    {
        vector<unsigned> a(n * n);

        if (n == 4 && small) {
            a = b4[prng() % b4.size()];

        } else {
            uniform_int_distribution<int> index(0, n * n);

            for (size_t i = 0; i < a.size(); a[i] = i + 1, ++i);

            if (n != 0) {
                a.back() = 0;
            }

            if (n > 0 && permute)
            {
                for (size_t i = n * n - 1;;)
                {
                    if (i == 0) break;
                    size_t j = (index(prng) % i) + 1;
                    swap(a[i], a[j]);
                    --i;
                }
            }
        }

        s << n << '\n';
        for (const auto i: a)
        {
            s << i << " ";
        }
        s << '\n';
    }
};

void generate(string tag, size_t n, function<void(ostream&)> f) {
    cout << "TESTCATEGORY " << tag << '\n';
    tag = tag.substr(0, tag.find('_'));

    for (size_t j = 0; j < n; ++j)
    {
        cout << tag << '\n'; // for testing in different ways
        f(cout);

        if (j != n - 1)
        {
            cout << "TESTLOLKEKTEST" << '\n';
        }
    }
}

int main()
{
    /// test board
    generate("board_empty", 1, make_board_test<0>);
    generate("board_one", 1, make_board_test<1>);
    generate("board_two", 6, make_board_test<2>);
    generate("board_three", 15, make_board_test<2>);
    generate("board_four", 24, make_board_test<4>);
    generate("board_ten", 20, make_board_test<10>);
    generate("board_nineteen", 15, make_board_test<19>);

    /// test solver
    generate("solver_empty", 1, make_board_test<0>);
    generate("solver_one", 1, make_board_test<1>);
    generate("solver_two", 6, make_board_test<2>);
    generate("solver_three", 30, make_board_test<3>);
    generate("solver_four_small", 15, make_board_test<4, true, true>);
    generate("solver_four_full", 60, make_board_test<4>);
    generate("solver_five", 40, make_board_test<5>);

    /// test solver tricky
    generate("solver_three_solved", 1, make_board_test<3, false>);
    generate("solver_four_solved", 1, make_board_test<4, false>);
    generate("solver_five_solved", 1, make_board_test<5, false>);
}
