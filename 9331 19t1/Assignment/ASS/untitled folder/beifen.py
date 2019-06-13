import sys
import threading
import socket
import time
import struct
import random



HOST = "127.0.0.1"
PORTID = 65500 ######记得改回50000
PING_TIMEOUT = 1.0
FILE_TIMEOUT = 1.0
PING_GAP = 4.0 
UDP_BUFFER = 2048 
TCP_BUFFER = 1024 
ACK_CUMULATION_LIMIT = 5 




TOREAD = {0:'pingreq', 1:'pingres', 2:'filereq', 3:'fileres', 4:'lostpeer', 5:'quit'}
TOSEND = {'pingreq':0, 'pingres':1, 'filereq':2, 'fileres':3, 'lostpeer':4, 'quit':5}


class Peer:
	def __init__(self,PeerId,pred,succ1,succ2,MSS,droprate):
		self.id = PeerId
		self.port = PORTID + PeerId
		self.pred = pred  #pred = None
		self.succ1 = succ1
		self.succ2 = succ2
		self.MSS = MSS
		self.droprate = droprate
		self.starttime = time.time()


		# create threads
		#Listening to input from commend
		t0 = threading.Thread(target=self.Input)
		t0.start()

		#PING successors 
		t1 = threading.Thread(target = self.Ping, args = (1,))
		t2 = threading.Thread(target = self.Ping, args = (2,))
		t1.daemon = True
		t2.daemon = True
		t1.start()
		t2.start()

		#Listening to TCP
		t3 = threading.Thread(target=self.TCP)
		t3.daemon = True
		t3.start()

		#Listening to UDP
		t4 = threading.Thread(target=self.UDP)
		t4.daemon = True
		t4.start()



	

	# creates byte array message.
	def Create_message(self, TYPE, info1 = None, info2 = None, info3 = None, info4 = None):
		message = bytearray([TYPE])
		message.extend(struct.pack("B", self.id))
		if info1 != None:
			message.extend(struct.pack("B", info1))
		if info2 != None:
			message.extend(struct.pack("B", info2))
		if info3 != None:
			message.extend(struct.pack("B", info3))	
		if info4 != None:
			message.extend(struct.pack("B", info4))	
		return message

	def Peer_lost(self, succ_number):
		TCP_socket = socket.socket(socket.AF_INET,socket.SOCK_STREAM)
		if succ_number == 1: #1st suc lost
			self.succ1 = self.succ2
			TCP_socket.connect((HOST,PORTID+self.succ2))
		else:#2nd suc lost
			time.sleep(PING_GAP+1) 
			TCP_socket.connect((HOST,PORTID+self.succ1))
		message = self.Create_message(TOSEND['lostpeer'])
		TCP_socket.send(message)
		data = TCP_socket.recvfrom(TCP_BUFFER)
		self.succ2 = int(data[0])
		TCP_socket.close()
		if self.succ1 == self.id: #conly one peer in network
			print("I'm last peer on network, I have no successor")
			self.succ1 ,self.succ2= None
		else:
			print(f'My first successor is now peer {self.succ1}')
			print(f'My second successor is now peer {self.succ2}')


	def Ping(self,flag):
		current_seq = 0
		seq_num = 0
		while True:
			if flag == 1:
				targetID = self.succ1
			elif flag == 2:
				targetID = self.succ2
			if not targetID or targetID == self.id:
				return
			UDP_socket = socket.socket(socket.AF_INET,socket.SOCK_DGRAM)
			UDP_socket.settimeout(PING_TIMEOUT)
			message = self.Create_message(TOSEND['pingreq'],flag, seq_num)
			UDP_socket.sendto(message,(HOST,PORTID+targetID))
			try:
				data,addr = UDP_socket.recvfrom(UDP_BUFFER)
				if TOREAD[data[0]] == 'pingres':
					print(f"A ping response message was received from Peer {int(data[1])}")
					current_seq = seq_num
			except socket.timeout:
				if current_seq > seq_num:
					ack_cumulation =  255 - current_seq + seq_num + 1 
				else:
					ack_cumulation =  seq_num - current_seq
				if ack_cumulation:
					print(f"No ping response from Peer {targetID}, no ACK's accumulation = {ack_cumulation}, sequence gap = [ {current_seq} - {seq_num} ]")
				if ack_cumulation > ACK_CUMULATION_LIMIT: #peer is not alive
					print(f'Peer {targetID} is no longer alive.')
					self.Peer_lost(flag)
			seq_num = (seq_num+1)%256   
			time.sleep(PING_GAP)
	

	def UDP(self):
		Server_socket = socket.socket(socket.AF_INET,socket.SOCK_DGRAM)
		Server_socket.bind((HOST,self.port))
		while 1:
			data, addr = Server_socket.recvfrom(UDP_BUFFER)
			if not data: 
				break
			try:
				try:
					if TOREAD[data[0]] == 'pingreq':
						if data[2] == 1:
							self.pred = int(data[1])#sender ID is  int(data[1])
						print(f"A ping request message was received from Peer {int(data[1])}")
						message = self.Create_message(TOSEND['pingres'],data[3])	
						Server_socket.sendto(message,addr)
				except KeyError:
					message = struct.unpack('sl?l999s',data)
					seq = message[1]
					flag = message[2]
					if not flag:
						if seq == 1:
							received_file = open('received_file.pdf', "wb")
							rlog = open('receivered_log.txt', "w")
					
						lendata = message[3]
						filedata = message[4][:lendata]
						received_file.write(filedata)
						t_gap = time.time() - self.starttime
						rlog.write('{:<18}\t{:<8.2f}\t{:<10}\t{:<10}\t{:<10}\n' .format('rcv', t_gap ,seq,lendata, 0))
						ack = str(message[1] + lendata)
						rlog.write('{:<18}\t{:<8.2f}\t{:<10}\t{:<10}\t{:<10}\n' .format('snd', t_gap ,0 ,lendata, ack))
						Server_socket.sendto(ack.encode(),addr)
	
					else:
						print('The file is received.')
						received_file.close()
						rlog.close()

			except ConnectionRefusedError:
				print("Couldn't connect to peer, please wait until peers update and try again")


	#TCP messages  kill peer/ peer departure / file requet and response
	def TCP(self):
		Server_socket = socket.socket(socket.AF_INET,socket.SOCK_STREAM)
		Server_socket.bind((HOST,self.port))
		Server_socket.listen()
		while True:
			Connection_socket, addr = Server_socket.accept()
			data = Connection_socket.recv(TCP_BUFFER)
			try:
				if TOREAD[data[0]] == 'quit': # quit message received (update sucessors)
					if int(data[1]) == self.succ1:
						TCP_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
						if int(data[2]) == self.id:
							print("I am only one peer in network, I have no successors")
							self.succ1 ,self.succ2= None
						else: 
							self.succ1 = int(data[2])
							self.succ2 = int(data[3])
						TCP_socket.connect((HOST,PORTID+self.pred))
						TCP_socket.send(data)
						TCP_socket.close()
					elif int(data[1]) == self.succ2:
						self.succ2 = int(data[2])
					if self.succ1 and self.succ2:
						print(f'My first successor is now peer {self.succ1}')
						print(f'My second successor is now peer {self.succ2}')


				elif TOREAD[data[0]] == 'lostpeer': 
					message = str(self.succ1).encode('utf-8')
					Connection_socket.send(message)

				elif TOREAD[data[0]] == 'filereq' :
					TCP_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)

					if self.id == data[2] or \
					(self.pred < data[2] <= self.id) or \
					(self.succ1 < self.id and data[2] > self.id) or\
					(self.pred > self.id and data[2] < self.id) :
						print(f'File {data[3]*256 + data[2]} is here.')

						### file in my peer
						## reponse to requester TCP
						TCP_socket.connect((HOST, PORTID + data[1]))
						message = self.Create_message(TOSEND['fileres'],data[2], data[3]) ####file request model
						TCP_socket.send(message)
						print(f'A response message, destined for peer {data[1]}, has been sent.')
						TCP_socket.close()
						####transfer file 
						self.file_transfer(data[1],data[3]*256 + data[2])


					else: #forward request to the next peer
						print(f'File {data[3]*256 + data[2]} is not stored here.')
						print('File request has been forwarded to my sucessor')
						TCP_socket.connect((HOST,PORTID+self.succ1))
						TCP_socket.send(data)
						TCP_socket.close()

				elif TOREAD[data[0]] == 'fileres': #requester receive the file response
					print(f'Received a response message from peer {data[1]}, which has file {data[3]*256 + data[2]}')
					print('we now start receiving the file...')

				
			except ConnectionRefusedError:
				print("Couldn't connect to peer, please wait until peers update and try again")
			
			Connection_socket.close()


	# when input "request XXXX", initial the file transfer
	def file_transfer_inital(self,filenum):
		filehash = filenum % 256 ##取余数
		filevalue = filenum // 256 
		if not self.pred: 
			print("wait Ping to update Presuccessors")
		
		elif filehash == self.id or \
		self.succ1 == None or \
		(self.pred < filehash <= self.id) or \
		(self.succ1 < self.id and filehash > self.id) or \
		(self.pred > self.id and filehash < self.id):
			print(f"File {filenum} is found here in peer {self.id}")
			#file in requester peer

		else: # forward to successor
			try:

				### 256 * filevalue + filehash
				### 256 * date[3] + date[2]
				TCP_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
				TCP_socket.connect((HOST,PORTID+self.succ1))				
				message = self.Create_message(TOSEND['filereq'],filehash, filevalue)####file response model 'filereq'], filehash, filevalue, requester
				TCP_socket.send(message)
				TCP_socket.close()
				print(f'File {filenum} is not stored here.')
				print('File request has been forwarded to my sucessor')
			except ConnectionRefusedError:
				print("failure to connect to peer")
	
	def file_transfer(self,destnation,filenum):
		seq = 1 
		event = ''
		transfile_socket = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
		transfile_socket.settimeout(1)
		f = open(str(filenum) + '.pdf', "rb" )
		slog = open('sender_log.txt', "w")
		filedata = f.read(self.MSS)
		while filedata != b'':
			try:
				lendata = len(filedata)
				time_gap = time.time() - self.starttime
				if random.random() > self.droprate:
					if event == 'Drop' or event == 'RTX/Drop': 
						event = 'RTX'
					else:
						event = 'snd'
					message = struct.pack('sl?l999s',*(b'f',seq, 0, lendata, filedata))
					transfile_socket.sendto(message,(HOST,PORTID + destnation))
				else: 
					if event == 'Drop' or event == 'RTX/Drop': 
						event = 'RTX/Drop'
					else:
						event = 'Drop'
				slog.write('{:<18}\t{:<8.2f}\t{:<10}\t{:<10}\t{:<10}\n' .format(event, time_gap, seq, lendata, 0))
				ack = int(transfile_socket.recv(1024).decode('utf_8'))
				if ack == seq + lendata: #send next MSS
					ack_nb = ack
					seq += lendata
					time_gap = time.time() - self.starttime
					slog.write('{:<18}\t{:<8.2f}\t{:<10}\t{:<10}\t{:<10}\n' .format('rcv', time_gap,0,lendata,ack_nb))
					filedata = f.read(self.MSS)	
			except socket.timeout:
				print('time_out, transmiting again')
		message = struct.pack('sl?l999s',*(b'f',seq, 1, 0, filedata)) ###finish
		transfile_socket.sendto(message,(HOST,PORTID + destnation))
		print('The file is sent.')
		transfile_socket.close()
		slog.close()

	def Quit(self):
		if self.pred == None:
			time.sleep(PING_GAP + 1) 
		try:
			print(f'Peer {self.id} will depart from the network')
			TCP_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
			TCP_socket.connect((HOST,PORTID+self.pred))
			message = self.Create_message(TOSEND['quit'],self.succ1,self.succ2)
			TCP_socket.send(message)
			TCP_socket.close()
			sys.exit(0)
		except (TypeError,ConnectionRefusedError): #only one peeer in network
			sys.exit(0)
		
	#listening to input command
	def Input(self):
		while(1):
			commend = input("")
			commend.strip()
			if commend == 'quit':
				self.quit_function()
			elif commend[0:7] == 'request':
				try:
					filenum = int(commend[8:]) #delet all 0 front 
					if  0 <= filenum <= 9999:
						self.file_transfer_inital(filenum)
					else:
						raise ValueError
				except ValueError:
					print("Invaild file name")
			else:
				print("Invaild command")


	

if __name__ == "__main__":

	try:
		if len(sys.argv) != 6:
			raise ValueError
		for i in range(1,4):
			sys.argv[i]=int(sys.argv[i])
			if not 0 <= sys.argv[i] <= 255:
				raise ValueError
		sys.argv[-1] = float(sys.argv[-1])
		sys.argv[-2] = int(sys.argv[-2])
	except ValueError:
		print("Incorrect arguments entered")
		sys.exit()
	peer = Peer(sys.argv[1],None,sys.argv[2],sys.argv[3],sys.argv[4],sys.argv[5])
request