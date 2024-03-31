#include <iostream>
#include <string>
#include <vector>
#include <cassert>
#include <unordered_map>
#include <array>
#include <algorithm>
#include <fstream>

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
        return v.x + 1000 * v.y;
    }
};

template<>
struct std::hash<std::pair<Vec2d, Vec2d>> {
    std::size_t operator()(const std::pair<Vec2d, Vec2d>& p) const noexcept
    {
        return std::hash<Vec2d>()(p.first) ^ std::hash<Vec2d>()(p.second);
    }
};

struct Grid : std::vector<std::string> {
    char& operator[](const Vec2d& pos)
    {
        return this->at(pos.y).at(pos.x);
    }
};

static void dump_grid(const Grid& grid, const std::string& fname)
{
    std::ofstream f(fname);
    assert(f.is_open());
    for (const auto& row : grid)
        f << row << '\n';
    f.close();
}

struct Strip {
    int x0, x1, y, tiles;
    bool outer;
};
using StripGrid = std::vector<std::vector<Strip>>;

static Grid& mark_outer_strips(Grid& grid, const StripGrid& stgrid)
{
    for (const auto& strips : stgrid) {
        for (const auto& s : strips) {
            if (!s.outer)
                continue;
            auto& row = grid.at(s.y);
            std::fill(row.begin() + s.x0, row.begin() + s.x1 + 1, 'O');
        }
    }
    return grid;
}

int main()
{
    Grid grid;
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
    int w = grid.at(0).size();
    assert(w > 0);
    std::cout << "h " << h << " w " << w << '\n';

    // find ends connected to S
    const std::unordered_map<Vec2d, std::array<char, 3>> possible_joints = {
        {Vec2d::left,  {'-', 'L', 'F'}}, {Vec2d::up,    {'|', '7', 'F'}},
        {Vec2d::right, {'-', 'J', '7'}}, {Vec2d::down,  {'|', 'L', 'J'}}
    };
    struct End { Vec2d pos, prev_pos, dir; };
    std::vector<End> ends;
    for (const auto& dir : Vec2d::directions) {
        auto pt = S + dir;
        if (pt.x < 0 || pt.x >= w || pt.y < 0 || pt.y >= h)
            continue;
        const auto& joints = possible_joints.at(dir);
        if (std::find(joints.begin(), joints.end(), grid[pt]) != joints.end()) {
            ends.push_back({.pos = pt, .prev_pos = S, .dir = dir});
        }
    }
    assert(ends.size() == 2);
    std::cout << "ends " << ends[0].pos << ' ' << ends[1].pos << '\n';

    // determine S tile and replace in the grid
    const std::unordered_map<std::pair<Vec2d, Vec2d>, char> S_map = {
        {{Vec2d::left, Vec2d::up}, 'J'}, {{Vec2d::up, Vec2d::right}, 'L'},
        {{Vec2d::right, Vec2d::down}, 'F'}, {{Vec2d::left, Vec2d::down}, '7'},
        {{Vec2d::left, Vec2d::right}, '-'}, {{Vec2d::up, Vec2d::down}, '|'}
    };
    char S_tile = S_map.at({ends[0].dir, ends[1].dir});
    std::cout << "S tile " << S_tile << '\n';
    grid[S] = S_tile;

    // find the loop
    const std::unordered_map<char, std::array<Vec2d, 2>> tile_dirs = {
        {'|', {Vec2d::up, Vec2d::down}},   {'-', {Vec2d::left, Vec2d::right}},
        {'L', {Vec2d::up, Vec2d::right}},  {'J', {Vec2d::up, Vec2d::left}},
        {'7', {Vec2d::left, Vec2d::down}}, {'F', {Vec2d::right, Vec2d::down}}
    };
    std::unordered_map<Vec2d, bool> loop = {
        {S, 1}, {ends[0].pos, 1}, {ends[1].pos, 1}
    };
    while (ends[0].pos != ends[1].pos) {
        for (auto& end : ends) {
            bool found = false;
            for (const auto& dir : tile_dirs.at(grid[end.pos])) {
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
            loop[end.pos] = 1;
        }
    }
    std::cout << "farthest path " << loop.size() / 2 << '\n';

    // remove from the grid everything except loop
    for (int y = 0; y < h; ++y) {
        for (int x = 0; x < w; ++x) {
            if (!loop.contains({x, y}))
                grid[{x, y}] = '.';
        }
    }
    dump_grid(grid, "input10-clean");

    // insert space between rows to make room for horizontal corridors
    Grid new_grid;
    new_grid.reserve(grid.size() * 2 - 1);
    std::unordered_map<char, bool> up_joints = { {'|', 1}, {'7', 1}, {'F', 1} };
    for (int y = 0; y < h - 1; ++y) {
        auto& row = grid.at(y);
        std::string spacer_row(w, ' ');
        for (int x = 0; x < w; ++x) {
            if (up_joints.contains(row.at(x)))
                spacer_row.at(x) = '|'; // don't break joints
        }
        new_grid.push_back(std::move(row));
        new_grid.push_back(std::move(spacer_row));
    }
    new_grid.push_back(std::move(grid.back()));
    h = 2 * h - 1;

    // insert space between columns to make room for vertical corridors
    grid.clear();
    grid.reserve(new_grid.size());
    std::unordered_map<char, bool> left_joints = { {'-', 1}, {'L', 1}, {'F', 1} };
    for (int y = 0; y < h; ++y) {
        const auto& row = new_grid.at(y);
        std::string wide_row(2 * w - 1, ' ');
        for (int x = 0; x < w - 1; ++x) {
            wide_row.at(2 * x) = row.at(x);
            if (left_joints.contains(row.at(x)))
                wide_row.at(2 * x + 1) = '-'; // don't break joints
        }
        grid.push_back(std::move(wide_row));
    }
    for (int y = 0; y < h; ++y)
        grid.at(y).back() = new_grid.at(y).back();
    w = 2 * w - 1;
    dump_grid(grid, "input10-corridors");

    // parse horizontal strips of non-loop tiles
    StripGrid stgrid;
    stgrid.reserve(h);
    for (int y = 0; y < h; ++y) {
        const auto& row = grid.at(y);
        std::vector<Strip> strips;
        std::string::size_type x0 = 0;
        while (x0 < row.size()) {
            x0 = row.find_first_of(". ", x0);
            if (x0 == row.npos)
                break;
            auto x1 = row.find_first_not_of(". ", x0);
            if (x1 == row.npos)
                x1 = row.size();
            auto tiles = std::count(row.begin() + x0, row.begin() + x1, '.');
            strips.push_back(Strip(x0, x1 - 1, y, tiles, false));
            x0 = x1;
        }
        stgrid.push_back(std::move(strips));
    }

    // mark edge strips as outer
    for (auto& s : stgrid.at(0)) // mark top strips
        s.outer = true;
    for (int y = 1; y < h - 1; ++y) { // mark left and right edge strips
        auto& strips = stgrid.at(y);
        if (strips.empty())
            continue;
        if (strips.front().x0 == 0)
            strips.front().outer = true;
        if (strips.back().x1 == w - 1)
            strips.back().outer = true;
    }
    for (auto& s : stgrid.at(h - 1)) // mark bottom strips
        s.outer = true;

    // repeatedly mark remaining outer strips until there is no more
    bool marked = true;
    while (marked) {
        marked = false;

        // propagate outer strips top to bottom
        for (int y = 1; y < h - 1; ++y) {
            for (auto& strip : stgrid.at(y)) {
                if (strip.outer)
                    continue;
                for (const auto& ostrip : stgrid.at(y - 1)) {
                    if (!ostrip.outer)
                        continue;
                    if (ostrip.x1 < strip.x0)
                        continue;
                    if (ostrip.x0 > strip.x1)
                        break;
                    strip.outer = true;
                    marked = true;
                    break;
                }
            }
        }

        // propagate outer strips bottom to top
        for (int y = h - 2; y > 0; --y) {
            for (auto& strip : stgrid.at(y)) {
                if (strip.outer)
                    continue;
                for (const auto& ostrip : stgrid.at(y + 1)) {
                    if (!ostrip.outer)
                        continue;
                    if (ostrip.x1 < strip.x0)
                        continue;
                    if (ostrip.x0 > strip.x1)
                        break;
                    strip.outer = true;
                    marked = true;
                    break;
                }
            }
        }
    }
    dump_grid(mark_outer_strips(grid, stgrid), "input10-outside");

    int sum = 0;
    for (const auto& strips : stgrid)
        for (const auto& s : strips)
            if (!s.outer)
                sum += s.tiles;
    std::cout << sum << '\n';

    return 0;
}
