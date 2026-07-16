import time
from functools import cache

def line():
    print("-------------------------------------------------------")

@cache
def factorial(n):
    if n == 0:
        return 1
    else:
        return n * factorial(n-1)

def factorial_no_cache(n):
    if n == 0:
        return 1
    else:
        return n * factorial_no_cache(n-1)

@cache
def fibonacci_r(n):
    if n <= 1:
        return n
    else:
        return fibonacci_r(n-1) + fibonacci_r(n-2)

@cache
def fibonacci(n):
    a, b = 0, 1
    for _ in range(n):
        a, b = b, a + b
    return a

def benchmark(func, label, base_value=100, runs=5):
    for i in range(runs):
        n = base_value + (i * 10)
        start_time = time.time()
        func(n)
        end_time = time.time()
        elapsed = end_time - start_time
        print(f"{label} ({n}) Execution time: {elapsed:.6f} seconds")
    line()

def single_run_benchmark(func, label, n=100):
    start_time = time.time()
    func(n)
    end_time = time.time()
    elapsed = end_time - start_time
    print(f"{label} ({n}) Single run execution time: {elapsed:.6f} seconds")
    line()

line()
single_run_benchmark(factorial, "Cached Factorial")
single_run_benchmark(factorial_no_cache, "Non-Cached Factorial")

line()
benchmark(factorial, "Cached Factorial")
benchmark(factorial_no_cache, "Non-Cached Factorial")
benchmark(fibonacci_r, "Cached Recursive Fibonacci")
benchmark(fibonacci, "Cached Iterative Fibonacci")