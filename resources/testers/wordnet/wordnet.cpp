/**
 * @author Artem K
 */

#include "wordnet.h"

#include <iostream>
#include <iterator>
#include <string>
#include <vector>
#include <sstream>

int main()
{
    std::cout << "SUITE_START" << std::endl;

    WordNet wordnet("synsets.txt", "hypernyms.txt");
    Outcast outcast(wordnet);

    std::cout << "SUITE_INIT" << std::endl;

    std::string line;
    while (std::getline(std::cin, line)) {
        std::cerr << line << std::endl;
        std::istringstream iss(line);
        std::vector<std::string> words(std::istream_iterator<std::string>{iss},
                                       std::istream_iterator<std::string>{});
        std::cout << "TIME_START" << std::endl;
        std::string s = outcast.outcast(words);
        std::cout << "TIME_END" << std::endl;
        std::cout << s << std::endl;
        std::cout << "TEST_DONE" << std::endl;
    }
    return 0;
}
