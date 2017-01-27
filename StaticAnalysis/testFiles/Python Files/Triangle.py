input = raw_input("Enter Hight of the numerical triangle: ")

for i in range(1,int(input)):
    print i*(sum([10**x for x in range(i)]))
