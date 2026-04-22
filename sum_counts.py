import sys
import re

filename = sys.argv[1]

total = 0

with open(filename, "r") as f:
    for line in f:
        match = re.search(r"count=(\d+)", line)
        if match:
            total += int(match.group(1))

print("TOTAL COUNT:", total)