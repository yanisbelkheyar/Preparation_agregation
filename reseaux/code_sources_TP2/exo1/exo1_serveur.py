import socket

socket_ecoute=socket.socket(socket.AF_INET, socket.SOCK_STREAM)
socket_ecoute.bind(("127.0.0.1", 55555))
socket_ecoute.listen(5)

while True:
	(socket_service, (ip_client, port_client))=socket_ecoute.accept()
	print(ip_client, port_client, "connecte")
	
	data1=socket_service.recv(256)
	print("MSG1 recu : ", data1.decode())
	
	data2=socket_service.recv(256)
	print("MSG2 recu : ", data2.decode())
	
	data=data1.decode()+" "+data2.decode()
	socket_service.send(data.encode())
	
	socket_service.close()
	
socket_ecoute.close()


