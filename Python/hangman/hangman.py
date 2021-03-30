# Spencer Smith
# SDEV 220
# hangman
# Due 5/11/2018

from tkinter import *
import random

class CanvasShapes:
    def __init__(self, guesses):
        self.guesses = guesses
        #Creates the window and specifies size
        window = Tk()
        window.title("Hangman")
        self.canvas = Canvas(window, width = 400, height = 400, bg = "white")
        self.canvas.pack()
        self.frame1 = Frame(window)
        self.frame1.pack()
        

        #Initilizes variables
        self.complete = True
        self.incorrectGuesses = ""
        self.v1 = StringVar()
        self.failAttempts = 0
        self.wordIndex = -1
        self.currentWord = guesses[0].lower()
        self.guessChar = StringVar()
        self.guessWord = ""

        #Writes the rules and functions of the buttons and text
        self.entryGuess = Entry(self.frame1, textvariable = self.guessChar, width = 1)
        bCommitGuess = Button(self.frame1, text = "Guess!", command = self.commit)
        bNewGame = Button(self.frame1, text = "New Game", command = self.newGame)

        #Creates the buttons 
        self.entryGuess.grid(row = 2, column = 2)
        bCommitGuess.grid(row = 2, column = 3)
        bNewGame.grid(row = 2, column = 4)
        
        #runs initial functions
        self.displayPole()
        self.displayMan()
        self.drawWord()
        self.newGame()

        window.mainloop()

    #Displays a static pole for man to hang on
    def displayPole(self):
        self.canvas.create_arc(10,350, 100, 400, start  = "0", extent = "180")
        self.canvas.create_line(55,50,55,350)
        self.canvas.create_line(55,50,225,50)
        self.canvas.create_line(225,50,225,100)

    #Depending on failed attempts draws man with correct appendages
    def displayMan(self):
        if self.failAttempts >= 1:
            self.canvas.create_oval(200,100,250,150, tag = "man")
            if self.failAttempts >= 2:
                self.canvas.create_line(205,140,150,200, tag = "man")
                if self.failAttempts >= 3:
                    self.canvas.create_line(245,140,300,200, tag = "man")
                    if self.failAttempts >= 4:
                        self.canvas.create_line(225,150,225,230, tag = "man")
                        if self.failAttempts >= 5:
                            self.canvas.create_line(225,230,205,300, tag = "man")
                            if self.failAttempts >= 6:
                                self.canvas.create_line(225,230,245,300, tag = "man")

    #Displays winning text
    def displayWin(self):
        self.canvas.create_text(200,350, text = "You Won!", tag = "man")

    #Displays lost text
    def displayLost(self):
        self.canvas.create_text(200,350, text = "You Lost", tag = "man")

    #Displays the incorrect guesses
    def displayGuessBank(self, displayText):
        self.canvas.create_text(200,375, text = "Incorrect guesses: " + displayText, tag = "bank")

    #Clears the man and incorrect guess bank 
    def clearCanvas(self):
        self.canvas.delete("man", "bank")

    #Creates the text with approriate number of *
    def newGuessWord(self):
        self.guessWord = ""
        for i in range(len(self.currentWord)):
            self.guessWord = str(self.guessWord) + '*'

        self.drawWord()
        return self.guessWord

    #Passes the first character of input if not empty
    def commit(self):
        guessString = str(self.guessChar.get())
        if(guessString):
            self.entryGuess.delete(0, 'end')
            self.checkCorrect(guessString[0])

    #Bulk of the code, controls what happens to the guessed character
    def checkCorrect(self, evalChar):
        #If game isn't running, does nothing
        if(self.complete == False):
            #No use for case sensitivity
            evalChar = evalChar.lower()
            #Tests if the char is in the correct word
            if(self.currentWord.find(evalChar) != -1):
                #Finds how many times the character is in the word and replaces the strings at appropiate indexes
                for i in range(self.currentWord.count(evalChar)):
                    index = self.currentWord.find(evalChar)
                    self.guessWord = self.guessWord[:index] + evalChar + self.guessWord[index + 1:]
                    self.currentWord = self.currentWord[:index] + ' ' + self.currentWord[index + 1:]
                self.drawWord()
                #If all characters have been replaces, win condition is met
                if(self.guessWord.find('*') == -1):
                    self.complete = True
                    self.displayWin()
            else:
                #Does nothing if it finds the character in the incorrect guesses or answer so far
                if(self.incorrectGuesses.find(evalChar) == -1 and self.guessWord.find(evalChar) == -1):
                    self.canvas.delete("bank")
                    self.incorrectGuesses = self.incorrectGuesses + evalChar
                    self.displayGuessBank(self.incorrectGuesses)
                    self.failAttempts+=1
                    self.displayMan()
                    #Loss condition is met
                    if self.failAttempts == 6:
                        for i in self.currentWord:
                            if i != " ":
                                index = self.currentWord.find(i)
                                self.guessWord = self.guessWord[:index] + i + self.guessWord[index + 1:]
                                self.currentWord = self.currentWord[:index] + ' ' + self.currentWord[index + 1:]
                        self.drawWord()
                        self.complete = True
                        self.displayLost()

    #Draws the correct guesses so far
    def drawWord(self):
        self.labelWord = Label(self.frame1, text = self.guessWord)
        self.labelWord.grid(row = 2, column = 1)

    #Reinitilized most values, finds next word
    def newGame(self):
        self.wordIndex += 1
        self.incorrectGuesses = ""
        self.failAttempts = 0
        self.clearCanvas()
        self.currentWord = self.guesses[self.wordIndex]
        self.currentWord = self.currentWord.lower()
        self.guessWord = self.newGuessWord()
        self.complete = False

import os.path

#Returns a scrambled list of word from wordBank.txt
def WordsFind():    
    filename = "wordBank.txt"
    totalLines = 0
    totalWords = 0
    wordList = []

    #Checks to see if file exists
    if os.path.isfile(filename):
        
        infile = open(filename, "r")
        for line in infile:
            totalLines += 1

            words = line.split()
            for word in words:
                totalWords += 1
                wordList.append(word)


        random.shuffle(wordList)
        return wordList
    else:
        print("Error: File not found.")

def main():
    wordList = WordsFind()
    #Unnote this for a cheat sheat
    #print(wordList)
    CanvasShapes(wordList)

main()