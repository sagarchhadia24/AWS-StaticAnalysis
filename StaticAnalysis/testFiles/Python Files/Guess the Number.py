import random

class Number:
    def RandomFunction(self):
        self.random_number = random.randint(1,9)
        print self.random_number
        return self.random_number

    def CheckNumber(self):
        input = raw_input("Guess what that number is")
        if (int(input) == int(self.random_number)):
            print ("you have guess correct number")
        elif int(input) > int(self.random_number):
            print ("your guess is higher than generated number")
            self.CheckNumber()
        else:
            print ("your guess is lower than generated number")
            self.CheckNumber()

# Program starts from here
n = Number()
# Calling function to generate Random number only once
n.RandomFunction()
# Calling faunction to ask user input and compare with generated random number untill user guess correct number
n.CheckNumber()

