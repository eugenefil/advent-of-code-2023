#include <iostream>
#include <string>
#include <charconv>
#include <cassert>
#include <vector>
#include <cmath>
#include <algorithm>

unsigned long parse_val(std::string s)
{
    auto pos = s.find_first_of("0123456789");
    assert(pos > 0 && pos != s.npos);
    s.erase(0, pos);

    s.erase(std::remove(s.begin(), s.end(), ' '), s.end());

    unsigned long val = 0;
    auto res = std::from_chars(s.data(), s.data() + s.size(), val);
    assert(res.ec == std::errc{} && val);
    return val;
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
    auto t = parse_val(line);

    std::getline(std::cin, line);
    assert(std::cin);
    auto d = parse_val(line);

    std::cout << get_num_ways(t, d) << '\n';
    return 0;
}
