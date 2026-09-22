from typing import List

def is_prime(n:int)-> bool:
    if n<2:
        return False
    i:int = 2
    while(i*i<=n):
        if n%i == 0 :
            return False
        i+=1
    return True

def eratosthene_invariant_check(l: List[bool],i: int) -> None:
    #on peut enlever des cas de la boucle mais je ne sais pas s'il faut mieux les laisser pour rester proche de la def de l'invariant
    for k in range(2, len(l)):
        if l[k]:
            for j in range(2, k):
                if k%j == 0:
                    assert(j>i) 

def crible_eratosthene(n:int) -> List[bool]:
        #Precondition : n > 1
        assert(n>1)
        l : List[bool] = [True] * n
        l[0], l[1] = False, False
        i : int = 1
        while(i<n):
            i += 1
            while(i<n and not(l[i])):
                i+=1
            if i>= n:
                break
            for j in range(i+1,n):
                if j%i == 0:
                    l[j] = False
            """Invariant : pour tout k ≥ 2, L[k] est vrai si et seulement si tout diviseur propre de k est supérieur à i."""
            eratosthene_invariant_check(l,i)
        return l

def tests() -> None:
    assert(is_prime(11))
    assert(not(is_prime(75)))
    assert(is_prime(2))
    assert(is_prime(3))
    assert(not(is_prime(4)))
    assert(crible_eratosthene(2)==[False, False])
    assert(crible_eratosthene(10)==[False, False, True, True, False, True, False, True, False, False])
    assert(crible_eratosthene(100)==crible_eratosthene(1000)[:100])
    l = crible_eratosthene(1000)
    assert all(is_prime(i) for i in range(1000) if l[i])
        

if __name__ == "__main__":
    tests()
    print("Everything is okay. Youpi !")
    print(f"{67}\n{67:.2f}")
    
#mypy --disallow-untyped-defs exemples.py donne Success