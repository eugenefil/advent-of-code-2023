#include <iostream>
#include <string>
#include <cassert>
#include <vector>
#include <unordered_map>
#include <array>

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
    decltype(instructions)::size_type steps = 0;
    std::string node = "AAA";
    while (node != "ZZZ") {
        auto instr = instructions[steps++ % instructions.size()];
        node = net.at(node).at(instr);
    }
    std::cout << steps << '\n';
    return 0;
}
