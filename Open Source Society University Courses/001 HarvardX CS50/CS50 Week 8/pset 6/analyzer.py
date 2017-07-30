import nltk

class Analyzer():
    """Implements sentiment analysis."""
    
    # creates a list to store the positive and negative words.
    positiveWords = []
    negativeWords = []

    def __init__(self, positives, negatives):
        """Initialize Analyzer."""
        
        # Initializes the directories of positive and negative to self.
        self.positives = positives
        self.negatives = negatives
        
        # Opens the file containing lositive words and reads it line
        # by line. If it's a newline or starts with ;, skip it.
        # Otherwise, add it to the list of positive words. When 
        # added, it also removes the last 2 characters, '\n'.
        with open(self.positives, 'r') as posFile:
            for line in posFile:
                if line.startswith(";") or line.startswith("\n"):
                    continue
                else:
                    self.positiveWords.append(line[:-2])
                    
        # Does the exact same as above but for negative words.
        with open(self.negatives, 'r') as negFile:
            for line in negFile:
                if line.startswith(";") or line.startswith("\n"):
                    continue
                else:
                    self.negativeWords.append(line[:-2])
        
        
        

    def analyze(self, text):
        """Analyze text for sentiment, returning its score."""
        
        # Initializes self.text to the text the user input/tweet.
        self.text = text
        
        # Tokenizes (creates a list of) the words in the input text.
        self.wordsInText = nltk.word_tokenize(self.text)
        
        # Dictionary to keep track of each word and whether it's
        # positive (In which case, will have a value of 1), negative
        # (value of -1), or neutral (value of 0)
        self.wordWithScore = {}
        
        # Variable to  add the sum of the value of each word, thus
        # beng the total score for each word in text.
        self.textScore = 0
        
        # Itterate through each word in text and, if it's in the list
        # of positive words, add it to the dictionary with a value of
        # 1; if negative, with a value of -1; if in neither, with a
        # value of 0.
        for word in self.wordsInText:
            if word.lower() in self.positiveWords:
                self.wordWithScore[word] = 1
            
            elif word.lower() in self.negativeWords:
                self.wordWithScore[word] = -1
            
            else:
                self.wordWithScore[word] = 0
        
        # Itterate through each word:score pair in the dictionary and
        # add up the total of all the scores inside.
        for word, score in self.wordWithScore.items():
            self.textScore += score
        
        # Returns the total of the score of all words in text.
        return self.textScore
