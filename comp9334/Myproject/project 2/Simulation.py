
import math
from numpy import random
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
                mindepart = min(len(self.running) * self.running[i][1], mindepart)
            self.nextdepart = mindepart + self.ctime
   
    def update_running(self,temp_time):
        if len(self.running) == 0:
            return

        else: 
            for i in range(len(self.running)):
                self.running[i][1] = self.running[i][1] - (self.ctime - temp_time)/len(self.running)
    
    def sort_running(self):   # 按照剩余时间排序      
        self.running = sorted(self.running, key = lambda x : x[1]) # 按照运行时间顺序排序



def simulation(mode, arrival, service, fogTimeLimit, fogTimeToCloudeTime, network, time_end, SEED):
    result =[] 

    depart_to_net = []
    depart_to_cloud = []
    left_time = []

    cloud_list= []
    service_time_cloud = []
    arrival_time_cloud = []

    depart_from_cloud = []
    rebulish = []
    
    dic_check ={}
    currtime = 0
    index = 0

    if mode == 'trace':
        arrival_list = arrival
        service_list = service
        num_jobs = len(arrival_list)
        sum_time = 0
        ## 计算fog
        depart_to_net, left_time,sum_time = compute(arrival_list, service_list, fogTimeLimit ,0,0,float('inf'))
        depart_to_net.sort(key = lambda x: x[0])
        left_time.sort(key = lambda x: x[0])

        ## 计算net
        for i in range(len(depart_to_net)):
            if network[i] != 0:
                time_depart_net = network[i] + depart_to_net[i][1] # 离开网络时间 = 到达网络时间 + 网络延迟

                depart_to_cloud.append([depart_to_net[i][0], time_depart_net])
        sum_time += sum(network)

        

        for e in depart_to_cloud:
            dic_check[e[1]] =e[0]

        for i in range(len(left_time)):
                new_service = fogTimeToCloudeTime * (left_time[i][1])
                cloud_list.append([depart_to_cloud[i][1], new_service])
        cloud_list.sort(key = lambda x: x[0])
        for i in range(len(cloud_list)):
            arrival_time_cloud.append(cloud_list[i][0])
            service_time_cloud.append(cloud_list[i][1])

        depart_from_cloud, rebulish , sum_time2= compute(arrival_time_cloud, service_time_cloud, float('inf'),0,0,float('inf'))
        
        for e in depart_from_cloud:
            e[0] = dic_check[e[0]]
        depart_from_cloud.sort(key = lambda x: x[0])

        result.append(depart_to_net)
        result.append(depart_to_cloud)
        result.append(depart_from_cloud)
        return result, (sum_time+sum_time2)/num_jobs
    elif mode == 'random':
        #generate_list(_lambda, a ,b,c,d,e,fogtimelimit,SEED, time_end):
    
        #print(arrival)
        #print(service)
        #print(network)
        #print(fogTimeLimit)
        #print(fogTimeToCloudeTime)
        #print(time_end)
        arrival_list,service_list,network,num_jobs= generate_list(float(arrival[0]),float(service[0]),float(service[1]),float(service[2]),float(network[0]),float(network[1]),float(fogTimeLimit),SEED,float(time_end))
        #num_jobs = len(arrival_list)
        sum_time = 0
        ## 计算fog
        depart_to_net, left_time,sum_time = compute(arrival_list, service_list, fogTimeLimit ,0,0,float('inf'))
        depart_to_net.sort(key = lambda x: x[0])
        left_time.sort(key = lambda x: x[0])


        ## 计算net
        for i in range(len(depart_to_net)):
            if network[i] != 0:

                time_depart_net = network[i] + depart_to_net[i][1] # 离开网络时间 = 到达网络时间 + 网络延迟

                depart_to_cloud.append([depart_to_net[i][0], time_depart_net])
        sum_time += sum(network)
        #print(depart_to_cloud)
        #print('上面是print(depart_to_cloud)')
        

        for e in depart_to_cloud:
            dic_check[e[1]] =e[0]

        for i in range(len(left_time)):
                new_service = fogTimeToCloudeTime * (left_time[i][1])
                cloud_list.append([depart_to_cloud[i][1], new_service])
        cloud_list.sort(key = lambda x: x[0])
        for i in range(len(cloud_list)):
            arrival_time_cloud.append(cloud_list[i][0])
            service_time_cloud.append(cloud_list[i][1])

        depart_from_cloud, rebulish , sum_time2= compute(arrival_time_cloud, service_time_cloud, float('inf'),0,0,float('inf'))
        
        for e in depart_from_cloud:
            e[0] = dic_check[e[0]]
        depart_from_cloud.sort(key = lambda x: x[0])

        result.append(depart_to_net)
        result.append(depart_to_cloud)
        result.append(depart_from_cloud)
        return result, (sum_time+sum_time2)/num_jobs       


def compute(arrival, service, fogTimeLimit, fogTimeToCloudeTime, network, time_end):
    sum_time = 0
    time_left =[] #记录那个任务还没有结束，剩余多少时间
    result = []
    ###initial  
    mys = Server()
    mys.ctime = arrival[0]
    mys.nextarrival = arrival[1]
    if service[0] > fogTimeLimit:
        mys.running.append([arrival[0], fogTimeLimit])
        time_left.append([arrival[0], service[0] - fogTimeLimit])
        mys.nextdepart = arrival[0] + fogTimeLimit
    else:
        mys.running.append([arrival[0], service[0]])
        mys.nextdepart = arrival[0] + service[0]

    del arrival[0]
    del service[0]
    
    #i = 0 
    while(len(arrival) != 0 or len(mys.running) != 0):
        if mys.ctime > time_end:
            break
        if mys.nextarrival > mys.nextdepart: # 没有任务进入服务器
            temp_time = mys.ctime
            mys.ctime = mys.nextdepart 

            result.append([mys.running[0][0], mys.nextdepart])#储存结果
            sum_time += (mys.nextdepart - mys.running[0][0])

            #del mys.running[0] #弹出完成的任务
            mys.update_running(temp_time)
            del mys.running[0] #弹出完成的任务
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
            if len(arrival) == 0:
                mys.nextarrival = float('inf') ###没有后续任务了
            else:
                mys.nextarrival = arrival[0]


        elif mys.nextarrival == mys.nextdepart:
            temp_time = mys.ctime
            mys.ctime = mys.nextdepart
            
            #先弹出任务
            result.append([mys.running[0][0], mys.nextdepart])#储存结果
            sum_time += (mys.nextdepart - mys.running[0][0])
            #del mys.running[0] #弹出完成的任务
            #再放入任务
            
            mys.update_running(temp_time)  #更新运算的程序

            #################放入running
            if service[0] <= fogTimeLimit:
                mys.running.append([arrival[0], service[0]])
            else:
                mys.running.append([arrival[0], fogTimeLimit])
                time_left.append([arrival[0], service[0] - fogTimeLimit])
            #################结束放入
            del mys.running[0] #弹出完成的任务
            mys.sort_running() ##排序
            mys.compute_nextdepart()
            del arrival[0]
            del service[0]
            if len[arrival] == 0:
                mys.nextarrival = float('inf') ###没有后续任务了
            else:
                mys.nextarrival = arrival[0]
            

    return result, time_left,sum_time

def get_service(a,b,c,SEED):
    random.seed(SEED)

    top = 1/(1.0 - c)
    bottom = random.random()*(pow(a,1.0-c) - pow(b,1.0-c))+ pow(a,1.0-c)
    
    return pow(bottom,top)

def generate_list(_lambda, a ,b,c,d,e,fogtimelimit,SEED, time_end):

    

    arrvial_list =[]
    service_list = []
    network_list = []
    i=0
    last_arr = 0 
    random.seed(SEED)
    while last_arr<= time_end:
        
        arrvial = last_arr + random.exponential(1/_lambda)
        arrvial_list.append(arrvial)
        i+=1
        
        top = 1/(1.0 - c)
        bottom = random.random()*(pow(b,1.0-c) - pow(a,1.0-c))+ pow(a,1.0-c)
        service = pow(bottom,top)
        service_list.append(service)
        if service > fogtimelimit:
            network_list.append(random.uniform(d, e))
        else:
            network_list.append(0)
        last_arr = arrvial


    return arrvial_list,service_list,network_list,i
            


            
if __name__ == '__main__':
    arrival1 = [1.000
                ,2.000
                ,4.000
                ,5.000
                ,6.000
]
    service1 = [3.700
                ,5.100
                ,1.300
                ,2.400
                ,4.500
]
    fogTimeLimit = 2.500
    fogTimeToCloudeTime = 0.700
    network =[1.500
,1.400
,0.000
,0.000
,1.600
]
 
    time_end = float('inf')
    simulation('trace',arrival1,service1,fogTimeLimit,fogTimeToCloudeTime,network,time_end)

    
    # analyse_tc_data('./test/tc_log_range_50_step_1_for_plot.txt') 



    
    

    


    
    