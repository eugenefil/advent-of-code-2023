#include <iostream>
#include <string>
#include <vector>
#include <cassert>
#include <unordered_map>
#include <array>
#include <algorithm>

struct Vec2d {
    int x = 0;
    int y = 0;
    static const Vec2d left, up, right, down;
    static const Vec2d directions[];

    bool operator==(const Vec2d&) const = default;

    friend Vec2d operator+(const Vec2d& lhs, const Vec2d& rhs)
    {
        return {lhs.x + rhs.x, lhs.y + rhs.y};
    }

    friend std::ostream& operator<<(std::ostream& out, const Vec2d& v)
    {
        out << '(' << v.x << ' ' << v.y << ')';
        return out;
    }
};

const Vec2d Vec2d::left = {-1, 0};
const Vec2d Vec2d::up = {0, -1};
const Vec2d Vec2d::right = {1, 0};
const Vec2d Vec2d::down = {0, 1};
const Vec2d Vec2d::directions[] = { left, up, right, down };

template<>
struct std::hash<Vec2d> {
    std::size_t operator()(const Vec2d& v) const noexcept
    {
        return v.x + 2 * v.y; // only directions are hashed
    }
};

int main()
{
    std::vector<std::string> grid;
    Vec2d S{-1, -1};
    long y = 0;
    for (std::string row; std::getline(std::cin, row); ++y) {
        grid.push_back(row);
        if (auto x = row.find('S'); x != row.npos)
            S = Vec2d(x, y);
    }
    assert(S != Vec2d(-1, -1));
    std::cout << "S " << S << '\n';
    int h = grid.size();
    assert(h > 0);
    int w = grid[0].size();
    assert(w > 0);

    const std::unordered_map<Vec2d, std::array<char, 3>> possible_joints = {
        {Vec2d::left,  {'-', 'L', 'F'}}, {Vec2d::up,    {'|', '7', 'F'}},
        {Vec2d::right, {'-', 'J', '7'}}, {Vec2d::down,  {'|', 'L', 'J'}}
    };
    struct End { Vec2d pos, prev_pos; };
    std::vector<End> ends;
    for (const auto& dir : Vec2d::directions) {
        auto pt = S + dir;
        if (pt.x < 0 || pt.x >= w || pt.y < 0 || pt.y >= h)
            continue;
        char tile = grid.at(pt.y).at(pt.x);
        const auto& joints = possible_joints.at(dir);
        if (std::find(joints.begin(), joints.end(), tile) != joints.end())
            ends.push_back({.pos = pt, .prev_pos = S});
    }
    assert(ends.size() == 2);
    std::cout << "ends " << ends[0].pos << ' ' << ends[1].pos << '\n';

    const std::unordered_map<char, std::array<Vec2d, 2>> tile_dirs = {
        {'|', {Vec2d::up, Vec2d::down}},   {'-', {Vec2d::left, Vec2d::right}},
        {'L', {Vec2d::up, Vec2d::right}},  {'J', {Vec2d::up, Vec2d::left}},
        {'7', {Vec2d::left, Vec2d::down}}, {'F', {Vec2d::right, Vec2d::down}}
    };
    unsigned steps = 1; // first step was to find both ends
    while (ends[0].pos != ends[1].pos) {
        for (auto& end : ends) {
            char tile = grid.at(end.pos.y).at(end.pos.x);
            bool found = false;
            for (const auto& dir : tile_dirs.at(tile)) {
                auto pt = end.pos + dir;
                if (pt.x < 0 || pt.x >= w || pt.y < 0 || pt.y >= h)
                    continue;
                if (pt == end.prev_pos)
                    continue;
                found = true;
                end.prev_pos = end.pos;
                end.pos = pt;
                break;
            }
            assert(found);
        }
        ++steps;
    }
    std::cout << steps << '\n';
    return 0;
}
