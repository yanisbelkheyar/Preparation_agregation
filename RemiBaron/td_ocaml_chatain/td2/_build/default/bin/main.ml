(*QUESTION 1*)
exception Tableau_vide

let min_tab (tab : 'a array) : 'a =
  if Array.length tab = 0 then
    raise Tableau_vide;
  let aux (a : 'a) (b : 'a) : 'a =
    if (b<a) then b else a
  in Array.fold_left aux (tab.(0)) tab

let somme (tab : int array) : int =
  Array.fold_left (+) 0 tab

let is_trie (tab : 'a array) : bool =
  if Array.length tab = 0 then
    true 
  else
    let rep = ref true in
    let aux (a : 'a) (b : 'a) : 'a =
      if a>b then (rep := false;a) else b
    in let _ = Array.fold_left aux tab.(0) tab
  in !rep

let array_to_list (tab : 'a array) : 'a list =
  Array.fold_left (fun a b -> a @ [b]) [] tab

let () =
  assert (min_tab [|5;3;1;10|] = 1);
  assert (min_tab [|-1.;50.50;-38.|] = -38.);
  assert (somme [|1;2;3;4;5;6|] = 21);
  assert (somme [||] = 0);
  assert (somme [|-1;-2|] = -3);
  assert (is_trie [|1;2;3;4;5|]);
  assert (is_trie [|1|]);
  assert (is_trie [||]);
  assert (not(is_trie [|5;6;1;9|]));
  assert (array_to_list [||] = []);
  assert (array_to_list [|1|] = [1]);
  assert (array_to_list [|"tata";"toto"|] = ["tata";"toto"]);  
  Printf.printf "Question 1 ok.\n"

(*QUESTION 2*)
let increment_tab (tab : int array) : int array =
  Array.map (fun a -> a+1) tab

let () =
  assert (increment_tab [||] = [||]);
  assert (increment_tab [|1|] = [|2|]);
  assert (increment_tab [|-1;5;-9;10|] = [|0;6;-8;11|]);
  Printf.printf("Question 2 ok.\n")

(*QUESTION 3*)
let rec itere (f:'a->'a) (n:int) : 'a->'a =
  match n with 
  |0 -> (fun x -> x)
  |1 -> (fun x -> f x)
  |_ -> (fun x -> ((itere f (n-1))) (f x))
  
let () = 
  assert ((itere (fun x -> x+1) 0) 0 = 0);
  assert ((itere (fun x -> x+1) 1) 0 = 1);
  assert ((itere (fun x -> x+1) 10) 0 = 10);
  assert ((itere (fun x -> "a"^x) 10) "" = "aaaaaaaaaa");
  Printf.printf("Question 3 ok.\n")

(*QUESTION 4*)
let teste_preds (preds : ('a -> bool) list) (elt : 'a) : bool list =
  List.fold_left (fun l pred -> l @ [pred elt]) [] preds

let () =
  assert ((teste_preds [(fun x->x=0); (fun x->(x mod 2) = 0); (fun x->x>0)] 0) = ([true; true; false]));
  assert ((teste_preds [(fun x->x=0); (fun x->(x mod 2) = 0); (fun x->x>0)] 5) = ([false; false; true]));
  Printf.printf("Question 4 ok.\n")

(*QUESTION 5*)
let rec sum (a:int) (b:int) (f:int->int) : int =
  if (a>b) then
    0
  else 
    f a + (sum (a+1) b f) 

let () =
    assert (sum 1 100 (fun i -> i * i) = (100)*(101)*(201)/6);
    assert (sum 1 10 (fun i -> i+5) = 105);
    Printf.printf("Question 5 ok.\n")

(*QUESTION 6*)
let q6 (a: 'a) : (unit -> 'a) =
  fun _ -> a

let () =
  assert (q6 5 () = 5);
  (*Utilisation possible : faire un print, un changement de ref, un truc de type unit... en executant la fonction*)
  (*Source de constante*)
  Printf.printf("Question 6 ok.\n")

(*QUESTION 6 Raffinement 1*)
let genere_compteur () : (unit->int) =
  let a = ref 0 in
  fun _ -> incr a; !a

let () =
  let compteur = genere_compteur () in
  assert(compteur () = 1);
  assert(compteur () = 2);
  Printf.printf "Question 6 raffinement 1 ok.\n"

(*QUESTION 6 Raffinement 2*)
let genere_get_set (a : int) : (unit -> int) * (int->unit) =
  let num = ref a in
  let f1 = fun _ -> !num in
  let f2 = fun x -> num := x in
  f1, f2

let () =
  let get, set = genere_get_set 3 in
  assert (get () = 3);
  set 5;
  assert (get () = 5);
  Printf.printf "Question 6 raffinement 2 ok.\n"

(*QUESTION 6 Raffinement 3*)
let version_memoisee (f : 'a->'b) : ('a->'b) =
  let tab = Hashtbl.create 100 in
  fun x -> 
    if Hashtbl.mem tab x then (
      Printf.printf "\tTrouvé en mémoire : ";
      Hashtbl.find tab x
    )
    else (
      let res = f x in
      Hashtbl.add tab x res;
      res
    )

let () =
  let f = fun x -> x*5 in
  let f_memoisee = version_memoisee f in
  Printf.printf "\tf(%i) = %i\n" 5 (f_memoisee 1);
  Printf.printf "\tf(%i) = %i\n" 5 (f_memoisee 5);
  Printf.printf "\tf(%i) = %i\n" 5 (f_memoisee 5);
  Printf.printf "\tf(%i) = %i\n" 10 (f_memoisee 10);
  Printf.printf "\tf(%i) = %i\n" 1 (f_memoisee 1);
  Printf.printf "Question 6 raffinement 3 ok.\n"


(*QUESTION 7*)
type 'a liste_doublement_chainee = {
  key : 'a;
  mutable previous : 'a liste_doublement_chainee option;
  mutable next : 'a liste_doublement_chainee option
}

let create_doublement (elt : 'a) : 'a liste_doublement_chainee =
  let l = {key = elt; previous = None; next = None} in
  l

let add_elt_doublement (elt : 'a) (l : 'a liste_doublement_chainee) : 'a liste_doublement_chainee =
  let l2 = {key = elt; previous = None; next = Some l} in
  l.previous <- Some l2;
  l2

let pop_doublement (l : 'a liste_doublement_chainee) : 'a * 'a liste_doublement_chainee option =
  let elt = l.key in
  match l.next with
  |None -> elt, None
  |Some x -> x.previous <- None; elt, Some x

let () =
  assert (fst (pop_doublement (add_elt_doublement 1 (add_elt_doublement 2 (create_doublement 3)))) = 1);
  Printf.printf("Question 7 ok.\n")

(*QUESTION 8*)
type 'a node = {
  key : 'a ;
  mutable position_tas_min : int;
  mutable position_tas_max : int
}
type 'a heap = {
  mutable size : int;
  max_size : int;
  mutable tas_min : 'a node array;
  mutable tas_max : 'a node array
}

exception Heap_full
exception Heap_empty

let create_node (elt : 'a) (n : int) : 'a node =
  {key = elt; position_tas_max = n; position_tas_min = n}

let create_heap (elt : 'a) (n : int): 'a heap =
  let node = create_node elt 0 in
  {size = 1 ; max_size = n; tas_min = Array.make n node; tas_max = Array.make n node}


let echanger_elt_min (h: 'a heap) (node1 : 'a node) (node2 : 'a node) : unit = 
  let ind1 = node1.position_tas_min in
  let ind2 = node2.position_tas_min in
  node1.position_tas_min <- ind2;
  node2.position_tas_min <- ind1;
  (h.tas_min).(ind1) <- node2;
  (h.tas_min).(ind2) <- node1;
  (*Et echanger les indices dans tas_max*)
  ((h.tas_max).(node1.position_tas_max)).position_tas_min <- ind2;
  ((h.tas_max).(node2.position_tas_max)).position_tas_min <- ind1

let echanger_elt_max (h: 'a heap) (node1 : 'a node) (node2 : 'a node) : unit = 
  let ind1 = node1.position_tas_max in
  let ind2 = node2.position_tas_max in
  node1.position_tas_max <- ind2;
  node2.position_tas_max <- ind1;
  (h.tas_max).(ind1) <- node2;
  (h.tas_max).(ind2) <- node1;
  (*Et echanger les indices dans tas_min*)
  ((h.tas_min).(node1.position_tas_min)).position_tas_max <- ind2;
  ((h.tas_min).(node2.position_tas_min)).position_tas_max <- ind1

let remonter_elt_tas (h: 'a heap) : unit =
  (*Remonter dans tas_min*)
  let ind = ref (h.size-1) in
  while(!ind>0) do
    let node1 = (h.tas_min).(!ind) in
    let ind_parent = (!ind - 1) / 2 in
    let node2 = (h.tas_min).(ind_parent) in
    if (node1.key < node2.key) then (
      echanger_elt_min h node1 node2;
      ind := ind_parent
    )
    else 
      ind := 0
  done;
  (*Remonter dans tas_max*)
  let ind = ref (h.size-1) in
  while(!ind>0) do
    let node1 = (h.tas_max).(!ind) in
    let ind_parent = (!ind - 1) / 2 in
    let node2 = (h.tas_max).(ind_parent) in
    if (node1.key > node2.key) then (
      echanger_elt_max h node1 node2;
      ind := ind_parent
    )
    else 
      ind := 0
  done

let add_elt_heap (elt : 'a) (h : 'a heap) : unit =
  if h.size = h.max_size then raise Heap_full;
  let node = create_node elt h.size in
  (h.tas_min).(h.size) <- node;
  (h.tas_max).(h.size) <- node;
  h.size <- (h.size + 1);
  remonter_elt_tas h

let descendre_tas_max (n:int) (h:'a heap) : unit =
  let ind = ref n in
  (*Descendre tant qu'il y a deux enfants*)
  while (!ind*2+2<h.size) do
    let ind_f1 = !ind*2+1 in
    let ind_f2 = ind_f1+1 in
    let ind_max = if (h.tas_max.(ind_f1) > h.tas_max.(ind_f2)) then ind_f1 else ind_f2 in
    let node_parent = h.tas_max.(!ind) in
    let node_max_child = h.tas_max.(ind_max) in
    if (node_parent.key >= node_max_child.key) then
      ind := h.size
    else
      echanger_elt_max h node_parent node_max_child
  done;
  (*S'il y a qu'un seul enfant, descendre ou pas un dernier coup*)
  if(!ind*2+2 = h.size) then
    (
      let node_parent = h.tas_max.(!ind) in
      let node_enfant = h.tas_max.(!ind*2+1) in
      if (node_enfant.key > node_parent.key) then echanger_elt_max h node_enfant node_parent
    )

let descendre_tas_min (n:int) (h:'a heap) : unit =
  let ind = ref n in
  (*Descendre tant qu'il y a deux enfants*)
  while (!ind*2+2<h.size) do
    let ind_f1 = !ind*2+1 in
    let ind_f2 = ind_f1+1 in
    let ind_min = if (h.tas_min.(ind_f1) < h.tas_min.(ind_f2)) then ind_f1 else ind_f2 in
    let node_parent = h.tas_min.(!ind) in
    let node_min_child = h.tas_min.(ind_min) in
    if (node_parent.key <= node_min_child.key) then
      ind := h.size
    else
      echanger_elt_min h node_parent node_min_child
  done;
  (*S'il y a qu'un seul enfant, descendre ou pas un dernier coup*)
  if(!ind*2+2 = h.size) then
    (
      let node_parent = h.tas_min.(!ind) in
      let node_enfant = h.tas_min.(!ind*2+1) in
      if (node_enfant.key > node_parent.key) then echanger_elt_max h node_enfant node_parent
    )

(*Pop max, pop min*)
let pop_max (h : 'a heap) : 'a =
  if (h.size = 0) then raise Heap_empty;
  let max_node = (h.tas_max).(0) in
  echanger_elt_max h (h.tas_max.(0)) (h.tas_max.(h.size -1));
  echanger_elt_min h (h.tas_min.(max_node.position_tas_min)) (h.tas_min.(h.size - 1));
  h.size <- h.size - 1;
  (*descendre la racine dans tas_max et l'element echangé dans tas_min*)
  descendre_tas_max 0 h;
  descendre_tas_min (max_node.position_tas_min) h;
  max_node.key

let pop_min (h : 'a heap) : 'a =
  if (h.size = 0) then raise Heap_empty;
  let min_node = (h.tas_min).(0) in
  echanger_elt_min h (h.tas_min.(0)) (h.tas_min.(h.size -1));
  echanger_elt_max h (h.tas_max.(min_node.position_tas_max)) (h.tas_max.(h.size - 1));
  h.size <- h.size - 1;
  (*descendre la racine dans tas_min et l'element echangé dans tas_max*)
  descendre_tas_min 0 h;
  descendre_tas_max (min_node.position_tas_max) h;
  min_node.key

(*Pour debug seulement*)
let print_tas (h:int heap) : unit =
  Printf.printf("Printing heap (tas min) - (tas max) :\n");
  for i = 0 to h.size -1 do
    Printf.printf ("\t%i %i\n") (h.tas_min.(i).key) (h.tas_max.(i).key)
  done

let () =
  let h = create_heap 67 128 in 
  add_elt_heap 1 h;
  add_elt_heap 100 h;
  add_elt_heap 50 h;
  add_elt_heap 500 h;
  add_elt_heap (-5) h;
  add_elt_heap 250 h;
  print_tas h;
  assert (pop_min h = -5);
  assert (pop_max h = 500);
  assert (pop_min h = 1);
  assert (pop_max h = 250);
  assert (pop_min h = 50);
  assert (pop_min h = 67);
  assert (pop_max h = 100);
  Printf.printf("Question 8 ok.\n")