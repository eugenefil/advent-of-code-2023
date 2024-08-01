use std::collections::HashMap;

fn main() {
    let mut sum: u32 = 0;
    let bag: HashMap<_, u32> = [("red", 12), ("green", 13), ("blue", 14)].into();
    'game: for line in std::io::stdin().lines().map(|x| x.unwrap()) {
        let (game_id, sets) = line.split_once(": ").unwrap();
        for set in sets.split("; ") {
            for cubes in set.split(", ") {
                let (n, color) = cubes.split_once(' ').unwrap();
                let n: u32 = n.parse().unwrap();
                if n > *bag.get(&color).unwrap() {
                    continue 'game;
                }
            }
        }
        let (_, id) = game_id.split_once(' ').unwrap();
        sum += id.parse::<u32>().unwrap();
    }
    println!("{sum}");
}
