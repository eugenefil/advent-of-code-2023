#include <iostream>
#include <string>
#include <charconv>
#include <cassert>
#include <vector>
#include <algorithm>
#include <unordered_map>

enum class Type { HIGH, ONE, TWO, THREE, HOUSE, FOUR, FIVE };

static std::unordered_map<char, unsigned> strength;

struct Hand {
    std::string cards;
    unsigned long bid;
    Type type;

    bool operator<(const Hand& other) const
    {
        if (type != other.type)
            return type < other.type;
        else {
            assert(cards.size() == other.cards.size());
            for (decltype(cards)::size_type i = 0; i < cards.size(); ++i) {
                auto card = cards[i];
                auto oth = other.cards[i];
                if (card != oth)
                    return strength.at(card) < strength.at(oth);
            }
            assert(0);
        }
    }
};

Type get_type(std::string cards)
{
    std::sort(cards.begin(), cards.end());
    std::vector<unsigned> groups;
    auto cur = cards[0];
    unsigned group = 1;
    for (auto card : cards.substr(1)) {
        if (card == cur)
            ++group;
        else {
            groups.push_back(group);
            cur = card;
            group = 1;
        }
    }
    groups.push_back(group);

    auto n = groups.size();
    assert(n > 0 && n <= 5);
    if (n == 1)
        return Type::FIVE;
    else if (n == 2)
        return groups[0] * groups[1] == 4 ? Type::FOUR : Type::HOUSE;
    else if (n == 3)
        return groups[0] * groups[1] % 2 ? Type::THREE : Type::TWO;
    else if (n == 4)
        return Type::ONE;
    else
        return Type::HIGH;
}

std::vector<Hand> parse_hands()
{
    std::vector<Hand> hands;
    for (std::string line; getline(std::cin, line);) {
        auto pos = line.find(' ');
        assert(pos == 5);
        std::string cards = line.substr(0, pos);

        unsigned long bid = 0;
        auto res = std::from_chars(line.data() + pos + 1,
            line.data() + line.size(), bid);
        assert(res.ec == std::errc{} && bid);

        auto type = get_type(cards);
        hands.push_back({std::move(cards), bid, type});
    }
    return hands;
}

int main()
{
    std::string labels = "AKQJT98765432";
    for (std::string::size_type i = 0; i < labels.size(); ++i)
        strength[labels[i]] = labels.size() - i;

    auto hands = parse_hands();
    std::sort(hands.begin(), hands.end());

    unsigned long sum = 0;
    for (decltype(hands)::size_type i = 0; i < hands.size(); ++i)
        sum += (i + 1) * hands[i].bid;
    std::cout << sum << '\n';
    return 0;
}
