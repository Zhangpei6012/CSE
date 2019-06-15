'''
For drawing figures
Author: Tianpeng Chen
Note: This program depends on Matplotlib, so it may not be able to run on CSE machines
'''

import matplotlib.pyplot as plt
from Simulation import *
from numpy import exp, random
import xlwt

def draw_inter_arrivals(inter_arrival_list):
    fig, ax = plt.subplots()
    n, bins, patchs =  plt.hist(inter_arrival_list, bins=50)
    y = 0.35 * exp(-0.35 * bins) * 2000
    ax.plot(bins, y, '--')
    ax.set_xlabel('Inter-Arrival Time, lambda=0.35')
    ax.set_title('Histogram of 3500 exponentially distributed inter-arrival times')
    fig.tight_layout()
    plt.show()


def draw_service_times(services):
    fig, ax = plt.subplots()
    n, bins, patchs = plt.hist(services, bins=50)
    y = 1 * exp(-1 * bins) * 7200
    ax.plot(bins, y, '--')
    ax.set_xlabel('Service Time (Exponentially Generated), mu=1')
    ax.set_title('35000 exponentially distributed numbers (for calculating service time)')
    fig.tight_layout()
    plt.show()


def draw_real_service_times(services):
    fig, ax = plt.subplots()
    n, bins, patchs = plt.hist(services, bins=100)
    mu = 1
    y = (-mu**2 * bins**2) / 2 * exp(-mu*bins) - mu*bins*exp(-mu*bins) + 1 - exp(-mu * bins)
    ax.plot(bins, y, '--')
    ax.set_xlabel('Service Time (Calculated')
    ax.set_title('Histogram of 3500 service times in phase-type distribution')
    fig.tight_layout()
    plt.show()
    # plt.hist(services, bins=100)
    # plt.xlabel('Service Time (Calculated)')
    # plt.title('Histogram of 3500 service times in phase-type distribution')
    # plt.show()


def draw_mrt(mrts):
    plt.plot(mrts)
    plt.ylabel('Mean Response Time')
    plt.show()

def testXlwt(file = 'data.xls'):
    #############

    arrival = [5.72]
    service =[0.05,0.3,0.74]
    network = [1.2,1.47]
    #fogTimeLimit = 0.2
    fogTimeToCloudeTime = 0.6
    time_end = 10000
    ##########
    book = xlwt.Workbook()
    sheet1 = book.add_sheet('hello')
    i = 0.05  
    j = 0
    row = 0 
    clo = 0
    sheet1.write(0,0,'seed')
    sheet1.write(0,1,'FOGTIMELIMIT')
    sheet1.write(0,2,'MRT')
    row = 1
    clo =1 
    while i <= 0.3:
        SEED = 0 
        while SEED < 15:
        #simulation(mode, arrival, service, fogTimeLimit, fogTimeToCloudeTime, network, time_end, SEED):
            res, mrt = simulation('random', arrival, service, i, fogTimeToCloudeTime, network, time_end, SEED)
            sheet1.write(row,0,SEED)
            sheet1.write(row,1,i)
            sheet1.write(row,2,mrt)
            SEED += 1
            row += 1
        i += 0.01
        
    book.save(file)

def main():
    
    #res = simulate(0, 'random', 0.35, 1, 5, 5, 10, 10000, output=False, seed=100)
    #random.seed(0)
    #draw_inter_arrivals(res[1])
    #draw_service_times(res[2])
    #draw_real_service_times(res[3])
    mrts = []
    arrival = [5.72]
    service =[0.05,0.3,0.74]
    network = [1.2,1.47]
    #fogTimeLimit = 0.2
    fogTimeToCloudeTime = 0.6
    time_end = 10000
    i = 0.05 
    while i <= 0.3:
        SEED = 1

        #simulation(mode, arrival, service, fogTimeLimit, fogTimeToCloudeTime, network, time_end, SEED):
        res, mrt = simulation('random', arrival, service, i, fogTimeToCloudeTime, network, time_end, SEED)
        i += 0.01
        mrts.append(mrt)
    draw_mrt(mrts)
    #print(sum(res[1]) / len(res[1]), 1/0.35)
    #print(sum(res[2]) / len(res[2]), 1/1)
    
    #testXlwt()

if __name__ == '__main__':
    main()
