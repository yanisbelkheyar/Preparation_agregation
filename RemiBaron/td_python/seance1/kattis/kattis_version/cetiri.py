from typing import List

def cetiri(n1 : int, n2 : int, n3 : int) -> int:
    l : List[int] = [n1, n2, n3]
    l.sort()
    diff1 : int = l[1] - l[0]
    diff2 : int = l[2] - l[1]
    if diff1 == diff2 :
        return l[2] + diff1
    return l[0]+diff2 if diff1>diff2 else l[1]+diff1

if __name__ == "__main__":
    n1, n2, n3 = map(int, input().split())
    print(cetiri(n1,n2,n3))