use atoi::FromRadix10;
use std::str::FromStr;

struct Number {
    value: u32,
    start_pos: usize,
    end_pos: usize,
}

struct Symbol {
    ch: u8,
    pos: usize,
}

struct SchematicLine {
    numbers: Vec<Number>,
    symbols: Vec<Symbol>,
}

impl SchematicLine {
    fn sum(&self, prev: Option<&Self>, next: Option<&Self>) -> u32 {
        let mut sum: u32 = 0;
        let lines: Vec<&SchematicLine> = [prev, Some(self), next]
            .into_iter()
            .filter_map(|x| x)
            .collect();
        'symbol: for sym in &self.symbols {
            if sym.ch != b'*' {
                continue;
            }
            let mut adjacent = Vec::with_capacity(2);
            for line in &lines {
                for num in &line.numbers {
                    if num.end_pos < sym.pos.saturating_sub(1) {
                        continue;
                    } else if num.start_pos > sym.pos + 1 {
                        break;
                    }
                    if adjacent.len() == 2 {
                        continue 'symbol;
                    }
                    adjacent.push(num.value);
                }
            }
            if adjacent.len() == 2 {
                sum += adjacent.iter().product::<u32>();
            }
        }
        sum
    }
}

impl FromStr for SchematicLine {
    type Err = &'static str;

    fn from_str(s: &str) -> Result<Self, Self::Err> {
        let mut numbers = Vec::new();
        let mut symbols = Vec::new();
        let bytes = s.as_bytes();
        let mut pos = 0;
        while let Some(&ch) = bytes.get(pos) {
            if ch == b'.' {
                pos += 1;
            } else if ch.is_ascii_digit() {
                let (value, len) = u32::from_radix_10(&bytes[pos..]);
                assert!(len > 0);
                numbers.push(Number {
                    value,
                    start_pos: pos,
                    end_pos: pos + len - 1,
                });
                pos += len;
            } else {
                assert!(ch.is_ascii_graphic());
                symbols.push(Symbol { ch, pos });
                pos += 1;
            }
        }
        Ok(SchematicLine { numbers, symbols })
    }
}

fn main() {
    let mut sum: u32 = 0;
    let mut lines = std::io::stdin().lines().map(|s| s.unwrap());
    let mut prev: Option<SchematicLine> = None;
    let mut cur: SchematicLine = lines.next().unwrap().parse().unwrap();
    let mut next: SchematicLine;
    for line in lines {
        next = line.parse().unwrap();
        sum += cur.sum(prev.as_ref(), Some(&next));
        prev = Some(cur);
        cur = next;
    }
    sum += cur.sum(prev.as_ref(), None);
    println!("{sum}");
}
