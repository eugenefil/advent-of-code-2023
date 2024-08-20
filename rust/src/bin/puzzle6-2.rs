fn read_number(prefix: &str) -> u64 {
    let line = std::io::stdin().lines().next().unwrap().unwrap();
    let (pref, parts) = line.split_once(':').unwrap();
    assert_eq!(pref, prefix);
    parts.replace(' ', "").parse::<u64>().unwrap()
}

fn main() {
    let time = read_number("Time");
    let dist = read_number("Distance");
    let discr = (time * time - 4 * dist) as f64;
    assert!(discr >= 0.0);
    let t1 = (((time as f64) - discr.sqrt()) / 2.0).floor(); // winning low hold time
    assert!(t1 >= 0.0);
    let t1 = (t1 as u64) + 1;
    let t2 = (((time as f64) + discr.sqrt()) / 2.0).ceil(); // winning high hold time
    let t2 = (t2 as u64) - 1;
    assert!(t1 <= t2);
    println!("{}", t2 - t1 + 1);
}
