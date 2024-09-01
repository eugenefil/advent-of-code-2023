#[derive(PartialEq, Eq, PartialOrd, Ord, Hash, Debug)]
struct Card {
    strength: u8,
    label: char,
}

impl Card {
    fn from_label(label: char) -> Self {
        let strength = match label {
            '2' => 2,
            '3' => 3,
            '4' => 4,
            '5' => 5,
            '6' => 6,
            '7' => 7,
            '8' => 8,
            '9' => 9,
            'T' => 10,
            'J' => 11,
            'Q' => 12,
            'K' => 13,
            'A' => 14,
            _ => panic!(),
        };
        Card { strength, label }
    }
}

#[derive(PartialEq, Eq, PartialOrd, Ord)]
enum HandType {
    HighCard,
    OnePair,
    TwoPair,
    ThreeOfAKind,
    FullHouse,
    FourOfAKind,
    FiveOfAKind,
}

#[derive(PartialEq, Eq, PartialOrd, Ord)]
struct Hand {
    hand_type: HandType,
    cards: [Card; 5],
}

impl Hand {
    fn cards_to_type(cards: &[Card; 5]) -> HandType {
        let mut counts = std::collections::HashMap::new();
        cards
            .iter()
            .for_each(|c| *counts.entry(c).or_insert(0u32) += 1);
        let groups = counts.values().map(|&c| c).collect::<Vec<u32>>();
        match groups.len() {
            1 => HandType::FiveOfAKind,
            2 => match groups[0] {
                1 | 4 => HandType::FourOfAKind,
                _ => HandType::FullHouse,
            },
            3 => {
                if groups[0] == 2 || groups[1] == 2 {
                    HandType::TwoPair
                } else {
                    HandType::ThreeOfAKind
                }
            }
            4 => HandType::OnePair,
            _ => HandType::HighCard,
        }
    }
}

impl From<&str> for Hand {
    fn from(cards: &str) -> Self {
        let cards: [Card; 5] = cards
            .chars()
            .map(|label| Card::from_label(label))
            .collect::<Vec<Card>>()
            .try_into()
            .unwrap();
        let hand_type = Hand::cards_to_type(&cards);
        Self { cards, hand_type }
    }
}

#[derive(PartialEq, Eq, PartialOrd, Ord)]
struct Bid {
    hand: Hand,
    amount: usize,
}

fn main() {
    let mut bids = Vec::new();
    for line in std::io::stdin().lines().map(|s| s.unwrap()) {
        let (hand, amount) = line.split_once(' ').unwrap();
        bids.push(Bid {
            hand: hand.into(),
            amount: amount.trim_end().parse::<usize>().unwrap(),
        });
    }
    bids.sort();
    println!(
        "{}",
        bids.into_iter()
            .enumerate()
            .map(|(rank, bid)| (rank + 1) * bid.amount)
            .sum::<usize>()
    );
}
