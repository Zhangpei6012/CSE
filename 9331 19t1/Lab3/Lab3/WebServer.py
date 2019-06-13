

#run by Python2
from socket import *
import re
import sys
import os

Host = '127.0.0.1' 
Port =  int(sys.argv[1]) #port number 

## 404 NOT FOUND header###
NOTFOUND= '''
HTTP/1.1 404 Not Found
Content-Type: text/html
'''


myserver = socket(AF_INET, SOCK_STREAM)
myserver.setsockopt(SOL_SOCKET, SO_REUSEADDR, 1)

myserver.bind((Host, Port))
myserver.listen(1)
print ("The server is ready to receive")
while True:
    client_connection, client_address = myserver.accept()
    request = client_connection.recv(2048)
    request = request.split(' ')
    request_method = request[0]
    request_fname = request[1]

    if request_method == 'GET':
        if request_fname == '/index.html':
            file = open('index.html','r')
            index_content = '''
HTTP/1.1 200 ok
Cotent-Type:text/html

'''
            index_content += file.read()
            file.close()
            content = index_content
        elif re.match(r'/(.*).html', request_fname):
            path = request_fname[1:]
            content = '''
HTTP/1.1 200 ok
Cotent-Type:text/html

'''
            try:
                file = open(path,'rb')
                content += file.read()
                file.close()
            except:
                content = NOTFOUND
        elif re.match(r'/(.*).png', request_fname):
            path = request_fname[1:]
            content = '''
HTTP/1.1 200 ok
Content-Type: image/png

'''
            try:
                file = open(path,'rb')
                content += file.read()
                file.close()
            except:
                content = NOTFOUND
        else:
            content = NOTFOUND
    client_connection.sendall(content)
    client_connection.close()