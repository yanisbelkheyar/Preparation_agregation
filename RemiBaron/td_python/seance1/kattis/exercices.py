from typing import List

#Pas la version finale, qui est dans le dossier kattis_version

def get_r2(r1: int, s: int) -> int:
    return 2*s - r1

def cetiri(n1 : int, n2 : int, n3 : int) -> int:
    l : List[int] = [n1, n2, n3]
    l.sort()
    diff1 : int = l[1] - l[0]
    diff2 : int = l[2] - l[1]
    if diff1 == diff2 :
        return l[2] + diff1
    return l[0]+diff2 if diff1>diff2 else l[1]+diff1

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

def get_num_digits(x:int, aux:int) -> int:
    if x//10 <1:
        return aux+1
    return get_num_digits(x//10, aux+1)
        

def digits_aux(x0 : int) -> int:
    i : int = 1
    xi : int = get_num_digits(x0, 0)
    while(xi != x0):
        x0, xi = xi, get_num_digits(xi,0)
        i+=1
    return i

def digits(l : List[int]) -> List[int]:
    #Version pas bonne, changée dans kattis_version/digits.py
    rep : List[int] = []
    for x0 in l:
        rep.append(digits_aux(x0))
    return rep

#int | str ne marche que pour Python => 3.10 et à l'agreg on semble avoir 3.10.6. Sinon il faut utiliser from typing import Union; Union[int, str]
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
    
def remove_after_dec_point(s:str) -> str:
    #Precondi, s de la forme n1.n2
    remove_end : int = 1
    n : int = len(s)
    while(remove_end < n and (s[n-remove_end]=='0' or s[n-remove_end]=='.')):
        remove_end+=1
        if s[n-remove_end+1] =='.':
            break
    remove_end -= 1
    return s[:n-remove_end]
    
def magic_division(n: int, m:int) -> str :
    #changed a lot, see kattis_version/divide.py
    n_str : str = str(n)
    n_digits : int = len(n_str)
    m_digits : int = len(str(m))
    diff : int = n_digits-m_digits
    if diff>0:
        n_str = n_str[:diff+1]+"."+n_str[diff+1:]
    else :
        prefixe : str = "0."
        for _ in range(1,diff*-1):
            prefixe += "0"
        n_str = prefixe + n_str
    return remove_after_dec_point(n_str)

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
    
def str_digit_line(digit : int, line : int)->str:
    digit0 : List[str] = [
        "+---+", "|   |", "|   |", "+   +","|   |","|   |","+---+"
    ]
    digit1 : List[str] = [
            "    +", "    |", "    |", "    +","    |","    |","    +"
        ]
    digit2 : List[str] = [
            "+---+", "    |", "    |", "+---+","|    ","|    ","+---+"
        ]
    digit3 : List[str] = [
            "+---+", "    |", "    |", "+---+","    |","    |","+---+"
        ]
    digit4 : List[str] = [
            "+   +", "|   |", "|   |", "+---+","    |","    |","    +"
        ]
    digit5 : List[str] = [
            "+---+", "|    ", "|    ", "+---+","    |","    |","+---+"
        ]
    digit6 : List[str] = [
            "+---+", "|    ", "|    ", "+---+","|   |","|   |","+---+"
        ]
    digit7 : List[str] = [
            "+---+", "    |", "    |", "    +","    |","    |","    +"
        ]
    digit8 : List[str] = [
            "+---+", "|   |", "|   |", "+---+","|   |","|   |","+---+"
        ]
    digit9 : List[str] = [
            "+---+", "|   |", "|   |", "+---+","    |","    |","+---+"
        ]
    all_digits : List[List[str]] = [digit0, digit1, digit2, digit3, digit4, digit5, digit6, digit7, digit8, digit9]
    assert(digit>=0 and digit<=9)
    assert(line>=0 and line<=6)
    return all_digits[digit][line]
    
def clock_display(time : str) -> None :
    nums : List[int] = list(map(int, [time[0], time[1], time[3], time[4]]))
    for i in range(7):
        line : str = ""
        line+=str_digit_line(nums[0], i)+" "+str_digit_line(nums[1],i)
        if(i==2 or i==4):
            line+=" o "
        else:
            line+="   "
        line+=str_digit_line(nums[2], i)+" "+str_digit_line(nums[3],i)
        print(line)
    print("\n\n")
    
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

def find_in_str_list(a:str, l:List[str]) -> int:
    for i in range(len(l)):
        if l[i] == a:
            return i
    return -1

def manage_card(symbol : str, num_card : int, state_cards : List[int], symbol_to_num : List[str], symbols_seen:int)->int:
    ind : int = find_in_str_list(symbol, symbol_to_num)
    if ind == -1:
        symbol_to_num[symbols_seen] = symbol
        state_cards[symbols_seen] = num_card
        symbols_seen+=1
    else:
        other_num_card : int = state_cards[ind]
        if other_num_card != ind and other_num_card>0 and state_cards[ind] != -2:
            state_cards[ind] = 0
    return symbols_seen

def memory_match(n:int,k:int,card_numbers:List[tuple[int,int]], card_symbols:List[tuple[str,str]]) -> int:
    assert(len(card_numbers)==k)
    assert(len(card_symbols)==k)
    state_cards : List[int]= [-1]*n #state_cards[i] = -1 if no info on symbol i, -2 if symbol has been found,0 if card is findable, indice of card with symbol otherwise.
    symbol_to_num : List[str] = [""]*n #[First symbol, second symbol, ..., "", "", ...]
    symbol1 :str
    symbol2 : str 
    symbols_seen : int = 0
    for i in range(k):
        symbol1, symbol2 = card_symbols[i]
        if symbol1==symbol2:
            ind : int = find_in_str_list(symbol1, symbol_to_num)
            if ind == -1:
                symbol_to_num[symbols_seen] = symbol1
                state_cards[symbols_seen] = -2
                symbols_seen += 1
            else :
                state_cards[ind] = -2
                
        else :
            symbols_seen = manage_card(symbol1, card_numbers[i][0], state_cards, symbol_to_num, symbols_seen)
            symbols_seen = manage_card(symbol2, card_numbers[i][1], state_cards, symbol_to_num, symbols_seen)
        
    
    #Si tous les symboles ont été vus, on peut finir la partie
    if symbols_seen == n//2:
        pairs_finished : int = 0
        for a in state_cards:
            if a == -2:
                pairs_finished += 1
        return (n//2)-pairs_finished
    #Sinon on peut juste avoir pour certain ceux où les deux cartes de la pair ont été vus.
    else :
        rep : int = 0
        for a in state_cards:
            if a == 0:
                rep += 1
        return rep