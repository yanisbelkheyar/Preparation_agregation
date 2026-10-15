import socket
import select
import sys

socket_serveur=socket.socket(socket.AF_INET, socket.SOCK_STREAM)
socket_serveur.connect(("127.0.0.1", 55555))

inputs=[sys.stdin, socket_serveur]

while inputs:
	print("Entrez un message (inférieur à 256 caractères) :\n")
	
	(read, write, expt) = select.select(inputs, [], [])
	
	for sock in read:
		# données envoyées par le serveur
		if sock is socket_serveur:
			data=socket_serveur.recv(256)
			if data:
				print("Recu du serveur : ", data.decode())
			else:
				inputs.remove(sock)
				sock.close()
		# données recues au clavier
		else:
			msg=sys.stdin.readline()
			socket_serveur.send(msg.encode())

socket_serveur.close()

