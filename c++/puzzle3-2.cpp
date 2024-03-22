#include <iostream>
#include <string>
#include <string_view>
#include <vector>
#include <utility>
#include <cassert>
#include <charconv>
#include <cctype>
#include <functional>

struct Number {
    std::size_t start;
    std::size_t end;
    unsigned long val;
};

struct Symbol {
    std::size_t start;
    char ch;
};

using SchematicLine = std::pair<std::vector<Number>, std::vector<Symbol>>;

SchematicLine parse_line(std::string_view line)
{
    std::vector<Number> nums;
    std::vector<Symbol> syms;
    auto start = line.data();
    while (!line.empty()) {
        auto pos = line.find_first_not_of('.');
        if (pos == line.npos)
            break;
        line.remove_prefix(pos);

        pos = line.data() - start;
        if (std::isdigit(line[0])) {
            unsigned long val = 0;
            auto [ptr, ec] = std::from_chars(line.data(),
                line.data() + line.size(), val);
            assert(ec == std::errc{} && val);
            std::size_t len = ptr - line.data();
            nums.push_back({pos, pos + len - 1, val});
            line.remove_prefix(len);
        } else {
            syms.push_back({pos, line[0]});
            line.remove_prefix(1);
        }
    }
    return {nums, syms};
}

unsigned long sum_part_numbers(const SchematicLine& prev,
                               const SchematicLine& cur,
                               const SchematicLine& next)
{
    using Ref = std::reference_wrapper<const SchematicLine>;
    Ref lines[] = {prev, cur, next};
    unsigned long sum = 0;
    for (auto& sym : cur.second) {
        if (sym.ch != '*')
            continue;
        unsigned long vals[2] = { 0 };
        int i = 0;
        for (auto& line : lines) {
            for (auto& num : line.get().first) {
                if (sym.start > 0 && num.end < sym.start - 1)
                    continue;
                if (num.start > sym.start + 1)
                    break;
                if (i < 2)
                    vals[i] = num.val;
                ++i;
                if (i > 2)
                    break;
            }
            if (i > 2)
                break;
        }
        if (i == 2)
            sum += vals[0] * vals[1];
    }
    return sum;
}

int main()
{
    SchematicLine prev, cur, next;
    unsigned long total = 0;
    for (std::string line; std::getline(std::cin, line); ) {
        next = parse_line(line);
        auto sum = sum_part_numbers(prev, cur, next);
        std::cout << sum << '\n';
        total += sum;
        prev = std::move(cur);
        cur = std::move(next);
    }
    auto sum = sum_part_numbers(prev, cur, SchematicLine{});
    std::cout << sum << '\n';
    total += sum;
    std::cout << "total: " << total << '\n';
    return 0;
}
