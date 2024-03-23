#include <iostream>
#include <string>
#include <string_view>
#include <charconv>
#include <cassert>
#include <vector>
#include <algorithm>

struct Range {
    unsigned long start, end, dst;
    auto operator<=>(const Range&) const = default;
};

struct Map {
    std::string name;
    std::vector<Range> ranges;
};

std::vector<Range> parse_seeds(std::string_view s)
{
    std::vector<Range> seeds;
    assert(s.starts_with("seeds:"));
    s.remove_prefix(6);

    assert(!s.empty());
    while (!s.empty()) {
        assert(s.starts_with(' '));
        s.remove_prefix(1);

        unsigned long start = 0;
        auto res = std::from_chars(s.data(), s.data() + s.size(), start);
        assert(res.ec == std::errc{});
        s.remove_prefix(res.ptr - s.data());

        assert(s.starts_with(' '));
        s.remove_prefix(1);

        unsigned long len = 0;
        res = std::from_chars(s.data(), s.data() + s.size(), len);
        assert(res.ec == std::errc{});
        s.remove_prefix(res.ptr - s.data());

        seeds.push_back({start, start + len - 1, 0});
    }
    return seeds;
}

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

            unsigned long start = 0;
            res = std::from_chars(s.data(), s.data() + s.size(), start);
            assert(res.ec == std::errc{});
            s.remove_prefix(res.ptr - s.data());

            assert(s.starts_with(' '));
            s.remove_prefix(1);

            unsigned long len = 0;
            res = std::from_chars(s.data(), s.data() + s.size(), len);
            assert(res.ec == std::errc{} && len);
            s.remove_prefix(res.ptr - s.data());

            map.ranges.push_back({start, start + len - 1, dst});
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
        for (decltype(seeds)::size_type i = 0; i < seeds.size(); ++i) {
            // copy by value, b/c we may invalidate ref with push_back
            auto seed = seeds[i];
            for (const auto& rng : map.ranges) {
                if (rng.end < seed.start || rng.start > seed.end)
                    continue;
                if (seed.start < rng.start) {
                    seeds.push_back({seed.start, rng.start - 1, 0});
                    seed.start = rng.start;
                }
                if (seed.end > rng.end) {
                    seeds.push_back({rng.end + 1, seed.end, 0});
                    seed.end = rng.end;
                }
                seed.start = rng.dst + (seed.start - rng.start);
                seed.end = rng.dst + (seed.end - rng.start);
                seeds[i] = seed; // copy back
                break;
            }
        }
    }
    std::cout << (*std::min_element(seeds.begin(), seeds.end())).start << '\n';
    return 0;
}
