import sys

sum = 0
for line in sys.stdin:
    hist = list(map(int, line.rstrip().split()))
    first_vals = []
    while not all([v == 0 for v in hist]):
        first_vals.append(hist[0])
        hist = [nxt - prev for prev, nxt in zip(hist[:-1], hist[1:])]

    new_val = 0
    for v in reversed(first_vals):
        new_val = v - new_val
    # print(new_val)
    sum += new_val
print(sum)
