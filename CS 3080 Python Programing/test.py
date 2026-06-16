import numpy as np

#print example:
print("Hello from Zed!")

#array example:
arr = np.array([1,2,3,4,5])
arr_squared = arr ** 2
arr_doubled = arr * 2
#print results
print("Original array:", arr)
print("Squared Array",arr_squared)
print("Doubled array:",arr_doubled)
"""
This is a multi-line comment
it can span many lines and anything between tripple " will be ignored
commonly used for preamble
"""

#function call
def greet(name):
    print(f"Hello {name}!")


greet("Abe")
# scripting vs module protection
x=1 # vars are not declared but set when assigned like this
x = str(3)
y = int(3)
z = float(3)
print(type(z)) # to get the type of z

#string vars can be defined with "" or ''

#you can define mulitiple vars in one line
x, y, z = "Orange", "Banana", "Cherry"
# or you can define multiple vars with the same value
x = y = z = "Orange"


#tuples
def arithmetic(x, y):
    return x + y, x - y, x * y, x / y

a, s, m, d = arithmetic(10, 2)
print(a, s, m, d) # Output: 12 8 20 5.0


def main():
    print("this runs when the fiels is exeuted as a script.")


if __name__ == "__main__":
    main()
