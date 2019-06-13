import sys
import time
import socket
import threading
import struct
import random

f = open('2012.pdf', "rb" )
filedata = f.read(300)
i = 0
print('start')
while filedata != b'':
    print(f'{i}  :  {filedata}\n')
    i += 1 
    filedata = f.read(300)
print('end')
f.close()