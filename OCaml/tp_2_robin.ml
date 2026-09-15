(* Précondition: `arr` un tableau non-vide d'entiers 
 * Renvoie le plus petit élément présent dans le tableau *)
let plus_petit_entier (arr: int array): int =
  let l : int = Array.length arr in
  if l = 0 then
    failwith "tableau vide";
  let minimum : int ref = ref (arr.(0)) in
  for i = 1 to l - 1 do
    if arr.(i) < !minimum then
      minimum := arr.(i)
  done ;
  !minimum
;;

assert((plus_petit_entier [|42|]) = 42);;
assert((plus_petit_entier [|0; 1; 2|]) = 0);;
assert((plus_petit_entier [|2; 1; 0|]) = 0);;
assert((plus_petit_entier [|41; 42; 40; 41; 40; 42|]) = 40);;

(* Précondition: `arr` un tableau non vide de flottants non-NaN
 * renvoie le plus petit élément présent dans le tableau *)
let plus_petit_flottant (arr: float array): float =
  let l : int = Array.length arr in
  if l = 0 then
    failwith "tableau vide";
  let minimum : float ref = ref (arr.(0)) in
  for i = 1 to l - 1 do
    if arr.(i) < !minimum then
      minimum := arr.(i)
  done ;
  !minimum
;;

assert((plus_petit_flottant[|0.5|]) = 0.5);;
assert((plus_petit_flottant[|0.; 1.; 2.3|]) = 0.);;
assert((plus_petit_flottant[|0.6; 0.56; 0.5|]) = 0.5);;
assert((plus_petit_flottant[|41.; 42.3; 40.1; 41.2; 40.1; 42.3|]) = 40.1);;

let somme_tableau (arr: int array) =
  let resultat = ref 0 in
  for i = 0 to (Array.length arr) - 1 do
    resultat := !resultat + arr.(i)
  done;
  !resultat
;;

assert((somme_tableau [||]) = 0);;
assert((somme_tableau [|42|]) = 42);;
assert((somme_tableau [|2; 2|]) = 4);;
assert((somme_tableau [|2; -3; 0|]) = -1);;

exception NonTrie;;
(* arr: un tableau d'éléments comparable par `>`, pour lesquels il définit un ordre total
 * Renvoie true s'il est trié de façon croissante, faux autrement *)
let est_trie_croissant (arr: 'a array): bool =
  let l : int = Array.length arr in
  try 
    for i = 1 to l - 1 do
      if arr.(i-1) > arr.(i) then
        raise NonTrie
    done;
    true
  with
  | NonTrie -> false
;;

assert(est_trie_croissant([||]) = true);;
assert(est_trie_croissant([|42|]) = true);;
assert(est_trie_croissant([|0; 1; 2|]) = true);;
assert(est_trie_croissant([|2; 1; 0|]) = false);;
assert(est_trie_croissant([|2; 1; 3|]) = false);;
assert(est_trie_croissant([|0.; 0.5; 0.5; 2014.32|]) = true);;
assert(est_trie_croissant([|40; 40|]) = true);;
assert(est_trie_croissant([|40; 41; 41; 42|]) = true);;

type ordreDirection = Croissant | Decroissant | Constant;;
(* arr: un tableau d'éléments comparable par `>`, pour lesquels il définit un ordre total
 * Renvoie true s'il est trié de façon soit croissante soit décroissante *)
let est_trie (arr: 'a array): bool =
  let l : int = Array.length arr in
  let direction : ordreDirection ref = ref Constant in
  try
    for i = 1 to l - 1 do
      if arr.(i) > arr.(i-1) then
        begin if !direction = Decroissant then
          raise NonTrie
        else
          direction := Croissant
        end
      else if arr.(i) < arr.(i-1) then
        begin if !direction = Croissant then
          raise NonTrie
        else
          direction := Decroissant
        end
    done;
    true
  with
  | NonTrie -> false
;;

assert(est_trie([||]) = true);;
assert(est_trie([|42|]) = true);;
assert(est_trie([|0; 1; 2|]) = true);;
assert(est_trie([|2; 1; 0|]) = true);;
assert(est_trie([|40; 40|]) = true);;
assert(est_trie([|0; 0; 0; 1; 2|]) = true);;
assert(est_trie([|0; 0; 0; 1; 2; 1|]) = false);;
assert(est_trie([|0; 1; 1; 2|]) = true);
assert(est_trie([|2; 1; 3|]) = false);;
assert(est_trie([|0.; 0.5; 0.5; 2014.32|]) = true);;

let tableau_vers_liste_iter (arr: 'a array) : 'a list =
  let resultat : 'a list ref = ref [] in
  for i = (Array.length arr) -  1 downto 0 do
    resultat := arr.(i) :: !resultat
  done;
  !resultat
;;

assert(tableau_vers_liste_iter([||]) = []);;
assert(tableau_vers_liste_iter([|0.3|]) = [0.3]);;
assert(tableau_vers_liste_iter([|0; 1; 2; 3|]) = [0; 1; 2; 3]);;

let tableau_vers_liste_recursif (arr: 'a array) :  'a list =
  let rec tableau_prefixe_vers_liste (i: int) (acc: 'a list): 'a list =
    if i <= 0 then
      acc
    else
      tableau_prefixe_vers_liste (i - 1) (arr.(i - 1) :: acc)
  in
  tableau_prefixe_vers_liste (Array.length arr) []
;;

assert(tableau_vers_liste_recursif([||]) = []);;
assert(tableau_vers_liste_recursif([|0.3|]) = [0.3]);;
assert(tableau_vers_liste_recursif([|0; 1; 2; 3|]) = [0; 1; 2; 3]);;

let liste_vers_tableau_iteratif (l: 'a list) : 'a array =
  let n : int = List.length l in
  let resultat : 'a array = Array.init n (fun _ -> List.hd l) in
  let list_tmp : 'a list ref = ref l in
  for i = 0 to n - 1 do
    match !list_tmp with
    | [] -> failwith "la liste n'a pas la taille attendue"
    | x :: xs -> begin
      resultat.(i) <- x;
      list_tmp := xs
      end
  done;
  assert(!list_tmp = []);
  resultat
;;

assert(liste_vers_tableau_iteratif([]) = [||]);;
assert(liste_vers_tableau_iteratif([0]) = [|0|]);;
assert(liste_vers_tableau_iteratif(['a'; 'b'; 'c']) = [|'a'; 'b'; 'c'|]);;

let liste_vers_tableau_recursif (l: 'a list) : 'a array =
  let n : int = List.length l in
  let resultat : 'a array = Array.init n (fun _ -> List.hd l) in
  let rec liste_vers_tableau_suffixe (l_inner: 'a list) (i: int) =
    match l_inner with
    | [] -> ()
    | x :: xs -> begin
      resultat.(i) <- x;
      liste_vers_tableau_suffixe xs (i + 1)
    end
  in
  liste_vers_tableau_suffixe l 0;
  resultat
;;

assert(liste_vers_tableau_recursif([]) = [||]);;
assert(liste_vers_tableau_recursif([0]) = [|0|]);;
assert(liste_vers_tableau_recursif(['a'; 'b'; 'c']) = [|'a'; 'b'; 'c'|]);;

(* arr est un tableau trié de façon croissante
 * Si elem est présent dans arr, renvoie Some i tel que arr.(i) = elem
 * Autrement renvoie None *)
let recherche_dichotomique_rec (elem: 'a) (arr: 'a array) : int option =
  (* recherche dans arr[i..(j-1)] *)
  let rec recherche_intervalle (i: int) (j: int) =
    if i >= j then
      None
    else
      let mid = i + (j - i) / 2 in
      if arr.(mid) = elem then
        Some mid
      else if arr.(mid) < elem then
        recherche_intervalle (mid + 1) j
      else
        begin
          assert(arr.(mid) > elem);
          recherche_intervalle i mid
        end
  in
  recherche_intervalle 0 (Array.length arr)
;;

assert(recherche_dichotomique_rec 42 [|0; 1; 2; 42|] = Some 3);;
assert(recherche_dichotomique_rec 42 [|0; 1; 42; 43; 45|] = Some 2);;
assert(recherche_dichotomique_rec 42 [|0; 1; 2; 3|] = None);;
assert(recherche_dichotomique_rec 42 [|0; 15; 30; 45|] = None);;
assert(recherche_dichotomique_rec 42 [|0|] = None);;
assert(recherche_dichotomique_rec 42 [|42|] = Some 0);;
assert(recherche_dichotomique_rec 42 [||] = None);;

exception Trouve of int;;
let recherche_dichotomique_iter (elem: 'a) (arr: 'a array) : int option =
  let low = ref 0 in
  let high = ref (Array.length arr) in
  try
    while !low < !high do
      let mid = !low + (!high - !low) / 2 in
      if arr.(mid) = elem then
        raise (Trouve mid)
      else if arr.(mid) < elem then
        low := mid + 1
      else begin
        assert(arr.(mid) > elem);
        high := mid
      end
    done;
    None
  with
  | Trouve i -> Some i
;;

assert(recherche_dichotomique_iter 42 [|0; 1; 2; 42|] = Some 3);;
assert(recherche_dichotomique_iter 42 [|0; 1; 42; 43; 45|] = Some 2);;
assert(recherche_dichotomique_iter 42 [|0; 1; 2; 3|] = None);;
assert(recherche_dichotomique_iter 42 [|0; 15; 30; 45|] = None);;
assert(recherche_dichotomique_iter 42 [|0|] = None);;
assert(recherche_dichotomique_iter 42 [|42|] = Some 0);;
assert(recherche_dichotomique_iter 42 [||] = None);;

(* Précondition: n doit être un entier positif, <= 20 *)
let factorielle_rec (n: int): int =
  let rec factorielle_acc (acc : int) (i: int) =
    if i <= 1 then
      acc
    else
      factorielle_acc (acc * i) (i - 1)
  in
  factorielle_acc 1 n
;;

(* Précondition: n doit être un entier positif, <= 20 *)
let factorielle_iter (n: int): int =
  let resultat : int ref = ref 1 in
  let i : int ref = ref n in
  while !i > 1 do
    resultat := !i * !resultat;
    decr i
  done;
  !resultat
;;

assert(factorielle_rec 0 = factorielle_iter 0);;
assert(factorielle_rec 1 = factorielle_iter 1);;
assert(factorielle_rec 2 = factorielle_iter 2);;
assert(factorielle_rec 5 = factorielle_iter 5);;

(* for i = 0 to 100 do
  Printf.printf "i=%d, rec=%d, iter=%d\n" i (factorielle_rec i) (factorielle_iter i)
done;; *)

(* Précondition: n doit être un entier >= 2
 * Renvoie une liste des facteurs premiers de n
 * Note: la complexité pourrait être améliorée en ne testant que les nombres premiers *)
let rec decomposer_entier (n: int): int list =
  try
    let i = ref 2 in
    while !i * !i <= n do
      if n mod !i = 0 then
        raise (Trouve !i)
      else
        incr i
    done;
    [n]
  with
  | Trouve i -> i :: decomposer_entier (n / i)
;;

assert(decomposer_entier 2 = [2]);;
assert(decomposer_entier 3 = [3]);;
assert(decomposer_entier 4 = [2; 2]);;
assert(decomposer_entier 5 = [5]);;
assert(decomposer_entier 30 = [2; 3; 5]);;