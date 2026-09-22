def count_1s(s:str) ->int:
    count : int = 0
    for i in s:
        if i == '1':
            count +=1
    return count

def bits_equalizer(s:str, t:str) -> int :
    assert(len(s)==len(t))
    t_1 : int = count_1s(t)
    s_1 : int = count_1s(s)
    total_op : int = 0
    #S'il y a déjà trop de 1 dans s alors on est foutu
    if s_1 > t_1:
        return -1
    #Sinon, il faut faire attention à ne pas en rajouter trop en changeant paresseusement les ?
    #On change donc les ? en la cible dans t sauf si ? doit être changé en 1 et qu'il y a déjà pile le nombre de 1
    for i in range(len(s)):
        if s[i] == '?':
            total_op += 1
            if t[i]=='1' and  s_1 < t_1:
                s_1 += 1
    #On fait en sorte qu'il n'y ait pas de 1 là où il faut un zero : on fait une opération à chaque fois (=1 echange)
    for i in range(len(s)):
        if s[i]=='1' and t[i]=='0':
            total_op+=1
    #Les seules différences possibles c'est des 0 de s au lieu de 1 dans t : on fait une opération pour chacun (=un 0->1)
    total_op += max(0,t_1 - s_1)
    return total_op

if __name__ == "__main__":
    n = int(input().strip())
    for i in range(n):
        s = input().strip()
        t = input().strip()
        rep = bits_equalizer(s,t)
        print("Case "+str(i+1)+": "+str(bits_equalizer(s,t)))