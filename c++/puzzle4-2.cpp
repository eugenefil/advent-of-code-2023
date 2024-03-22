#include <iostream>
#include <string>
#include <string_view>
#include <charconv>
#include <cassert>
#include <vector>
#include <unordered_map>

struct Card {
    std::unordered_map<unsigned long, bool> winning;
    std::vector<unsigned long> ours;
};

Card parse_line(std::string_view line)
{
    Card card;
    assert(line.starts_with("Card "));
    line.remove_prefix(5);

    auto pos = line.find(':');
    assert(pos != line.npos);
    line.remove_prefix(pos + 1);

    assert(!line.empty());
    while (!line.empty()) {
        pos = line.find_first_not_of(' ');
        assert(pos != line.npos);
        line.remove_prefix(pos);

        if (line.starts_with('|')) {
            line.remove_prefix(1);
            break;
        }

        unsigned long num = 0;
        auto [ptr, ec] = std::from_chars(line.data(),
            line.data() + line.size(), num);
        assert(ec == std::errc{});
        line.remove_prefix(ptr - line.data());
        card.winning[num] = true;
    }

    assert(!line.empty());
    while (!line.empty()) {
        pos = line.find_first_not_of(' ');
        assert(pos != line.npos);
        line.remove_prefix(pos);

        unsigned long num = 0;
        auto [ptr, ec] = std::from_chars(line.data(),
            line.data() + line.size(), num);
        assert(ec == std::errc{});
        line.remove_prefix(ptr - line.data());
        card.ours.push_back(num);
    }
    return card;
}

int main()
{
    unsigned long sum = 0;
    std::unordered_map<unsigned long, unsigned long> extra;
    unsigned long i = 1;
    for (std::string line; std::getline(std::cin, line); ++i) {
        unsigned long cur_cnt = 1 + extra[i];
        sum += cur_cnt;
        auto card = parse_line(line);
        unsigned long wins = 0;
        for (auto& num : card.ours)
            wins += card.winning.contains(num);
        for (; wins; --wins)
            extra[i + wins] += cur_cnt;
    }
    std::cout << sum << '\n';
    return 0;
}
