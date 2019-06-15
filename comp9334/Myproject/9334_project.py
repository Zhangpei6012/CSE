import random
import math
import numpy as np
from queue import Queue



class Server:
    def __init__(self):
        self.ctime = 0
        self.nextarrival = 0
        self.nextdepart = float('inf')
        self.running = []
    
    def compute_nextdepart(self):
        mindepart = float('inf')
        if len(self.running) == 0:#没有任务了
            self.nextdepart = float('inf')
        elif len(self.running) == 1:#只有一个任务
            self.nextdepart = self.ctime + self.running[0][1]#最小的总在上面
        else:   # 多个任务找出最小
            for i in range(len(self.running)):
                mindepart = min(len(self.running) * running[i][1], mindepart)
            self.nextdepart = mindepart + self.ctime
   
    def update_running(self,temp_time):
        if len(self.running) == 0:
            return
        else: 
            for i in range(len(self.running)):
                self.running[i][1] = self.running[i][1] - (self.ctime - temp_time)/len(self.running)
    
    def sort_running(self):   # 按照剩余时间排序      
        self.running = sorted(self.running, key = lambda x : x[1]) # 按照运行时间顺序排序



def simulation(mode, arrival, service, fogTimeLimit, fogTimeToCloudeTime, network, time_end):
    result =[] 

    depart_to_net = []
    depart_to_cloud = []
    left_time = []

    cloud_list= []
    service_time_cloud = []
    arrival_time_cloud = []

    currtime = 0
    index = 0

    if mode == 'trace':
        arrival_list = arrival
        service_list = service
    elif mode == 'random':
        next_arrival_time = -math.log(1 - random.random()) / arrival
        k1=-math.log(1 - random.random()) / service
        k2=-math.log(1 - random.random()) / service
        k3=-math.log(1 - random.random()) / service
        next_service_time =  k1+k2+k3
    ## 计算fog
    depart_to_net, left_time = compute(arrival_list, service_list, fogTimeLimit ,0,0,float('inf'))
    sorted(depart_to_net,key = lambda x: x[0]) # 以arrival时间排序
    sorted(left_time,key = lambda x: x[0])
    #####test 
    print('here is depart_to_net\n')
    print(depart_to_net)
    print('here is left time\n')
    print(left_time)

    ## 计算net
    for i in range(len(depart_to_net)):
        if network[i] != 0:
            time_depart_net = network[i] + depart_to_net[i][1] # 离开网络时间 = 到达网络时间 + 网络延迟
            depart_to_cloud.append([arrival_list[i], time_depart_net])
    ########test
    print('here is depart_to_could\n')
    print(depart_to_cloud)
    

    ## 计算cloud
    for i in range(len(left_time)):
            new_service = fogTimeToCloudeTime * (left_time[i][1])
            cloud_list.append([depart_to_cloud[i][1], new_service])
    
    sorted(cloud_list,key = lambda x: x[0])
    for i in range(len(cloud_list)):
        arrival_time_cloud.append(cloud_list[i][0])
        service_time_cloud.append(cloud_list[i][1])
    depart_from_cloud = []
    rebulish = []
    depart_from_cloud, rebulish = compute(arrival_time_cloud, service_time_cloud, float('inf'),0,0,float('inf'))
    sorted(depart_from_cloud,key = lambda x: x[0]) # 以arrival时间排序  
    ########test
    print('here is depart_from_could\n')
    print(depart_from_cloud)
    

def compute(arrival, service, fogTimeLimit, fogTimeToCloudeTime, network, time_end):

    time_left =[] #记录那个任务还没有结束，剩余多少时间
    result = []
    ###initial  
    mys = Server()
    mys.currentime = arrival[0]
    mys.nextarrival = arrival[1]
    mys.running.append([arrival[0], service[0]])
    del arrival[0]
    del service[0]
    mys.compute_nextdepart()

    while(len(arrival) != 0 or len(mys.running) != 0):
        if mys.ctime > time_end:
            break
        if mys.nextarrival > mys.nextdepart: # 没有任务进入服务器
            temp_time = mys.ctime
            mys.ctime = mys.nextdepart 

            result.append([mys.running[0][0], mys.nextdepart])#储存结果

            del mys.running[0] #弹出完成的任务
            mys.update_running(temp_time)
            mys.sort_running() ##排序
            mys.compute_nextdepart() # 计算个抵达时间
            
        elif mys.nextarrival < mys.nextdepart:
            temp_time = mys.ctime
            mys.ctime = mys.nextarrival  #改时间
            mys.update_running(temp_time)  #更新运算的程序
            #################放入running
            if service[0] <= fogTimeLimit:
                mys.running.append([arrival[0], service[0]])
            else:
                mys.running.append([arrival[0], fogTimeLimit])
                time_left.append([arrival[0], service[0] - fogTimeLimit])
            #################结束放入
            mys.sort_running() ##排序
            mys.compute_nextdepart()
            del arrival[0]
            del service[0]
            if len[arrival] == 0:
                mys.nextarrival = float('inf') ###没有后续任务了
            else:
                mys.nextarrival = arrival[0]


        elif mys.nextarrival == mys.nextdepart:
            temp_time = mys.ctime
            mys.ctime = mys.nextdepart
            
            #先弹出任务
            result.append([mys.running[0][0], mys.nextdepart])#储存结果
            del mys.running[0] #弹出完成的任务
            #再放入任务
            
            mys.update_running(temp_time)  #更新运算的程序
            #################放入running
            if service[0] <= fogTimeLimit:
                mys.running.append([arrival[0], service[0]])
            else:
                mys.running.append([arrival[0], fogTimeLimit])
                time_left.append([arrival[0], service[0] - fogTimeLimit])
            #################结束放入
            mys.sort_running() ##排序
            mys.compute_nextdepart()
            del arrival[0]
            del service[0]
            if len[arrival] == 0:
                mys.nextarrival = float('inf') ###没有后续任务了
            else:
                mys.nextarrival = arrival[0]
    return result, time_left


            


            
            
            



    
    

    


    
    