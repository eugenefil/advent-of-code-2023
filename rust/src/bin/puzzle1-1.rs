fn main() {
    let mut sum: u32 = 0;
    for line in std::io::stdin().lines().map(|r| r.unwrap()) {
        let digit1 = line.chars().find(char::is_ascii_digit).unwrap();
        let digit2 = line.chars().rfind(char::is_ascii_digit).unwrap();
        sum += digit1.to_digit(10).unwrap() * 10 + digit2.to_digit(10).unwrap();
    }
    println!("{sum}");
}
