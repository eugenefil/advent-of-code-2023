#include <iostream>
#include <string>
#include <cassert>

int main()
{
    const char* digits[10][2] = {
        {"zero", "0"}, {"one", "1"}, {"two", "2"}, {"three", "3"}, {"four", "4"},
        {"five", "5"}, {"six", "6"}, {"seven", "7"}, {"eight", "8"}, {"nine", "9"}
    };
    unsigned long sum = 0;
    for (std::string line; std::getline(std::cin, line);) {
        char ch1 = 0;
        auto first = line.find_first_of("0123456789");
        if (first != std::string::npos)
            ch1 = line[first];
        for (auto [digit, ch] : digits) {
            if (auto pos = line.find(digit); pos < first) {
                first = pos;
                ch1 = ch[0];
            }
        }
        assert(ch1);

        char ch2 = 0;
        auto last = line.find_last_of("0123456789");
        if (last != std::string::npos)
            ch2 = line[last];
        for (auto [digit, ch] : digits) {
            auto pos = line.rfind(digit);
            if (pos != std::string::npos && (pos > last || ch2 == 0)) {
                last = pos;
                ch2 = ch[0];
            }
        }
        assert(ch2);

        sum += std::stoul(std::string{ch1, ch2});
    }
    std::cout << sum << '\n';
    return 0;
}
