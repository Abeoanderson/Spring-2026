print("module_b: starting...")
from module_a import hello
print("module_b: calling hello() from module_a")
hello()
print("module_b: done.")