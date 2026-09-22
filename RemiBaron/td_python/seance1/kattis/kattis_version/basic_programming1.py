from typing import List

def basic_prog(n : int, t : int, l : List[int]) -> int | str : 
    assert(len(l)==n)
    assert(t>0 and t<8)
    if t==1 :
        return 7
    elif t==2 :
        assert(n>=2)
        if l[0] == l[1]:
            return "Equal"
        elif l[0] > l[1]:
            return "Bigger"
        else :
            return "Smaller"
    elif t ==3 :
        assert(n>=3)
        temp_l : List[int] = [l[0], l[1], l[2]]
        temp_l.sort()
        return temp_l[1] 
    elif t==4:
        rep :int = 0
        for i in l:
            rep+=i
        return rep
    elif t==5:
        rep:int = 0
        for i in l:
            if i%2 == 0:
                rep+=i
        return rep
    elif t==6:
        return ''.join(list(map(lambda x: chr(x+ord('a')), map(lambda x: x%26, l))))
    elif t==7:
        i :int = 0
        for _ in range(n):
            #si c'est non cyclique, on ne prend que des nouvelles valeurs à chaque itération, or il y a max n valeurs prises
            i = l[i]
            if i<0 or i>=n:
                return "Out"
            elif i == n-1:
                return "Done"
        return "Cyclic"
    
if __name__ == "__main__":
    n, t = map(int,input().split())
    l = list(map(int,input().split()))
    print(basic_prog(n,t,l))