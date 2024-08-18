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
    let mut seed_ranges = parse_numbers(read_line().unwrap().split_once(':').unwrap().1)
        .chunks_exact(2)
        .map(|pair| {
            let [src, len] = pair.try_into().unwrap();
            (src, src + len - 1)
        })
        .collect::<Vec<(u64, u64)>>();
    assert_eq!(read_line().unwrap().len(), 0);

    let mut maps = Vec::new();
    while let Some(line) = read_line() {
        assert!(line.ends_with("map:"));
        let mut ranges = Vec::new();
        for line in std::io::stdin().lines().map(|s| s.unwrap()) {
            if line.len() == 0 {
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
        let mut i = 0usize;
        while i < seed_ranges.len() {
            let (seed_start, seed_end) = &mut seed_ranges[i];
            let mut new_seed_ranges = Vec::new();
            for range in &map.ranges {
                if *seed_end < range.src_start || *seed_start > range.src_end {
                    continue;
                }
                if *seed_start < range.src_start {
                    new_seed_ranges.push((*seed_start, range.src_start - 1));
                    *seed_start = range.src_start;
                }
                if *seed_end > range.src_end {
                    new_seed_ranges.push((range.src_end + 1, *seed_end));
                    *seed_end = range.src_end;
                }
                *seed_start = *seed_start - range.src_start + range.dst_start;
                *seed_end = *seed_end - range.src_start + range.dst_start;
                break;
            }
            seed_ranges.append(&mut new_seed_ranges);
            i += 1;
        }
    }
    println!("{}", seed_ranges.into_iter().map(|r| r.0).min().unwrap());
}
