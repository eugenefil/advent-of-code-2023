fn main() {
    let mut sum: u32 = 0;
    for line in std::io::stdin().lines().map(|x| x.unwrap()) {
        let mut max_cubes = std::collections::HashMap::new();
        let (_, sets) = line.split_once(": ").unwrap();
        for set in sets.split("; ") {
            for cubes in set.split(", ") {
                let (n, color) = cubes.split_once(' ').unwrap();
                let n: u32 = n.parse().unwrap();
                let max = max_cubes.entry(color).or_insert(n);
                if n > *max {
                    *max = n;
                }
            }
        }
        sum += max_cubes.values().product::<u32>();
    }
    println!("{sum}");
}
