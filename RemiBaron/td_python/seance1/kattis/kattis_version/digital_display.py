from typing import List

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
        line+=str_digit_line(nums[0], i)+"  "+str_digit_line(nums[1],i)
        if(i==2 or i==4):
            line+="  o  "
        else:
            line+="     "
        line+=str_digit_line(nums[2], i)+"  "+str_digit_line(nums[3],i)
        print(line)
    print("\n")
    
if __name__ == "__main__":
    a = input().strip()
    while(a != "end"):
        clock_display(a)
        a=input().strip()
    print("end")