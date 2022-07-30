#Diffie-Hellman key exchange
#Ashley Foley 19178703

import random
import os
import time
from datetime import datetime, timedelta
from sys import maxint

#this function checks if a number n is prime and returns true or false
def isPrime(n) :
 
    # Outlier cases
    if n <= 1:
        return False
    if n <= 3:
        return True
 
    # This is checked so we can skip these below
    if (n % 2 == 0 or n % 3 == 0):
        return False
 
    i = 5
    while(i * i <= n):
        if n % i == 0:
            return False
        if n % (i + 2) == 0:
            return False
        i = i + 6
 
    return True

if __name__ == '__main__':
    #Generate random prime = P (We are generating primes under 1000 for this project)
    random.seed(datetime.now())
    testNum = 4
    while not isPrime(testNum):
        testNum = random.randrange(1000)
    P = testNum


    #Generate primitive root modulo P = g
    g = random.randrange(P)
    flag = False
    required_set = set(range(1, P))
    while not flag:
        actual_set = set(pow(g, powers, P) for powers in range (1, P))
        if required_set == actual_set:
            flag = True
        else:
            g = random.randrange(P)


    #fork children
    #read and write for os.pipe()
    read, write = os.pipe()
    #create the second set for two way communication
    read2, write2 = os.pipe()
    #create first child to be alice
    pid1 = os.fork()
    pid2 = -1
    if pid1:
        #parents creates a second child to be bob
        pid2 = os.fork()


    if pid1 == 0:
        #generate private key = p_key
        random.seed(datetime.now() + timedelta(days=os.getpid()))
        p_key = random.randint(2, P -1)
        while p_key < 2 and p_key > P-1:
            p_key = random.randint(2, P -1)
        
    
        #close the unused write2 and write the message to the os.pipe
        os.close(write2)
        w = os.fdopen(write, 'w')

        #send g**p_key mod P to other child = sent_value
        w.write(str(g**p_key))
        w.close()
        os.close(read)
        r = os.fdopen(read2)

        #receive key = r_value
        r_value = r.read()


        #Calculate r_value**p_key mod P
        shared_key = int(r_value)**p_key % P

        #print the shared key
        print("Shared key Alice: ")
        print(shared_key)

    elif pid2 == 0:
        #generate private key = p_key
        random.seed(datetime.now() + timedelta(days=os.getpid()))
        p_key = random.randint(2, P -1)
        while p_key < 2 and p_key > P-1:
            p_key = random.randint(2, P -1)
     
        #close the unused write and write2 the message to the os.pipe
        os.close(write)
        w = os.fdopen(write2, 'w')

        #send g**p_key mod P to other child = sent_value
        w.write(str(g**p_key))
        w.close()
        os.close(read2)
        r = os.fdopen(read)

        #receive key = r_value
        r_value = r.read()


        #Calculate r_value**p_key mod P
        shared_key = int(r_value)**p_key % P

        #print the shared key
        print("Shared key Bob: ")
        print(shared_key)