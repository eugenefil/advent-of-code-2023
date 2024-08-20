fn read_numbers(prefix: &str) -> Vec<u64> {
    let line = std::io::stdin().lines().next().unwrap().unwrap();
    let (pref, numbers) = line.split_once(':').unwrap();
    assert_eq!(pref, prefix);
    numbers
        .split_ascii_whitespace()
        .map(|s| s.parse::<u64>().unwrap())
        .collect()
}

fn main() {
    let times = read_numbers("Time");
    let distances = read_numbers("Distance");
    let mut product = 1u64;
    for (time, dist) in times.into_iter().zip(distances) {
        let discr = (time * time - 4 * dist) as f64;
        assert!(discr >= 0.0);
        let t1 = (((time as f64) - discr.sqrt()) / 2.0).floor(); // winning low hold time
        assert!(t1 >= 0.0);
        let t1 = (t1 as u64) + 1;
        let t2 = (((time as f64) + discr.sqrt()) / 2.0).ceil(); // winning high hold time
        let t2 = (t2 as u64) - 1;
        assert!(t1 <= t2);
        product *= t2 - t1 + 1;
    }
    println!("{product}");
}
