import socket
import threading

class ThreadFils(threading.Thread):
	def __init__(self, ip, port, clientsocket):
		threading.Thread.__init__(self)
		self.ip=ip
		self.port=port
		self.clientsocket=clientsocket
	
	def run(self):
		print("connecte ", self.ip, ":", self.port)
		
		data1=self.clientsocket.recv(256)
		print("MSG1 recu : ", data1.decode())
	
		data2=self.clientsocket.recv(256)
		print("MSG2 recu : ", data2.decode())
	
		data=data1.decode()+" "+data2.decode()
		self.clientsocket.send(data.encode())
	
		self.clientsocket.close()
		
socket_ecoute=socket.socket(socket.AF_INET, socket.SOCK_STREAM)
socket_ecoute.bind(("127.0.0.1", 55555))
socket_ecoute.listen(5)

while True:
	(socket_service, (ip_client, port_client))=socket_ecoute.accept()
	
	s=ThreadFils(ip_client, port_client, socket_service)
	s.start()
	
socket_ecoute.close()

