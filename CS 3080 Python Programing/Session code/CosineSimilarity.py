import spacy

class WordCosineSim:
    def __init__(self):
        self.nlp = spacy.load('en_core_web_md')

    def sim(self, word1, word2):
        self.token1 = self.nlp(word1)
        self.token2 = self.nlp(word2)
        print(f"Cosine similarity between '{self.token1.text}' and '{self.token2.text}': {self.token1.similarity(self.token2)}")

WCS = WordCosineSim()

words = input('To exit the program enter: exit\n Else, enter two words separated by space for showing the cosine similarity value between them:\n')

while words != 'exit':
    words = words.split()
    if len(words) > 1:
        WCS.sim(words[0], words[1])
    else:
        print("Please enter two words separated by space.")
    words = input('To exit the program enter: exit\n Else, enter two words separated by space for showing the cosine similarity value between them:\n')
