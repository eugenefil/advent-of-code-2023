import sys

sum = 0
for line in sys.stdin:
    hist = list(map(int, line.rstrip().split()))
    last_vals = []
    while not all([v == 0 for v in hist]):
        last_vals.append(hist[-1])
        hist = [nxt - prev for prev, nxt in zip(hist[:-1], hist[1:])]

    new_val = 0
    for v in last_vals:
        new_val += v
    # print(new_val)
    sum += new_val
print(sum)
