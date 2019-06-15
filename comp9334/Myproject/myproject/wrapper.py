# encoding: utf-8
from Simulation import *
import numpy as np

SEED = 0 
#set SEED
num_test=open('num_tests.txt','r')
number=num_test.read()
for text_index in range(1,int(number)+1):
    mode=open('mode_{}.txt'.format(text_index),'r').read()
    if mode=='trace':
        arrival = open('arrival_{}.txt'.format(text_index), 'r').read()
        service = open('service_{}.txt'.format(text_index), 'r').read()
        network = open('network_{}.txt'.format(text_index), 'r').read()
        para = open('para_{}.txt'.format(text_index), 'r').read()
        para_list = para.split('\n')
        arrival_list=np.matrix(arrival)
        arrival_list=arrival_list.tolist()
        arrival_list=arrival_list[0]

        network_list = np.matrix(network)
        network_list=network_list.tolist()
        network_list=network_list[0]

        service_list = np.matrix(service)
        service_list=service_list.tolist()
        service_list=service_list[0]
        # print(mode,arrival_list,service_list,para_list[0],para_list[1],para_list[2])
        result,mrt=simulation(mode,arrival_list,service_list,float(para_list[0]),float(para_list[1]),network_list,float('inf'),SEED)
        with open('mrt_{}.txt'.format(text_index),'w') as f:
            f.write("%.4f"%mrt)
        with open('fog_dep_{}.txt'.format(text_index),'w') as f:
            for i in range(len(result[0])):
                    f.write(format(result[0][i][0],'.4f') + '	' + format(result[0][i][1],'.4f')+ '\n')
        with open('net_dep_{}.txt'.format(text_index),'w') as f:
            for i in range(len(result[1])):
                    f.write(format(result[1][i][0],'.4f') + '	' + format(result[1][i][1],'.4f')+ '\n')
        with open('cloud_dep_{}.txt'.format(text_index),'w') as f:
            for i in range(len(result[2])):
                    f.write(format(result[2][i][0],'.4f') + '	' + format(result[2][i][1],'.4f')+ '\n')

    if mode =='random':
        arrival=open('arrival_{}.txt'.format(text_index),'r').read()
        service=open('service_{}.txt'.format(text_index),'r').read()
        network = open('network_{}.txt'.format(text_index), 'r').read()

        para = open('para_{}.txt'.format(text_index), 'r').read()
        para_list = para.split('\n')
        arrival_list = arrival.split('\n')
        service_list = service.split('\n')
        network_list = network.split('\n')
        result,mrt=simulation(mode,arrival_list,service_list,float(para_list[0]),float(para_list[1]),network_list,float(para_list[2]),SEED)

        with open('mrt_{}.txt'.format(text_index),'w') as f:
            f.write("%.3f"%mrt)
        with open('fog_dep_{}.txt'.format(text_index),'w') as f:
            for i in range(len(result[0])):
                    f.write(format(result[0][i][0],'.4f') + '	' + format(result[0][i][1],'.4f')+ '\n')
        with open('net_dep_{}.txt'.format(text_index),'w') as f:
            for i in range(len(result[1])):
                    f.write(format(result[1][i][0],'.4f') + '	' + format(result[1][i][1],'.4f')+ '\n')
        with open('cloud_dep_{}.txt'.format(text_index),'w') as f:
            for i in range(len(result[2])):
                    f.write(format(result[2][i][0],'.4f') + '	' + format(result[2][i][1],'.4f')+ '\n')
