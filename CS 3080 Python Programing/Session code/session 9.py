# argparse stuff
import argparse

def count_to_number(number, step):
    for i in range(1, number + 1, step):
        print(i)
parser = argparse.ArgumentParser()
parser.add_argument("--mode", choices=["count", "greet", "items"])
parser.add_argument("--number", type=int, default=10)
parser.add_argument("--step", type=int, default=1)
parser.add_argument("--name", type=str)
parser.add_argument("--times", type=int, default=1)
parser.add_argument("--shout", action="store_true")
parser.add_argument("--reverse", action="store_true")
parser.add_argument("--items", nargs="+")
parser.add_argument("--sorted", action="store_true")
args = parser.parse_args()

if args.mode == "count":
    count_to_number(args.number, args.step)

elif args.mode == "greet":
    if args.reverse:
        args.name = args.name[::-1]

    for _ in range(args.times):
        if args.shout:
            print(f"HELLO {args.name.upper()}!")
        else:
            print(f"Hello {args.name}!")

elif args.mode == "items":
    if args.sorted:
        args.items.sort()
    for item in args.items:
        print(f"Item: {item}")