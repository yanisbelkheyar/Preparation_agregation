exception Tableau_vide

let min_tab (t: int array): int =
	(* Pour t un tableau d'entiers, renvoie le minimum de t
	Renvoie l'exception Tableau_vide si le tableau est vide *)
	if t = [||] then raise Tableau_vide
	else Array.fold_left (fun x y -> if x < y then x else y) t.(0) t 

let () = assert (min_tab [|1; 2; 4; 5|] = 1);
assert (min_tab [|2; 5; 3; 4|] = 2);
assert (min_tab [|5|] = 5)

let sum_tab (t: int array): int =
	(* Pour t un tableau d'entiers, renvoie la somme des éléments de t
	Renvoie l'exception Tableau_vide si le tableau est vide *)
	Array.fold_left (+) 0 t

let () = assert (sum_tab [|1; 2; 4; 5|] = 12);
assert (sum_tab [|2; 5; 3; 4|] = 14);
assert (sum_tab [|5|] = 5)

let is_sorted (t: int array): bool =
	(* Pour t un tableau d'entiers, renvoie true ssi t est trié *)
	match t with
	| [||] -> true
	| _ -> Array.fold_left
		(fun (x, sorted) y -> (y, sorted && x <= y)) (t.(0), true) t |> snd 

let () = assert (is_sorted [|1; 2; 4; 5|] = true);
assert (is_sorted [|2; 5; 3; 4|] = false);
assert (is_sorted [|5|] = true);
assert (is_sorted [||] = true)

let tab_to_list (t: 'a array): 'a list =
	(* Renvoie le tableau t sous forme de liste *)
	Array.fold_right List.cons t [] 

let list_to_tab (l: 'a list): 'a array =
	(* Renvoie la liste l sous forme de tableau *)
	if l = [] then [||] else
	let t = Array.make (List.length l) (List.hd l) in
	List.iteri (fun i x -> t.(i) <- x) l; t

let () = assert (tab_to_list [|5|] = [5]);
assert (tab_to_list [||] = []);
assert (list_to_tab [1; 2; 4; 5] = [|1; 2; 4; 5|]);
assert (list_to_tab [5] = [|5|]);
assert (list_to_tab [] = [||])

let incr_tab (n: int) (l: int array): int array =
	Array.map (fun x -> x + n) l 

let () = assert (incr_tab 4 [|2; 5; 3; 4|] = [|6; 9; 7; 8|]);
assert (incr_tab 0 [|5|] = [|5|])

let rec itere (f: 'a -> 'a) (n: int): 'a -> 'a =
	match n with
	| 0 -> (fun x -> x)
	| _ -> (fun x -> f (itere f (n-1) x)) 

let () = assert (itere (fun x -> x+1) 8 3 = 11);
assert (itere (fun x -> x+1) 0 3 = 3)

let teste_preds (l: ('a -> bool) list) (x: 'a): bool list =
	List.map (fun f -> f x) l 

let () = assert (teste_preds [(fun x -> x = 2); (fun x -> x = 3); (fun x -> x = 4)] 3 = [false; true; false]);
assert (teste_preds [] 3 = [])

let rec sum (i: int) (j: int) (f: int -> int): int =
	if i > j then 0
	else (f i) + sum (i+1) j f 

let () = assert (sum 1 6 (fun x -> 2 * x) = 6 * 7);
assert (sum 8 8 (fun x -> x * x) = 64);
assert (sum 8 6 (fun x -> 2 * x) = 0) 

let rec deffer (x: 'a): (unit -> 'a) =
	(fun () -> x)
(* On crée une source de donnée constante (source de type unit -> int) *)

(* Liste doublement chainée *)
type 'a node = {
  value : 'a;
  mutable prev : 'a node option;
  mutable next : 'a node option;
}

type 'a ldc = {
  mutable head : 'a node option;
  mutable tail : 'a node option;
}

(* Créer une liste vide *)
let create (): 'a ldc =
	{ head = None; tail = None } 

(* Vérifier si la liste est vide *)
let is_empty (l: 'a ldc): bool =
	l.head = None 

(* Insérer au début *)
let push (l: 'a ldc) (x: 'a): unit =
  	let new_node = { value = x; prev = None; next = l.head } in
  	begin match l.head with
  	| None ->
  		l.tail <- Some new_node
  	| Some old_head ->
  		old_head.prev <- Some new_node
  	end;
 	l.head <- Some new_node 

(* Insérer à la fin *)
let append (l: 'a ldc) (x: 'a): unit =
  	let new_node = { value = x; prev = l.tail; next = None } in
  	begin match l.tail with
  	| None ->
  		l.head <- Some new_node
  	| Some old_tail ->
    	old_tail.next <- Some new_node
    end;
    l.tail <- Some new_node 

(* Supprimer et renvoyer le premier élément *)
let pop_front (l: 'a ldc): 'a option =
  	match l.head with
  	| None -> None
  	| Some old_head ->
  		l.head <- old_head.next;
  		(match l.head with
  		| None -> l.tail <- None (* La liste est devenue vide *)
  		| Some new_head -> new_head.prev <- None);
  		Some old_head.value

(* Supprimer et renvoyer le dernier élément *)
let pop_back (l: 'a ldc): 'a option =
  	match l.tail with
  	| None -> None
  	| Some old_tail ->
  		l.tail <- old_tail.prev;
  		(match l.tail with
  		| None -> l.head <- None (* La liste est devenue vide *)
  		| Some new_tail -> new_tail.next <- None);
  		Some old_tail.value

(* Convertir en liste standard *)
let to_list (l: 'a ldc): 'a list =
  let rec loop (current: 'a node option) (acc: 'a list): 'a list =
    match current with
    | None -> List.rev acc
    | Some node -> loop node.next (node.value::acc)
  in loop l.head []