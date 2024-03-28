#include <iostream>
#include <string>
#include <string_view>
#include <charconv>
#include <cassert>
#include <vector>
#include <algorithm>

int main()
{
    long sum = 0;
    for (std::string line; std::getline(std::cin, line);) {
        std::vector<long> vals;
        std::string_view s(line);
        assert(!s.empty());
        while (!s.empty()) {
            auto pos = s.find_first_not_of(' ');
            assert(pos != s.npos);
            s.remove_prefix(pos);

            long val = 0;
            auto [ptr, ec] = std::from_chars(s.data(), s.data() + s.size(), val);
            assert(ec == std::errc{});
            s.remove_prefix(ptr - s.data());
            vals.push_back(val);
        }

        decltype(vals) last_vals;
        auto is_zero = [](long n) { return n == 0; };
        while (!std::all_of(vals.begin(), vals.end(), is_zero)) {
            assert(vals.size() > 1);
            for (decltype(vals)::size_type i = 1; i < vals.size(); ++i) {
                vals[i - 1] = vals[i] - vals[i - 1];
            }
            last_vals.push_back(vals.back());
            vals.pop_back();
        }

        std::reverse(last_vals.begin(), last_vals.end());
        long extrap = 0;
        for (auto v : last_vals)
            extrap += v;

        sum += extrap;
    }
    std::cout << sum << '\n';
    return 0;
}
