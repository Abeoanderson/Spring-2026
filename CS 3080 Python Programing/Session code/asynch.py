import asyncio
from unicodedata import name

from numpy import rint
async def countdown(name, delay):
    while delay > 0:
        print(f"{name}: {delay} seconds remaining")
        await asyncio.sleep(1)
        delay -= 1
    print(f"{name} completed!")

async def main():
    await asyncio.gather(
    countdown("Timer A", 3),
    countdown("Timer B", 5),
    countdown("Timer C", 2)
)
    
await main()