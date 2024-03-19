#include <iostream>
#include <string>
#include <cassert>

int main()
{
    unsigned long sum = 0;
    for (std::string line; std::getline(std::cin, line);) {
        auto first = line.find_first_of("0123456789");
        assert(first != std::string::npos && line[first] != '0');
        auto last = line.find_last_of("0123456789");
        assert(last != std::string::npos);
        sum += std::stoul(std::string{line[first], line[last]});
    }
    std::cout << sum << '\n';
    return 0;
}
