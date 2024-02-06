import sys

def HASH(s):
    h = 0
    for c in s:
        h += ord(c)
        h *= 17
        h %= 256
    return h

seq = sys.stdin.read().rstrip().split(',')
boxes = []
for _ in range(256):
    boxes.append({})
for op in seq:
    print(op, end=' ')
    if op[-1] == '-':
        label = op[:-1]
        box_idx = HASH(label)
        print(f'remove {label} from {box_idx}')
        boxes[box_idx].pop(label, None)
    elif op[-1].isdigit():
        label, flen = op.split('=')
        box_idx = HASH(label)
        print(f'add/replace {label} in {box_idx}')
        boxes[box_idx][label] = int(flen)
    else:
        assert 0
for box in boxes:
    if box:
        print(box)

total = 0
for b, box in enumerate(boxes):
    if box:
        print(f'box {b + 1}:', box, end=' ')
    for slot, flen in enumerate(box.values()):
        power = (b + 1) * (slot + 1) * flen
        print('power', power)
        total += power
print(total)
