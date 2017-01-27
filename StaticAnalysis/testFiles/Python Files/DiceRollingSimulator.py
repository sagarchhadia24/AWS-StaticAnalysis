import random

def dice1():
    print random.randint(1,6)


dice1()
while True:
    input = raw_input("Would you like to roll again?[y/n]")
    if input=="y":
        dice1()
    else :
        break
