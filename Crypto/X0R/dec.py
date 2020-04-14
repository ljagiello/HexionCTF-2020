from random import choice, randint
from string import ascii_letters
from itertools import cycle
from itertools import combinations

with open("flag.enc", "r") as file:
    flag = file.read()

max_length = 2
for length in xrange(2, max_length + 1):
    for word in map(''.join, combinations(ascii_letters, length)):
        key = 'JtmZzCJ' + word
        key_gen = cycle(key)
        data = []
        for i in range(len(flag)):
            code = int(ord(flag[i]) ^ ord(next(key_gen)))
            if code == 32:
                break
            if code == 64:
                break
            if code == 91:
                break
            if code == 92:
                break
            if code == 93:
                break
            if code == 94:
                break
            if code == 95:
                break
            if code == 96:
                break
            if code == 124:
                break
            if code == 126:
                break
            if code == 127:
                break
            if code == 125 and i != 41:
                break
            if code == 123 and i != 6:
                break
            data.append(chr(code))
        if len(data) == 42:
            if data[41] == "}":
                print ''.join(data)
