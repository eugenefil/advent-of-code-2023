import sys
from functools import cmp_to_key

def set_type(hand):
    h = sorted(hand['hand'])
    groups = []
    max_n = 0
    while h:
        c = h.pop(0)
        n = 1
        while h:
            if h[0] == c:
                n += 1
                h.pop(0)
            else:
                break
        groups.append({'label': c, 'count': n})
        if n > max_n:
            max_n = n
    hand['groups'] = groups

    if len(groups) == 1:
        hand['type'] = 7 # 5 of a kind
    elif len(groups) == 2:
        if max_n == 4:
            hand['type'] = 6 # 4 of a kind
        else:
            hand['type'] = 5 # full house
    elif len(groups) == 3:
        if max_n == 3:
            hand['type'] = 4 # 3 of a kind
        else:
            hand['type'] = 3 # 2 pair
    elif len(groups) == 4:
        hand['type'] = 2 # 1 pair
    else:
        hand['type'] = 1 # high card


strength = {
    "2": 2,
    "3": 3,
    "4": 4,
    "5": 5,
    "6": 6,
    "7": 7,
    "8": 8,
    "9": 9,
    "T": 10,
    "J": 11,
    "Q": 12,
    "K": 13,
    "A": 14
}

def handcmp(hand1, hand2):
    t1 = hand1['type']
    t2 = hand2['type']
    if t1 < t2:
        return -1
    elif t1 > t2:
        return 1
    else:
        for c1, c2 in zip(hand1['hand'], hand2['hand']):
            st1 = strength[c1]
            st2 = strength[c2]
            if st1 < st2:
                return -1
            elif st1 > st2:
                return 1
    assert(0)
    return 0


hands = []
for line in sys.stdin:
    hand, bid = line.rstrip().split()
    hand = dict(hand=hand, bid=int(bid))
    set_type(hand)
    hands.append(hand)
    # print(hand['hand'], hand['type'])

hands.sort(key=cmp_to_key(handcmp))
sum = 0
for i, hand in enumerate(hands):
    sum += (i + 1) * hand['bid']
print(sum)
