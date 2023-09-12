import os

neg = int(input("Neg count: "))
pos = int(input("Pos count: "))
tests = os.listdir("func_tests/data")

for i in range(1, neg + 1):
    if f"neg_{str(i).rjust(2, '0')}_in.txt" not in tests:
        os.system(f"touch func_tests/data/neg_{str(i).rjust(2, '0')}_in.txt")
        os.system(f"touch func_tests/data/neg_{str(i).rjust(2, '0')}_out.txt")

for i in range(1, pos + 1):
    if f"pos_{str(i).rjust(2, '0')}_in.txt" not in tests:
        os.system(f"touch func_tests/data/pos_{str(i).rjust(2, '0')}_in.txt")
        os.system(f"touch func_tests/data/pos_{str(i).rjust(2, '0')}_out.txt")

os.system(f"touch func_tests/readme.md") if "readme.md" not in os.listdir() else None