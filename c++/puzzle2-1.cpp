#include <iostream>
#include <string>
#include <string_view>
#include <cassert>
#include <unordered_map>
#include <charconv>

int main()
{
    std::unordered_map<std::string_view, unsigned long> max_cubes = {
        {"red", 12}, {"green", 13}, {"blue", 14}
    };
    unsigned long sum = 0;
    for (std::string line; std::getline(std::cin, line);) {
        std::string_view s(line);
        assert(s.starts_with("Game "));
        s.remove_prefix(5);

        unsigned long id = 0;
        auto [ptr, ec] = std::from_chars(s.data(), s.data() + s.size(), id);
        assert(ec == std::errc{} && id);
        s.remove_prefix(ptr - s.data());

        assert(s.starts_with(':'));
        s.remove_prefix(1);

        assert(s.size() > 0);
        while (s.size() > 0) {
            assert(s.starts_with(' '));
            s.remove_prefix(1);

            unsigned long num = 0;
            auto [ptr, ec] = std::from_chars(s.data(), s.data() + s.size(), num);
            assert(ec == std::errc{} && num);
            s.remove_prefix(ptr - s.data());

            assert(s.starts_with(' '));
            s.remove_prefix(1);

            auto pos = s.find_first_of(",;");
            auto color = s.substr(0, pos);
            auto max = max_cubes.at(color);
            if (num > max) {
                id = 0;
                break;
            }
            s.remove_prefix(pos != s.npos ? pos + 1 : s.size());
        }
        sum += id;
    }
    std::cout << sum << '\n';
    return 0;
}
