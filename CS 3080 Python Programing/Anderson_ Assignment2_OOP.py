class Book():
    total_books = 0
    def __init__(self, title, author, price):
        self.title = title
        self.author = author
        self.price = price
        Book.total_books += 1
        if Book.total_books >= 3:
            print(f"total number of books: {Book.total_books}")
            
    def get_summary(self):
        return f"Title: {self.title}, Author: {self.author}, Price: ${self.price:.2f}"
    
    def __str__(self):
        return f"Title: {self.title}, Author: {self.author}, Price: ${self.price:.2f}"
    def apply_discount(self, discount_percentage):
        discount_amount = self.price * (discount_percentage / 100)
        self.price -= discount_amount
    
    @classmethod
    def from_string(cls, book_str):
        title, author, price = book_str.split(",")
        return cls(title.strip(), author.strip(), float(price.strip()))
    
    @staticmethod
    def is_expensive(price):
        return price > 20
    
    
class Ebook(Book):
    def __init__(self, title, author, price, file_size):
        super().__init__(title, author, price)
        self.file_size = file_size
        
    def get_summary(self):
        return f"Title: {self.title}, Author: {self.author}, Price: ${self.price:.2f}, File Size: {self.file_size} MB"


def print_summary(obj):
    print(obj.get_summary())





# === TEST CASES ===
# Q1
print("Q1 test:")
book1 = Book("1984", "George Orwell", 15.0)
book2 = Book("Brave New World", "Aldous Huxley", 18.0)
book3 = Book("Fahrenheit 451", "Ray Bradbury", 12.0)
print(f"Total Books: {Book.total_books}")
# Q2
print("Q2 test:")
print(book1)
print(book1.get_summary())
# Q3
print("Q3 test:")
book_str = "The Alchemist, Paulo Coelho, 15.99"
book4 = Book.from_string(book_str)
print(book4)
# Q4
print("Q4 test:")
print("Is book expensive?", Book.is_expensive(book4.price))
# Q5
print("Q5 test:")
ebook1 = Ebook("Digital Fortress", "Dan Brown", 25.0, 5.2)
print(ebook1.get_summary())
# Q6
print("Q6 test:")
books = [book1, ebook1, book2]
for b in books:
    print_summary(b)
# Q7
print("Q7 test:")
book1.apply_discount(10)
print(f"Discounted Price of '{book1.title}': ${book1.price:.2f}")