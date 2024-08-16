use std::collections::HashMap;

fn main() {
    let mut counts: HashMap<usize, u32> = HashMap::new();
    let mut last = 0;
    for (i, line) in std::io::stdin().lines().map(|s| s.unwrap()).enumerate() {
        *counts.entry(i).or_insert(0) += 1; // count original
        let cur_count = counts[&i];
        let (_, numbers) = line.split_once(':').unwrap();
        let (winning, ours) = numbers.split_once('|').unwrap();
        let winning: HashMap<&str, usize> = winning
            .split_ascii_whitespace()
            .zip([1].into_iter().cycle())
            .collect();
        let wins: usize = ours
            .split_ascii_whitespace()
            .filter_map(|n| winning.get(&n))
            .sum();
        for j in 1..=wins {
            *counts.entry(i + j).or_insert(0) += cur_count;
        }
        last = i;
    }
    let total = counts
        .iter()
        .map(|(&k, &v)| if k <= last { v } else { 0 })
        .sum::<u32>();
    println!("{total}");
}
