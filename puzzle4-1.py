import sys

sum = 0
for line in sys.stdin:
    nums = line.strip().split(':')[1]
    win_nums, our_nums = [s.strip().split() for s in nums.split('|')]
    win_nums = [int(num) for num in win_nums]
    our_nums = [int(num) for num in our_nums]
    wins = len(set(our_nums).intersection(win_nums))
    if (wins):
        sum += 2**(wins - 1)
print(sum)
