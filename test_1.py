import random
import threading
import time

factors = {}
lock = threading.Lock()
def randNum():
    listValues = []
    for i in range(1, 100000):
        value = random.randrange(1, 10000)
        listValues.append(value)
    return listValues

#this function will compute the the factors of a number
def factor(numb):
    fact = []
    #only need to go to half the value of the number
    for i in range(1, int(numb/2 + 1)):
        if numb % i is 0:
            #if is a factor append it to the factors list
            fact.append(i)
    #return that list of factors for that number
    return fact

#this is going to be the class to help us create threads in
class myThread:
    #what is going to be in our thread
    def __init__(self, listValues, start, end):
        self.listValues = listValues
        self.start = start
        self.end = end
        thread = threading.Thread(target = self.getFactors, args = ())
        thread.start()

    def getFactors(self):
        for i in range(self.start, self.end):
            if i not in self.listValues:
                print("inside: ", factors)
                lock.acquire()
                factors[i] = factor(self.listValues[i])
                lock.release()

def newThread(listOfvalues):
    index = 0
    while index < len(listOfvalues):
        myThread(listOfvalues, index, index + 200)
        index += 200

newThread(randNum())
