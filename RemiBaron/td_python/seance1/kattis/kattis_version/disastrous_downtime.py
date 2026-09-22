from typing import List

def clean_requests(old_tasks : List[int], new_timestamp : int) -> None:
    #Les lists sont modifiées en mémoire je crois en python
    while(len(old_tasks)>0 and old_tasks[0]<=new_timestamp-1000):
        old_tasks.pop(0)

def server_needed(n:int, k:int, t: List[int]) -> int :
    assert(len(t)==n)
    current_requests : List[int] = []
    max_requests : int = 0
    for i in range(n):
        clean_requests(current_requests, t[i])
        current_requests.append(t[i])
        max_requests = max(max_requests, len(current_requests))
    if max_requests%k == 0:
        return max_requests//k
    else :
        return (max_requests//k)+1
    
if __name__ == "__main__":
    n, k = map(int, input().split())
    t = []
    for _ in range(n):
        t.append(int(input().strip()))
    print(server_needed(n,k,t))