fn main() {
    let mut sum: u32 = 0;
    let digits = &[
        "one", "two", "three", "four", "five", "six", "seven", "eight", "nine",
    ];
    for line in std::io::stdin().lines().map(|r| r.unwrap()) {
        let mut min = line.find(|c: char| c.is_ascii_digit()).unwrap();
        let mut digit1 = char::from(line.as_bytes()[min]).to_digit(10).unwrap();
        for (i, digit) in digits.iter().enumerate() {
            if let Some(pos) = line.find(digit) {
                if pos < min {
                    min = pos;
                    digit1 = (i + 1) as u32;
                }
            }
        }

        let mut max = line.rfind(|c: char| c.is_ascii_digit()).unwrap();
        let mut digit2 = char::from(line.as_bytes()[max]).to_digit(10).unwrap();
        for (i, digit) in digits.iter().enumerate() {
            if let Some(pos) = line.rfind(digit) {
                if pos > max {
                    max = pos;
                    digit2 = (i + 1) as u32;
                }
            }
        }
        sum += digit1 * 10 + digit2;
    }
    println!("{sum}");
}
