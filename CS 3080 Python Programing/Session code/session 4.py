#Question 1
from unicodedata import name


class BankAccount():
    def __init__(self, owner, balance=0.0):
        self.owner = owner
        self.balance = balance
    def deposit(self, amount):
        if amount > 0:
            self.balance += amount
            print(f"Deposited: ${amount:.2f}. New balance: ${self.balance:.2f}")
        else:
            print("Deposit amount must be positive.")
    def withdraw(self, amount):
        if amount > 0 and amount <= self.balance:
            self.balance -= amount
            print(f"Withdrew: ${amount:.2f}. New balance: ${self.balance:.2f}")
        else:
            print("Withdrawal amount must be positive and less than or equal to the balance.")
    def display(self):
        print(f"Account owner: {self.owner}")
        print(f"Account balance: ${self.balance:.2f}")
    def transfer(sender, receiver, amount):
        if sender.balance >= amount:
            sender.withdraw(amount)
            receiver.deposit(amount)
            print(f"Transferred ${amount:.2f} from {sender.owner} to {receiver.owner}")
        else:
            print("Transfer failed: insufficient funds.")
            print("Transfer amount must be positive and less than or equal to the sender's balance.")
#-----------------------------------------------------------------------------------------------------
#part 2
class Rectangle():
    def __init__(self, width, height):
        self.width = width
        self.height = height
    def area(self):
        return self.width * self.height
    def parimeter(self):
        return 2 * (self.width + self.height)
    def is_square(self):
        return self.width == self.height
    def compare_area(obj1, obj2):
        area1 = obj1.width * obj1.height
        area2 = obj2.width * obj2.height
        if area1 > area2:
            print("Rectangle 1 has a larger area.")
        elif area1 < area2:
            print("Rectangle 2 has a larger area.")
        else:
            print("Both rectangles have the same area.")

#-----------------------------------------------------------------------------------------------------
#main
def main():
    #usage for part 1:
    acc1 = BankAccount("Alice", 500)
    acc2 = BankAccount("Bob", 300)
    acc1.deposit(150)
    acc2.withdraw(50)
    BankAccount.transfer(acc1, acc2, 200)
    acc1.display()
    acc2.display()
    #usage for part 2;
    r1 = Rectangle(10, 20)
    r2 = Rectangle(15, 15)
    print(f"Rectangle 1 - Area: {r1.area()}, Square: {r1.is_square()}")
    print(f"Rectangle 2 - Area: {r2.area()}, Square: {r2.is_square()}")
    Rectangle.compare_area(r1, r2)


if __name__ == "__main__":
    main()
