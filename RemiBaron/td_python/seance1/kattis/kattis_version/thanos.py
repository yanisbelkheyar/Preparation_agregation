from typing import List

def thanos(t : int, planets : List[List[int]]) -> List[int]:
    assert(len(planets) == t)
    rep : List[int] = [0] * t
    for i in range(t):
        assert(len(planets[i])==3)
        current_pop : int = planets[i][0]
        years_alive : int = 0
        while(current_pop<=planets[i][2]):
            current_pop*=planets[i][1]
            years_alive+=1
        rep[i] = years_alive
    return rep

if __name__ == "__main__":
    n = int(input())
    planets=[]
    for _ in range(n):
        p, r, f = map(int, input().split())
        planets.append([p,r,f])
    l = thanos(n, planets)
    for i in l:
        print(i)