def hello():
    print("Hello from hello() in module_a!")

def main():
    print("module_a: inside main()")
    hello()

if __name__ == "__main__":
    main()