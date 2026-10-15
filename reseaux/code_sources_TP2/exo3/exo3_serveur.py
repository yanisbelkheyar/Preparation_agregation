import socket
import select


socket_ecoute=socket.socket(socket.AF_INET, socket.SOCK_STREAM)
socket_ecoute.bind(("127.0.0.1", 55555))
socket_ecoute.listen(5)

inputs=[socket_ecoute]


while inputs:

	(read, write, expt) = select.select(inputs, [], [])
	
	for sock in read:
		if sock is socket_ecoute:
			(socket_service, (ip_client, port_client))=socket_ecoute.accept()
			inputs.append(socket_service)
			print(ip_client, port_client, "connecte")
		else:
			data=sock.recv(256)
			if data:
				print("MSG recu : ", data.decode())
				sock.send(data)
			else:
				inputs.remove(sock)
				sock.close()
	
socket_ecoute.close()


