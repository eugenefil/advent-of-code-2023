#include <iostream>
#include <string>
#include <string_view>
#include <cassert>
#include <unordered_map>
#include <charconv>

int main()
{
    unsigned long sum = 0;
    for (std::string line; std::getline(std::cin, line);) {
        std::string_view s(line);
        assert(s.starts_with("Game "));
        s.remove_prefix(5);

        unsigned long id = 0;
        auto res = std::from_chars(s.data(), s.data() + s.size(), id);
        assert(res.ec == std::errc{} && id);
        s.remove_prefix(res.ptr - s.data());

        assert(s.starts_with(':'));
        s.remove_prefix(1);

        std::unordered_map<std::string_view, unsigned long> max_cubes = {
            {"red", 1}, {"green", 1}, {"blue", 1}
        };
        assert(s.size() > 0);
        while (s.size() > 0) {
            assert(s.starts_with(' '));
            s.remove_prefix(1);

            unsigned long num = 0;
            res = std::from_chars(s.data(), s.data() + s.size(), num);
            assert(res.ec == std::errc{} && num);
            s.remove_prefix(res.ptr - s.data());

            assert(s.starts_with(' '));
            s.remove_prefix(1);

            auto pos = s.find_first_of(",;");
            auto color = s.substr(0, pos);
            if (num > max_cubes.at(color))
                max_cubes[color] = num;
            s.remove_prefix(pos != s.npos ? pos + 1 : s.size());
        }
        sum += max_cubes["red"] * max_cubes["green"] * max_cubes["blue"];
    }
    std::cout << sum << '\n';
    return 0;
}
