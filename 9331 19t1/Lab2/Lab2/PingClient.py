import time 
#from datetime import *
from socket import *
import sys

Host = sys.argv[1]
serverPort = int(sys.argv[2])

timeout = 1
BUFSIZE = 1024 # catch size
Address = (Host,serverPort) 
clientsocket = socket(AF_INET,SOCK_DGRAM)
clientsocket.settimeout(timeout)
MAXRTT = 0 
MINRTT = float('inf')
AVGRTT = 0

for i in range(0,10):
    sendtime = time.time()
    message = ('PING %d %f \r\n'% (i+1, sendtime)).encode()
    try:
        clientsocket.sendto(message,Address) #send request
        message, ServerAddress = clientsocket.recvfrom(BUFSIZE) #get response
        RTT = (time.time() - sendtime) * 1000
        MAXRTT = max(RTT, MAXRTT)
        MINRTT = min(RTT, MINRTT)
        AVGRTT += RTT
        print('ping to %s, seq = %d, rtt = %.fms' % (Host, i+1, RTT))
    except Exception as e:
        print ('ping to %s, seq = %d, timeout'% (Host, i+1))

print ('Min_RTTs = %.2fms, Max_RTTs = %.2fms, Average_RTTs = %.2fms' % (MINRTT, MAXRTT, AVGRTT/10))
clientsocket.close()
