data = []

n = int(raw_input('Enter how many elements you want: '))
for i in range(0, n):
    x = raw_input('Enter the numbers into the array: ')
    data.append(x)
   
print "Your String is: " + ''.join(data)
