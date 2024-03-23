#include <iostream>
#include <string>
#include <string_view>
#include <charconv>
#include <cassert>
#include <vector>
#include <algorithm>

std::vector<unsigned long> parse_seeds(std::string_view s)
{
    std::vector<unsigned long> seeds;
    assert(s.starts_with("seeds:"));
    s.remove_prefix(6);

    assert(!s.empty());
    while (!s.empty()) {
        assert(s.starts_with(' '));
        s.remove_prefix(1);

        unsigned long seed = 0;
        auto [ptr, ec] = std::from_chars(s.data(), s.data() + s.size(), seed);
        assert(ec == std::errc{});
        s.remove_prefix(ptr - s.data());
        seeds.push_back(seed);
    }
    return seeds;
}

struct Range {
    unsigned long dst, src, len;
};

struct Map {
    std::string name;
    std::vector<Range> ranges;
};

std::vector<Map> parse_maps()
{
    std::vector<Map> maps;
    for (std::string line; std::getline(std::cin, line);) {
        Map map;
        auto pos = line.find(" map:");
        assert(pos > 0 && pos != line.npos);
        map.name = line.substr(0, pos);

        while (std::getline(std::cin, line)) {
            if (line.empty()) {
                assert(!map.ranges.empty());
                break;
            }
            std::string_view s(line);

            unsigned long dst = 0;
            auto res = std::from_chars(s.data(), s.data() + s.size(), dst);
            assert(res.ec == std::errc{});
            s.remove_prefix(res.ptr - s.data());

            assert(s.starts_with(' '));
            s.remove_prefix(1);

            unsigned long src = 0;
            res = std::from_chars(s.data(), s.data() + s.size(), src);
            assert(res.ec == std::errc{});
            s.remove_prefix(res.ptr - s.data());

            assert(s.starts_with(' '));
            s.remove_prefix(1);

            unsigned long len = 0;
            res = std::from_chars(s.data(), s.data() + s.size(), len);
            assert(res.ec == std::errc{} && len);
            s.remove_prefix(res.ptr - s.data());

            map.ranges.push_back({dst, src, len});
        }
        maps.push_back(std::move(map));
    }
    return maps;
}

int main()
{
    std::string line;
    std::getline(std::cin, line);
    assert(std::cin);
    auto seeds = parse_seeds(line);

    std::getline(std::cin, line);
    assert(std::cin && line.empty());
    auto maps = parse_maps();

    for (const auto& map : maps) {
        for (auto& seed : seeds) {
            for (const auto& range : map.ranges) {
                if (range.src <= seed && seed < range.src + range.len) {
                    seed = range.dst + (seed - range.src);
                    break;
                }
            }
        }
    }
    std::cout << *std::min_element(seeds.begin(), seeds.end()) << '\n';
    return 0;
}
