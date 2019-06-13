import sys
import threading
import socket
import time
import struct
import random



HOST = "127.0.0.1"
PORTID = 40000 ######记得改回50000
PING_TIMEOUT = 1.0
FILE_TIMEOUT = 1.0
PING_GAP = 4.0 
UDP_BUFFER = 1024 
TCP_BUFFER = 1024 
ACK_ACCUMULATION_LIMIT = 5 



#dictionary to determine message types
# messages can be called either way to help make code readable.
MESSAGETYPE = {0:'pingreq', 1:'pingres', 2:'filereq', 3:'fileres', 4:'lostpeer', 5:'quit'}
ENCR_MTYPE = {'pingreq':0, 'pingres':1, 'filereq':2, 'fileres':3, 'lostpeer':4, 'quit':5}


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
		t0 = threading.Thread(target=self.input_function)
		t0.start()

		#PING successors 
		t1 = threading.Thread(target = self.send_ping, args = (1,))
		t2 = threading.Thread(target = self.send_ping, args = (2,))
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



	

	# creates byte array message to incorporate sender ID and extra information when needed
	def Create_message(self, TYPE, info1 = None, info2 = None, info3 = None, info4 = None):
		message = bytearray([TYPE])
		message.extend( struct.pack("B", self.id))
		if info1 != None:
			message.extend( struct.pack("B", info1))
		if info2 != None:
			message.extend( struct.pack("B", info2))
		if info3 != None:
			message.extend( struct.pack("B", info3))	
		if info4 != None:
			message.extend( struct.pack("B", info4))	
		return message

	# If no ping ACK and a peer is dead, this function updates peers through TCP
	# divided into the two cases for succ1 and succ2
	# succ1 updates immediately moving second forward then pinging that successor
	# succ2 waits for ping freq for the next sucessor to update then contacts for information
	def lost_peer(self, succ_number):
		TCP_socket = socket.socket(socket.AF_INET,socket.SOCK_STREAM)
		message = self.Create_message(ENCR_MTYPE['lostpeer'])   #####test 这里
		
		if succ_number == 1: #assume first sucessor is lost
			self.succ1 = self.succ2
			TCP_socket.connect((HOST,PORTID+self.succ2))
		elif succ_number == 2:
			time.sleep(PING_GAP+1) # sleep long enough to allow sucessors sucessor to update
			TCP_socket.connect((HOST,PORTID+self.succ1))
		TCP_socket.send(message)
		data = TCP_socket.recvfrom(TCP_BUFFER)
		self.succ2 = int(data[0])
		TCP_socket.close()
		if self.succ1 == self.id: #case for two nodes and one quits
			print("I'm last peer on network, I have no successors")
			self.succ1 = None
			self.succ2 = None
		else:
			print(f'My first successor is now peer {self.succ1}')
			print(f'My second sucessor is now peer {self.succ2}')


	# function to send_pings via UDP to successors
	def send_ping(self,indicator):
		last_sequence = 0
		sequenceNum = 0
		while True:
			if indicator == 1:
				targetID = self.succ1
			else:
				targetID = self.succ2
			if not targetID or targetID == self.id:
				return
			UDP_socket = socket.socket(socket.AF_INET,socket.SOCK_DGRAM)
			message = self.Create_message(ENCR_MTYPE['pingreq'],indicator,sequenceNum)
			# sets timeout if package isn't received in time (1 second in this case)
			UDP_socket.settimeout(PING_TIMEOUT)
			UDP_socket.sendto(message,(HOST,PORTID+targetID))
			try:
				data,addr = UDP_socket.recvfrom(UDP_BUFFER)
				responseID = int(data[1])
				if MESSAGETYPE[data[0]] == 'pingres':
					print(f"A ping response message was received from Peer {responseID}")
					last_sequence = sequenceNum
			except socket.timeout:
				  #####这里有修改

				if last_sequence > sequenceNum:
					ACK_accumulation =  255 - last_sequence + sequenceNum + 1 #1 because zero case
				else:
					ACK_accumulation =  sequenceNum - last_sequence

				if ACK_accumulation:
					print(f"No ping response from Peer {targetID}, no ACK's accumulation = {ACK_accumulation}, sequence gap = [ {last_sequence} - {sequenceNum} ]")
				if ACK_accumulation > ACK_ACCUMULATION_LIMIT: #succ is dead
					print(f'Peer {targetID} is no longer alive.')
					self.lost_peer(indicator)
			sequenceNum = (sequenceNum+1)%256   #####这里有过修改 next sequence number
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
					if MESSAGETYPE[data[0]] == 'pingreq':
						senderID = int(data[1])
					## if Ping is from first sucessor, update the predecessor qui
						if data[2] == 1:
							self.pred = int(senderID)
						print(f"A ping request message was received from Peer {senderID}")
						message = self.Create_message(ENCR_MTYPE['pingres'],data[3])	
						Server_socket.sendto(message,addr)
				except KeyError:
					packet = struct.Struct('sl?l992s')
					message = packet.unpack(data)
					seq = message[1]
					flag = message[2]
					if flag == False:
						if seq == 1:
							received_file = open('received_file.pdf', "wb")
							rlog = open('receivered_log.txt', "w")
					
						lendata = message[3]
						filedata = message[4][:lendata]
						received_file.write(filedata)
						t_gap = time.time() - self.starttime
						rlog.write('{:<18}\t{:<8.2f}\t{:<10}\t{:<10}\t{:<10}\n' .format('rcv', t_gap ,seq,lendata, 0))
						
						ack = str(message[1] + lendata)
						Server_socket.sendto(ack.encode(),addr)
						rlog.write('{:<18}\t{:<8.2f}\t{:<10}\t{:<10}\t{:<10}\n' .format('snd', t_gap ,0 ,lendata, ack))
					else:
						print('The file is received.')
						received_file.close()
						rlog.close()

			except ConnectionRefusedError:
				print("Couldn't connect to peer, please wait until peers update and try again")


	#function for listening to TCP messages  kill peer/ peer depattur/ file requet and response
	def TCP(self):
		Server_socket = socket.socket(socket.AF_INET,socket.SOCK_STREAM)
		Server_socket.bind((HOST,self.port))
		Server_socket.listen()
		while 1:
			Connection_socket, addr = Server_socket.accept()
			data = Connection_socket.recv(TCP_BUFFER)
	
			try:
				if MESSAGETYPE[data[0]] == 'quit': # quit message received (update sucessors)
					if int(data[1]) == self.succ1:
						TCP_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
						if int(data[2]) == self.id:
							print("I'm last peer on network, I have no successors")
							self.succ1 = None
							self.succ2 = None
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
						print(f'My second sucessor is now peer {self.succ2}')


				elif MESSAGETYPE[data[0]] == 'lostpeer': #lost peer message received
					# return a message containing my successor
					message = str(self.succ1).encode('utf-8')
					Connection_socket.send(message)

				elif MESSAGETYPE[data[0]] == 'filereq' : #file request from peer
					TCP_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
					# evaluates if it is held at my location
					# base case, general case, smaller than smallest peer and case of larger than highest peer,
					if self.id == data[2] or \
					(self.pred < data[2] <= self.id) or \
					(self.succ1 < self.id and data[2] > self.id) or\
					(self.pred > self.id and data[2] < self.id) :
						print(f'File {data[3]*256 + data[2]} is here.')

						###print(f'A response message, destined for Peer {data[1]}, has been sent')
						## 回应 TCP
						TCP_socket.connect((HOST, PORTID + data[1]))
						message = self.Create_message(ENCR_MTYPE['fileres'],data[2], data[3]) ####file request model
						TCP_socket.send(message)
						print(f'A response message, destined for peer {data[1]}, has been sent.')
						TCP_socket.close()
						####传输文件
						self.file_transfer(data[1],data[3]*256 + data[2])


					else: #forward the message to the next peer
						print(f'File {data[3]*256 + data[2]} is not stored here.')
						print('File request has been forwarded to my sucessor')
						TCP_socket.connect((HOST,PORTID+self.succ1))
						TCP_socket.send(data)
						TCP_socket.close()

				elif MESSAGETYPE[data[0]] == 'fileres': #file response from peer
					print(f'Received a response message from peer {data[1]}, which has file {data[3]*256 + data[2]}')
					print('we now start receiving the file...')

				
			except ConnectionRefusedError:
				print("Couldn't connect to peer, please wait until peers update and try again")
			
			Connection_socket.close()


	# if file request is called in input
	def file_transfer_inital(self,filenum):
		filehash = filenum % 256 ##取余数
		filevalue = filenum // 256 
		if not self.pred: # if predecessors are None, they need t update
			print("please wait until predecessors update")
		#checks if the file is located at my location
		elif filehash == self.id or \
		self.succ1 == None or \
		(self.pred < filehash <= self.id) or \
		(self.succ1 < self.id and filehash > self.id) or \
		(self.pred > self.id and filehash < self.id):
			print(f"File {filenum} is found here in peer {self.id}")

		else: #else it is forwarded to next peer
			try:
				print(f'File {filenum} is not stored here.')
				print('File request has been forwarded to my sucessor')
				### 256 * filevalue + filehash
				### 256 * date[3] + date[2]
				message = self.Create_message(ENCR_MTYPE['filereq'],filehash, filevalue)####file response model 'filereq'], filehash, filevalue, requester
				TCP_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
				TCP_socket.connect((HOST,PORTID+self.succ1))
				TCP_socket.send(message)
				TCP_socket.close()
			except ConnectionRefusedError:
				print("Couldn't connect to peer, please wait until peers update and try again")
	
	def file_transfer(self,destnation,filenum):
		packet = struct.Struct('sl?l992s')
		intial_seq = 1 
		transfile_socket = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
		f = open(str(filenum) + '.pdf', "rb" )
		filedata = f.read(self.MSS)
		transfile_socket.settimeout(1)
		slog = open('sender_log.txt', "w")
		event = ''
		while filedata != b'':
			try:
				lendata = len(filedata)
				time_gap = time.time() - self.starttime
				seq = intial_seq
				if random.random() > self.droprate:
					if event == 'Drop' or event == 'RTX/Drop': 
						event = 'RTX'
					else:
						event = 'snd'
					message = packet.pack(*(b'f',intial_seq, False, lendata, filedata))
					transfile_socket.sendto(message,(HOST,PORTID + destnation))
				else: ###丢包重传
					if event == 'Drop' or event == 'RTX/Drop': 
						event = 'RTX/Drop'
					else:
						event = 'Drop'
				slog.write('{:<18}\t{:<8.2f}\t{:<10}\t{:<10}\t{:<10}\n' .format(event, time_gap, seq, lendata, 0))
				#print('9')
				ack = int(transfile_socket.recv(1024).decode('utf_8'))
				if ack == intial_seq + lendata: #send next MSS
					ack_nb = ack
					intial_seq += lendata
					filedata = f.read(self.MSS)
					time_gap = time.time() - self.starttime
					slog.write('{:<18}\t{:<8.2f}\t{:<10}\t{:<10}\t{:<10}\n' .format('rcv', time_gap,0,lendata,ack_nb))
			except socket.timeout:
				print('time_out, transmiting again')
		message = packet.pack(*(b'f',intial_seq, True, 0, filedata)) ###完成传送
		transfile_socket.sendto(message,(HOST,PORTID + destnation))
		print('The file is sent.')
		transfile_socket.close()
		slog.close()






	#if quit is called, it informs the peers of departure
	def quit_function(self):
		if self.pred == None:
			time.sleep(PING_GAP+1) #waits 5 seconds to check if predecessors just havent been updated.
		try:
			print(f'Peer {self.id} will depart from the network')
			print("please allow a few seconds before subsuquent quit functions to allow peers to update")
			message = self.Create_message(ENCR_MTYPE['quit'],self.succ1,self.succ2)
			TCP_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
			TCP_socket.connect((HOST,PORTID+self.pred))
			TCP_socket.send(message)
			TCP_socket.close()
			sys.exit(0)
		except (TypeError,ConnectionRefusedError): #case where it's the only node left
			sys.exit(0)
		
	# function to constantly listen to input
	def input_function(self):
		while(1):
			inp = input("")
			inp.strip()
			if inp == 'quit':
				self.quit_function()
			elif inp[0:7] == 'request':
				try:
					filenum = int(inp[8:])
					if not 0 <= filenum <= 9999:
						raise ValueError
					self.file_transfer_inital(filenum)
				except ValueError:
					print("incorrect input: file number incorrect")
			else:
				print("Incorrect input: input doesn't match any function")


	

if __name__ == "__main__":

	## ensures correct amount of arguments entered (3) and  ##
	## ensure of the right format (integer) and ensure right range 0 <= arg <= 255 ##
	try:
		if len(sys.argv) != 6:
			print('hahahha')
			raise ValueError
		for i in range(1,4):
			sys.argv[i]=int(sys.argv[i])
			if not 0 <= sys.argv[i] <= 255:
				print('ffffffff')
				raise ValueError
			sys.argv[-1] = float(sys.argv[-1])
			sys.argv[-2] = int(sys.argv[-2])
	except ValueError:
		for i in range(len(sys.argv)):
			print(sys.argv[i])
		print("Incorrect arguments entered")
		sys.exit()

	#sets up peer with class functionality
	###packet = struct.Struct('s1?1992s')
	###starttime = time.time()
	#'f' <sequece> <end> <daralength> <data>
	peer = Peer(sys.argv[1],None,sys.argv[2],sys.argv[3],sys.argv[4],sys.argv[5])
    