import sys
from typing import List
input = sys.stdin.readline

def remove_after_dec_point(s:str) -> str:
    #Precondi, s de la forme n1.n2
    s = s.rstrip('0')
    if s.endswith('.'):
        s = s[:-1]
    return s
    
def magic_division(n_str: str, m_str:str) -> str :
    n_digits : int = len(n_str)
    m_digits : int = len(m_str)
    diff : int = n_digits-m_digits
    if diff>0:
        sys.stdout.write((n_str[:diff+1]+remove_after_dec_point("."+n_str[diff+1:]))+"\n")
    else :
        prefixe : str = "0." + "0"*(-diff-1)
        sys.stdout.write((prefixe + remove_after_dec_point(n_str)) + "\n")

if __name__ == "__main__":
    n = input().strip()
    m = input().strip()
    magic_division(n,m)