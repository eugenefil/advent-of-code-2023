fn main() {
    let mut sum: u32 = 0;
    for line in std::io::stdin().lines().map(|s| s.unwrap()) {
        let (_, cards) = line.split_once(':').unwrap();
        let (winning, ours) = cards.split_once('|').unwrap();
        let winning: std::collections::HashMap<&str, u32> = winning
            .split_ascii_whitespace()
            .zip([1].into_iter().cycle())
            .collect();
        let wins: u32 = ours
            .split_ascii_whitespace()
            .filter_map(|n| winning.get(&n))
            .sum();
        if wins > 0 {
            sum += 2u32.pow(wins - 1);
        }
    }
    println!("{sum}");
}
