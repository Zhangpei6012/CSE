import random
import math
from numpy import random
from queue import Queue

SEED = 1
i = 0 
random.seed(SEED)
while i<20:
    i+=1

    print(random.exponential(1/5))