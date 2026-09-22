exception Tableau_vide;;

let min_tab (t: int array): int =
	(* Pour t un tableau d'entiers, renvoie le minimum de t
	Renvoie l'exception Tableau_vide si le tableau est vide *)
	if t = [||] then raise Tableau_vide
	else let min = ref t.(0) in
	for i = 1 to Array.length t - 1 do
		if t.(i) < !min then
			min := t.(i)
	done;
	!min;;

assert (min_tab [|1; 2; 4; 5|] = 1);;
assert (min_tab [|2; 5; 3; 4|] = 2);;
assert (min_tab [|5|] = 5);;

let sum_tab (t: int array): int =
	(* Pour t un tableau d'entiers, renvoie la somme des éléments de t
	Renvoie l'exception Tableau_vide si le tableau est vide *)
	let sum = ref 0 in
	for i = 0 to Array.length t - 1 do
		sum := !sum + t.(i)
	done;
	!sum;;

assert (sum_tab [|1; 2; 4; 5|] = 12);;
assert (sum_tab [|2; 5; 3; 4|] = 14);;
assert (sum_tab [|5|] = 5);;

let is_sorted (t: int array): bool =
	(* Pour t un tableau d'entiers, renvoie true ssi t est trié *)
	match t with
	| [||] -> true
	| _ -> Array.fold_left
		(fun (x, sorted) y -> (y, sorted && x <= y)) (t.(0), true) t |> snd ;;

assert (is_sorted [|1; 2; 4; 5|] = true);;
assert (is_sorted [|2; 5; 3; 4|] = false);;
assert (is_sorted [|5|] = true);;
assert (is_sorted [||] = true);;

let tab_to_list_rec (t: 'a array): 'a list =
	(* Pour t un tableau, renvoie t sous forme de liste *)
	let rec tab_to_list_rec_aux (i: int): int list =
		if i = Array.length t then []
		else (t.(i))::(tab_to_list_rec_aux (i+1))
	in tab_to_list_rec_aux 0 ;;

let tab_to_list_imp (t: 'a array): 'a list =
	(* Pour t un tableau, renvoie t sous forme de liste *)
	let res = ref [] in
	for i = Array.length t - 1 downto 0 do
		res := (t.(i))::(!(res))
	done;
	!res ;;

assert (tab_to_list_rec [|1; 2; 4; 5|] = [1; 2; 4; 5]);;
assert (tab_to_list_imp [|1; 2; 4; 5|] = [1; 2; 4; 5]);;
assert (tab_to_list_rec [|5|] = [5]);;
assert (tab_to_list_imp [|5|] = [5]);;
assert (tab_to_list_rec [||] = []);;
assert (tab_to_list_imp [||] = []);;

let dichotomie_rec (p: int) (t: int array): bool =
	(* Renvoie true ssi p entier est présent dans
	t tableau d'entiers trié *)
	let rec dichotomie_rec_aux (d: int) (f: int) =
		(* Applique l'étape de dichotomie entre deux indices d et f *)
		if d > f then false
		else
			let m = (d+f)/2 in
			if t.(m) = p then true
			else if t.(m) < p
				then dichotomie_rec_aux (m+1) f
				else dichotomie_rec_aux d (m-1)
	in dichotomie_rec_aux 0 (Array.length t - 1);;

exception Found

let dichotomie_imp (p: int) (t: int array): bool =
	(* Renvoie true ssi p entier est présent dans
	t tableau d'entiers trié *)
  	let d = ref 0 in
  	let f = ref (Array.length t - 1) in
  	try
  	  	while !d <= !f do
  	  	  	let m = (!d + !f) / 2 in
  	  	  	if t.(m) = p then raise Found
  	  	  	else if t.(m) < p then d := m+1
  	  	  	else f := m-1
  	  	done;
  	  false
  	with Found -> true;;

assert (dichotomie_rec 1 [|1; 2; 4; 5|] = true);;
assert (dichotomie_rec 3 [|1; 2; 4; 5|] = false);;
assert (dichotomie_imp 1 [|1; 2; 4; 5|] = true);;
assert (dichotomie_imp 3 [|1; 2; 4; 5|] = false);;
assert (dichotomie_rec 1 [|1|] = true);;
assert (dichotomie_rec 3 [|1|] = false);;
assert (dichotomie_imp 1 [|1|] = true);;
assert (dichotomie_imp 3 [|1|] = false);;
assert (dichotomie_rec 4 [||] = false);;
assert (dichotomie_imp 4 [||] = false);;

let factorielle_imp (n: int): int =
	(* Renvoie n! *)
	let res = ref 1 in
	for i = 2 to n do
		res := (!res) * i
	done;
	!res

let rec factorielle_rec (n: int): int =
	(* Renvoie n! *)
	if n = 0 then 1
	else n*(factorielle_rec (n-1))

let decomposition (n: int): int list =
	(* Pour n >= 1, renvoie la liste des facteurs premiers de n,
	   avec répétition et par ordre croissant *)
	let rec decomposition_aux (n: int) (d: int): int list =
		(* Renvoie la liste des diviseurs de n, sachant que
		n n'est divisible par aucun entier inférieur à d *)
		if n = 1 then []
		else if d*d > n then [n]
		else if n mod d = 0 then d::(decomposition_aux (n/d) d)
		else decomposition_aux n (d+1)
	in decomposition_aux n 2;;

assert (decomposition 12 = [2; 2; 3]);;
assert (decomposition 60 = [2; 2; 3; 5]);;
assert (decomposition 97 = [97]);;
assert (decomposition 1 = []);;