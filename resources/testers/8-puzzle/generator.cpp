#include <functional>
#include <random>
#include <tuple>
#include <iostream>

using namespace std;

mt19937 prng(random_device{}());

template <int n, bool permute = true>
function make_board_test
{
    [](ostream& s)
    {
        vector<unsigned> a(n * n);
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
    generate("board_three", 10, make_board_test<2>);
    generate("board_four", 12, make_board_test<4>);
    generate("board_ten", 4, make_board_test<10>);
    generate("board_nineteen", 4, make_board_test<19>);

    /// test solver
    generate("solver_empty", 1, make_board_test<0>);
    generate("solver_one", 1, make_board_test<1>);
    generate("solver_two", 6, make_board_test<2>);
    generate("solver_three", 10, make_board_test<3>);
    generate("solver_four", 6, make_board_test<4>);

    /// test solver tricky
    generate("solver_three_solved", 1, make_board_test<3, false>);
    generate("solver_four_solved", 1, make_board_test<4, false>);
    generate("solver_five_solved", 1, make_board_test<5, false>);
}
