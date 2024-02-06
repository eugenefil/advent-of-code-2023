import sys
import re

mult = 5
if len(sys.argv) == 2:
    mult = int(sys.argv[1])

r = re.compile('[.?]+')
total = 0
for line_no, line in enumerate(sys.stdin):
    line = line.rstrip()
    cond, groups = line.split()
    cond = '?'.join([cond] * mult)
    length = len(cond)
    groups = ','.join([groups] * mult)
    groups = tuple(int(g) for g in groups.split(','))
    assert all(g > 0 for g in groups)
    unknown_left = cond.count('?')
    damaged_left = sum(groups) - cond.count('#')
    assert damaged_left >= 0
    assert unknown_left >= damaged_left

    arrangements = {}
    top_state = (0, groups, unknown_left, damaged_left, None)
    states = [top_state]
    while states:
        state = states.pop()
        i, groups_left, unknown_left, damaged_left, parent = state
        key = state[:-1]
        # print()
        # print((*key, parent[:-1] if parent else parent))
        # print(arrangements)
        if key in arrangements:
            num_arr = arrangements[key]
            if num_arr == 0:
                continue
            while parent:
                key = parent[:-1]
                arrangements[key] += num_arr
                parent = parent[-1]
            continue
        arrangements[key] = 0

        if damaged_left == 0:
            grps = tuple(len(g) for g in filter(None, re.split(r, cond[i:])))
            # print('final', end=' ')
            if grps == groups_left:
                arrangements[key] = 1
                while parent:
                    key = parent[:-1]
                    arrangements[key] += 1
                    parent = parent[-1]
                # print(1, end='')
            # print()
            continue
        
        assert groups_left
        groups_len = sum(groups_left) + len(groups_left) - 1
        g = groups_left[0]
        while True:
            while i < length and cond[i] == '.':
                i += 1
            # should not be possible - we should have catched
            # unknown_left < damaged_left earlier
            assert i < length

            # check there is enough place for remaining groups incl this
            if i + groups_len > length:
                break

            dmg = 0
            unk = 0
            j = i
            while j < i + g and cond[j] in ('#', '?'):
                if cond[j] == '#':
                    dmg += 1
                elif cond[j] == '?':
                    unk += 1
                j += 1
            if j == i + g and (j == length or cond[j] != '#'):
                n = 0
                # check we don't write off more than needed like in
                # e.g. "???# 2" - here first ?? is not an alternative, b/c
                # then we write off 2 damages but we only need 1
                if damaged_left - unk >= 0:
                    unk_trail = int(j < length and cond[j] == '?')
                    assert unknown_left - unk - unk_trail >= 0
                    if unknown_left - unk - unk_trail < damaged_left - unk:
                        break
                    states.append((j + 1 if j < length else length,
                        groups_left[1:],
                        unknown_left - unk - unk_trail,
                        damaged_left - unk,
                        state))
                    n += 1

                if cond[i] == '#': # no alternatives after #
                    break
                if n > 0 and j < length and cond[j] == '.':
                    if dmg > 0:
                        break
                    unknown_left -= unk
                    i = j + 1
                else:
                    unknown_left -= 1
                    i += 1
            elif j < i + g:
                # group was prematurely cut by .
                if dmg > 0:
                    break
                unknown_left -= unk
                i = j + 1
            else:
                if cond[i] == '#':
                    break
                unknown_left -= 1
                i += 1

            assert unknown_left >= 0
            if unknown_left < damaged_left:
                break
        # print('states:', [(*s[:-1], s[-1][:-1]) for s in states])

    key = top_state[:-1]
    print(line_no, cond, ','.join(str(g) for g in groups), arrangements[key])
    total += arrangements[key]
print(total)
