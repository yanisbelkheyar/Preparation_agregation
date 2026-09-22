from typing import List

def mymax(L: List[int]) -> int:
    """Retourne le maximum d'une liste d'entiers non vide.
    Lève une ValueError si la liste est vide"""

    if len(L) == 0:
        raise ValueError("Impossible de calculer le maximum d'une liste vide")
    m: int = L[0]  # m contient le max des entiers que l'on a vus jusqu'à ici
    i: int = 1
    while i < len(L):
        if L[i] > m:
            m = L[i]
        i += 1
    return m

#2 : mymax.mymax ([]) donne une erreur liste vide
#3 : help(mymax.mymax) donne la doc de la fonction mymax
#4 : mypy mymax.py donne "Success: no issues found in 1 source file"
#5 : Incompatible types in assignment :  on assigne un entier à une variable supposée être un string
#       et Unsupported operand types for > (int and str) : faire x>y lève une erreur si x est un int et y un string
#6 : mypy --disallow-untyped-defs mymax.py donne Function is missing a return type annotation
#       car dans la def de mymax on ne précise pas le type de sortie
#       pour régler ça on ajoute -> int lors de la definition car mymax renvoie m qui est un entier 
