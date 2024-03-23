#include <iostream>
#include <string>
#include <string_view>
#include <charconv>
#include <cassert>
#include <vector>
#include <cmath>

std::vector<unsigned long> parse_vals(std::string_view s)
{
    std::vector<unsigned long> vals;
    auto pos = s.find_first_of(':');
    assert(pos > 0 && pos != s.npos);
    s.remove_prefix(pos + 1);

    assert(!s.empty());
    while (!s.empty()) {
        pos = s.find_first_not_of(' ');
        assert(pos > 0 && pos != s.npos);
        s.remove_prefix(pos);

        unsigned long val = 0;
        auto res = std::from_chars(s.data(), s.data() + s.size(), val);
        assert(res.ec == std::errc{} && val);
        s.remove_prefix(res.ptr - s.data());
        vals.push_back(val);
    }
    return vals;
}

unsigned long get_num_ways(unsigned long t, unsigned long d)
{
    float t_maxdistf = t / 2.0f;
    unsigned long t_maxdist = t / 2;

    assert(t * t >= 4 * d);
    float t_distf = (t - std::sqrt(t * t - 4.0f * d)) / 2.0f;
    assert(t_distf >= 0.0f);
    unsigned long t_dist = std::ceil(t_distf);
    if (t_dist == t_distf)
        ++t_dist;
    assert(t_dist <= t_maxdist);

    unsigned long n = (t_maxdist - t_dist + 1) * 2;
    if (t_maxdist == t_maxdistf)
        --n;
    return n;
}

int main()
{
    std::string line;
    std::getline(std::cin, line);
    assert(std::cin);
    auto times = parse_vals(line);

    std::getline(std::cin, line);
    assert(std::cin);
    auto distances = parse_vals(line);

    assert(distances.size() == times.size());
    unsigned long total = 1;
    for (decltype(times)::size_type i = 0; i < times.size(); ++i) {
        auto t = times[i];
        auto d = distances[i];
        total *= get_num_ways(t, d);
    }
    std::cout << total << '\n';
    return 0;
}
