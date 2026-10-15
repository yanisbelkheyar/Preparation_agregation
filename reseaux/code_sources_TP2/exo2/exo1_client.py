import socket

socket_serveur=socket.socket(socket.AF_INET, socket.SOCK_STREAM)
socket_serveur.connect(("127.0.0.1", 55555))

msg=input("Entrez votre nom (inférieur à 256 caractères) :\n")
socket_serveur.send(msg.encode())

msg=input("Entrez votre nom (inférieur à 256 caractères) :\n")
socket_serveur.send(msg.encode())
data=socket_serveur.recv(512)

print("Recu du serveur : ", data.decode())

socket_serveur.close()

