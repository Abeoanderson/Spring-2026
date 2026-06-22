"""define a function called key_exists that receives two parameters, d and key (same order). And then
return key in d.
"""
def key_exists(d, key):
    return key in d

print(key_exists({"a": 1, "b": 2}, "a")) # True
print(key_exists({"a": 1, "b": 2}, "c")) # False