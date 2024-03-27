#include <iostream>
#include <string>
#include <cassert>
#include <vector>
#include <unordered_map>
#include <array>
#include <numeric>

using Node = std::array<std::string, 2>;
using Network = std::unordered_map<std::string, Node>;

enum Instruction : std::size_t { LEFT, RIGHT };

Network parse_network()
{
    Network net;
    for (std::string line; std::getline(std::cin, line);) {
        auto pos = line.find(' ');
        assert(pos == 3);

        auto lpos = line.find('(', pos);
        assert(lpos != line.npos);
        ++lpos;

        auto rpos = line.find(',', lpos);
        assert(rpos != line.npos && rpos - lpos == 3);
        ++rpos;
        assert(rpos < line.size() && line[rpos] == ' ');
        ++rpos;

        pos = line.find(')', rpos);
        assert(pos != line.npos && pos - rpos == 3);

        net[line.substr(0, 3)] = {line.substr(lpos, 3), line.substr(rpos, 3)};
    }
    return net;
}

std::vector<Instruction> parse_instructions()
{
    std::vector<Instruction> instructions;
    const std::unordered_map<char, Instruction> map = {
        {'L', LEFT}, {'R', RIGHT}
    };
    std::string line;
    std::getline(std::cin, line);
    assert(std::cin && !line.empty());
    for (auto ch : line)
        instructions.push_back(map.at(ch));

    std::getline(std::cin, line);
    assert(std::cin && line.empty());
    return instructions;
}

int main()
{
    const auto instructions = parse_instructions();
    const auto net = parse_network();

    std::vector<std::string> start_nodes;
    for (const auto& [node, _] : net) {
        if (node.ends_with('A'))
            start_nodes.push_back(node);
    }

    using size_type = decltype(instructions)::size_type;
    std::vector<size_type> node_steps;
    for (auto node : start_nodes) {
        size_type steps = 0;
        while (!node.ends_with('Z')) {
            auto instr = instructions[steps++ % instructions.size()];
            node = net.at(node).at(instr);
        }
        node_steps.push_back(steps);
    }

    size_type total = 1;
    for (auto steps : node_steps) {
        std::cout << steps << '\n';
        total = std::lcm(total, steps);
    }
    std::cout << "total: " << total << '\n';
    return 0;
}
