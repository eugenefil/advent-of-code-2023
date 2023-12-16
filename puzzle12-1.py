import sys

def do_state(state):
    # print(state)
    i, groups_left, c = state
    while i < length:
        while i < length and cond[i] == '.':
            i += 1
        if i >= length:
            break
    
        if cond[i] == '#' or c == '#':
            c = None
            if groups_left == 0:
                return 0
            g = groups[num_groups - groups_left]
            groups_left -= 1
            i += 1
            g -= 1
            while i < length and g > 0 and cond[i] in ('#', '?'):
                i += 1
                g -= 1
            if g > 0:
                return 0
            if i >= length:
                break

            # there always must be a dot after a group (last or not)
            if cond[i] == '#':
                return 0
        else: # ?
            if groups_left > 0:
                states.append((i, groups_left, '#'))
        i += 1

    if groups_left == 0:
        return 1
    return 0

sum = 0
for line in sys.stdin:
    line = line.rstrip()
    cond, groups = line.split()
    length = len(cond)
    groups = [int(g) for g in groups.split(',')]
    assert all([g > 0 for g in groups])
    num_groups = len(groups)

    arrangements = 0
    states = [(0, num_groups, None)]
    while states:
        arrangements += do_state(states.pop(0))
    # print(arrangements)
    sum += arrangements
print(sum)
