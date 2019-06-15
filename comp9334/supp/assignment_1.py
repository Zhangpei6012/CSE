#Run by Python3

import numpy as np
from numpy.linalg import solve

x = 1/600 #Machine failed rate
y= 1/60   #Leader repair rate
z= 1/90   #Trainee repair rate
A = np.mat([[-4*x,y,0,z,0,0],[0,y+3*x,-1*z,0,0,0],[0,-3*x,2*x+y+z,-3*x,-1*(y+z),0],[4*x,0,y,-3*x-z,0,0], [0,0,2*x,0,-(x+y+z),y+z], [1,1,1,1,1,1]])
b = np.mat([0,0,0,0,0,1]).T
r = solve(A,b)

#calculate for Q3_c
print (r)#r[0]=P(0,0,0)
         #r[1]=P(1,1,0)
         #r[2]=P(2,1,1)
         #r[3]=P(1,0,1)
         #r[4]=P(3,1,1)
         #r[5]=P(4,1,1)
print("\n")

#Calculate mean number of failed machines for Q3_e
N=r[3][0] + r[1][0] +2*r[2][0] + 3*r[4][0] + 4*r[5][0]
print("Here is mean number of failed machines:")
print(N)
print("\n")

#Calculate throughput for Q3_f
x_F = 4*x*r[0][0] + 3*x*(r[1][0]+r[3][0]) + 2*x*r[2][0] + x*r[4][0]
print("Here is system throughput:")
print (x_F)
print("\n")

#Calculate MTTR for Q3-f
R_avg = N/x_F
print("Here is MTTR:")
print(R_avg)