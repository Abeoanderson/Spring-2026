
#Q1
print("Question 1: ")
def greet(name):
    print(f"Hello, {name}!")

greet("Alice")

#Q2
print("Question 2: ")
def add(x, y):
    return x + y   

print(add(5,2))

#Q3
print("Question 3: ")
def power(base, exponent=2):
    return base ** exponent

print(power(5))
print(power(2, 3))

#Q4
print("Question 4: ")
def min_max_avg(numbers):
    minimum = min(numbers)
    maximum = max(numbers)
    average = sum(numbers) / len(numbers)
    return minimum, maximum, average 

nums = [2, 5, 7, 1, 9]
min_val, max_val, avg_val = min_max_avg(nums)
print(f"Min: {min_val}, Max: {max_val}, Avg: {avg_val:.2f}")

#Q
print("Question 5: ")
def sum_all(*args):
    return sum(args)    

print(sum_all(1, 2, 3))
print(sum_all(4, 5, 6, 7, 8))

#Q6
print("Question 6: ")
def print_info(**kwargs):
    for key, value in kwargs.items():
        print(f"{key}: {value}")

print_info(name="Alice", age=30, city="New York")
