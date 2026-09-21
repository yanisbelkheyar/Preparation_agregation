(*QUESTION 1*)
let minimum_tableau (tab : 'a array ) : 'a =
  (*Precondition : tab tableau contenant au moins un élément;
  Renvoie l'élément minimum du tableau*)
  let minimum = ref (tab.(0)) in
  for i = 1 to Array.length(tab)-1 do 
    if !minimum>tab.(i) then minimum := tab.(i)
    done;
  !minimum


let () = 
  assert (minimum_tableau [|3|] = 3);
  assert (minimum_tableau [|3.;6.;8.;-5051.51|] = -5051.51);
  assert (minimum_tableau [|3;6;8;-5051|] = -5051);
  Printf.printf "QUESTION 1 FAITE !\n"


(*QUESTION 2*)
let somme_tableau (tab : int array) : int =
  (*Précondition : tab tableau d'entiers*)
  let sum = ref 0 in
  for i = 0 to (Array.length(tab)-1) do
    sum := !sum + tab.(i)
  done;
  !sum


let () =
  assert (somme_tableau [|3;6;8;-5|] = 12);
  assert (somme_tableau [||] = 0);
  assert (somme_tableau [|15|] = 15);
  Printf.printf "QUESTION 2 FAITE !\n"


(*QUESTION 3*)
exception Not_found

let is_sorted (tab : 'a array) : bool =
  try 
    for i = 1 to (Array.length(tab) -1) do
      if tab.(i) < tab.(i-1) then 
        raise Not_found
    done;
    true
  with 
    |Not_found -> false


let () = 
  assert (is_sorted [|-3;6;8;15|]);
  assert (is_sorted [||]);
  assert (is_sorted [|-3|]);
  assert (is_sorted [|3.;3.;3.;3.;3.;3.|]);
  assert (not(is_sorted [|3;6;8;5|]));
  Printf.printf "QUESTION 3 FAITE !\n"


(*QUESTION 4*)
let tab_to_list_rec (tab : 'a array) : 'a list =
  let rec aux (indice : int) : 'a list =
    match indice with
      |x when x = Array.length(tab) -> []
      |x -> tab.(x) :: (aux (x+1))
  in aux 0

let tab_to_list (tab : 'a array) : 'a list =
  let l = ref [] in
  for i = 0 to (Array.length(tab)-1) do
    l := !l @ [tab.(i)]
  done;
  !l

let list_to_tab_rec (l : 'a list) : 'a array =
  match l with 
    |[] -> [||]
    |head::l -> let tab = Array.make (List.length l + 1) head in
    let rec aux (l : 'a list) (indice : int) : unit =
      match l with
        |[] -> ()
        |head::tail -> tab.(indice)<-head ; aux tail (indice+1)
    in aux l 1; 
    tab 

let list_to_tab (l : 'a list) : 'a array =
  match l with 
    |[] -> [||]
    |head::l -> let tab = Array.make (List.length l + 1) head in
    let temp_list = ref l in
    for i = 0 to (List.length l -1) do
      match !temp_list with 
        |[] -> ()
        |head::tail -> tab.(i+1) <- head ; temp_list := tail
    done;
    tab 


let () = 
  assert (tab_to_list_rec [|"aazza";"jmjm";"mjjmj"|]= ["aazza";"jmjm";"mjjmj"]) ;
  assert (tab_to_list_rec [||]= []) ;
  assert (tab_to_list [|"aazza";"jmjm";"mjjmj"|]= ["aazza";"jmjm";"mjjmj"]) ;
  assert (tab_to_list [||]= []) ;
  assert (list_to_tab_rec ["aazza";"jmjm";"mjjmj"]= [|"aazza";"jmjm";"mjjmj"|]) ;
  assert (list_to_tab_rec []= [||]) ;
  assert (list_to_tab ["aazza";"jmjm";"mjjmj"]= [|"aazza";"jmjm";"mjjmj"|]) ;
  assert (list_to_tab []= [||]);
  Printf.printf "QUESTION 4 FAITE !\n"


(*QUESTION 5*)
let dichoto (tab: 'a array) (elt : 'a) : int =
  (*Précondition : tab est trié*)
  let lower_bound = ref 0 in
  let upper_bound = ref (Array.length tab) in
  while (!upper_bound - !lower_bound > 1) do
    let middle = (!upper_bound + !lower_bound)/2 in 
    (if tab.(middle) <= elt then
      lower_bound := middle
    else
      upper_bound := middle)
  done;
  if ((Array.length tab)!=0 && elt = tab.(!lower_bound)) then
    !lower_bound
  else
    -1

let dichoto_rec (tab: 'a array) (elt : 'a) : int =
  (*Précondition : tab est trié*)
  let rec aux (lower : int) (upper : int) : int =
    if upper-lower <= 1 then
      lower
    else
      (let middle = (upper + lower)/2 in 
      if tab.(middle) <= elt then
        aux middle upper
      else
        aux lower middle)
  in let lower = aux 0 (Array.length tab)
  in if ((Array.length tab)!=0 && elt = tab.(lower)) then
    lower
  else
    -1


let () =
    assert (dichoto [|-3;6;8;15|] (-3) = 0);
    assert (dichoto [|-3;6;8;15|] 8 = 2);
    assert (dichoto [|-3;6;8;15|] 12 = -1);
    assert (dichoto [||] 12 = -1);
    assert (dichoto_rec [|-3;6;8;15|] (-3) = 0);
    assert (dichoto_rec [|-3;6;8;15|] 8 = 2);
    assert (dichoto_rec [|-3;6;8;15|] 12 = -1);
    assert (dichoto_rec [||] 12 = -1);
    Printf.printf "QUESTION 5 FAITE !\n"


(*QUESTION 6*)
let rec fact_rec (x : int) : int =
  (*Précondition : x >= 0*)
  if x = 0 then 1 else x * (fact_rec (x-1))

let fact (x : int) : int =
  (*Précondition : x >= 0*)
  let res = ref 1 in
  let ind = ref x in
  while !ind > 1 do
    res := !ind * !res;
    decr ind
  done;
  !res 


let () =
    assert (fact_rec 5 = 120);
    assert (fact_rec 0 = 1);
    assert (fact 5 = 120);
    assert (fact 0 = 1);
    Printf.printf "\tFactoriel %i : %i \n" 40 (fact 40) (*Gros nombres : ne marche pas*);
    Printf.printf "QUESTION 6 FAITE !\n"


(*QUESTION 7*)
let decompose (x : int) : int list =
  (*Précondition : x >= 1*)
  let rec aux (num : int)  (diviseur : int) : int list =
    match num, num mod diviseur with
    | 1, _ -> []
    | _, 0 -> diviseur :: aux (num/diviseur) diviseur
    | _ -> aux num (diviseur+1)
in aux x 2

let () =
    assert (decompose 120 = [2;2;2;3;5]);
    assert (decompose 1 = []);
    Printf.printf "QUESTION 7 FAITE !\n"