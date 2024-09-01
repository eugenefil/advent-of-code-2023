fn parse_numbers(s: &str) -> Vec<u64> {
    s.split_ascii_whitespace()
        .map(|s| s.parse::<u64>().unwrap())
        .collect()
}

fn read_line() -> Option<String> {
    Some(std::io::stdin().lines().next()?.unwrap())
}

struct Range {
    dst_start: u64,
    src_start: u64,
    src_end: u64,
}

struct Map {
    ranges: Vec<Range>,
}

fn main() {
    let mut seeds = parse_numbers(read_line().unwrap().split_once(':').unwrap().1);
    assert_eq!(read_line().unwrap().len(), 0);

    let mut maps = Vec::new();
    while let Some(line) = read_line() {
        assert!(line.ends_with("map:"));
        let mut ranges = Vec::new();
        for line in std::io::stdin().lines().map(|s| s.unwrap()) {
            if line.is_empty() {
                break;
            }
            let [dst, src, len] = parse_numbers(&line).try_into().unwrap();
            ranges.push(Range {
                dst_start: dst,
                src_start: src,
                src_end: src + len - 1,
            });
        }
        maps.push(Map { ranges });
    }

    for map in &maps {
        for seed in &mut seeds {
            for range in &map.ranges {
                if range.src_start <= *seed && *seed <= range.src_end {
                    *seed = *seed - range.src_start + range.dst_start;
                    break;
                }
            }
        }
    }
    println!("{}", seeds.into_iter().min().unwrap());
}
