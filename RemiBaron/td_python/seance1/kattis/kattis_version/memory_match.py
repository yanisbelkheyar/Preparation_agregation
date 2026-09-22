from typing import List

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
        if other_num_card != num_card and other_num_card>0:
            state_cards[ind] = 0
    return symbols_seen

def memory_match(n:int,k:int,card_numbers:List[tuple[int,int]], card_symbols:List[tuple[str,str]]) -> int:
    assert(len(card_numbers)==k)
    assert(len(card_symbols)==k)
    state_cards : List[int]= [-1]*(n//2) #state_cards[i] = -1 if no info on symbol i, -2 if symbol has been found,0 if card is findable, indice of card with symbol otherwise.
    symbol_to_num : List[str] = [""]*(n//2) #[First symbol, second symbol, ..., "", "", ...]
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
    pairs_finished : int = 0
    for a in state_cards:
        if a == -2:
            pairs_finished += 1
    if symbols_seen == n//2:
        return (n//2)-pairs_finished
    #Sinon si tout a été retourné sauf deux cartes dont le symbole est inconnu, on renvoie 1
    elif symbols_seen == (n//2)-1 and pairs_finished==(n//2)-1:
        return 1
    #Sinon on peut juste avoir pour certain ceux où les deux cartes de la pair ont été vus.
    else :
        rep : int = 0
        for a in state_cards:
            if a == 0:
                rep += 1
        if symbols_seen == (n//2)-1 and pairs_finished+rep==(n//2)-1:
            rep += 1
        return rep
    
if __name__ == "__main__":
    n = int(input().strip())
    k = int(input().strip())
    card_numbers = []
    card_symbols = []
    for i in range(k):
        a,b,c,d = input().split()
        card_numbers.append((int(a), int(b)))
        card_symbols.append((c,d))
        
    print(memory_match(n,k,card_numbers, card_symbols))