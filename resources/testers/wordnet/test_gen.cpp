/**
 * @author Artem K
 */

#include <fstream>
#include <iostream>
#include <random>
#include <vector>
#include <algorithm>
#include <iterator>

void runCategory(const std::string &category, const std::function<std::vector<std::string>()> &test)
{
    std::cout << "TESTCATEGORY " << category << std::endl;
    for (std::size_t i = 0; i < 50; ++i) {
        if (i != 0) {
            std::cout << "TESTLOLKEKTEST\n";
        }
        const auto query = test();
        std::copy(query.begin(), query.end(), std::ostream_iterator<std::string>(std::cout, " "));
        std::cout << std::endl;
    }
}

int main()
{
    std::ifstream in("words.txt");

    std::vector<std::string> words;
    for (std::string w; in >> w; words.push_back(w));

    std::mt19937 rng(std::random_device{}());
    std::uniform_int_distribution<std::size_t> wordDist(0, words.size() - 1);

    runCategory("three", [&] {
        std::vector<std::string> query;
        std::sample(words.begin(), words.end(), std::back_inserter(query), 3, rng);
        std::shuffle(query.begin(), query.end(), rng);
        return query;
    });

    const std::vector<std::pair<std::string, std::size_t>> unique {
        {"primitive", 5},
        {"small", 15},
        {"medium", 80},
        {"big", 500},
    };

    for (const auto &[name, maxSize] : unique) {
        std::uniform_int_distribution<std::size_t> sizeDist(1, maxSize);
        runCategory(name, [&] {
            std::vector<std::string> query;
            std::sample(words.begin(), words.end(), std::back_inserter(query), sizeDist(rng), rng);
            std::shuffle(query.begin(), query.end(), rng);
            return query;
        });
    }

    const std::vector<std::size_t> repeated {10, 25, 50, 75, 90};

    for (const auto perc : repeated) {
        runCategory("repeated_" + std::to_string(perc), [&] {
            std::vector<std::string> query;
            query.reserve(100);
            std::sample(words.begin(), words.end(), std::back_inserter(query), 100 - perc, rng);
            while (query.size() != 100) {
                std::sample(query.begin(), query.end(), std::back_inserter(query), 100 - query.size(), rng);
            }
            std::shuffle(query.begin(), query.end(), rng);
            return query;
        });
    }

    return 0;
}
