def get_r2(r1: int, s: int) -> int:
    return 2*s - r1


if __name__ == "__main__" :
    r1, s = map(int,input().split())
    print(get_r2(r1,s))