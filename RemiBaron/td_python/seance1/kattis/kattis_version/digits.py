from typing import List

def digits_aux(x0 : str) -> int:
    i : int = 1
    xi : str = str(len(x0))
    while(xi != x0):
        x0, xi = xi, str(len(xi))
        i+=1
    return i

def digits(l : List[str]) -> List[int]:
    rep : List[int] = []
    for x0 in l:
        rep.append(digits_aux(x0))
    return rep

if __name__ == "__main__":
    a = input()
    l = []
    while(a != "END"):
        l.append(a)
        a = input()
    l2 = digits(l)
    for i in l2:
        print(i)