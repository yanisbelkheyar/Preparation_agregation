(* man pstree *)

open Printf

type t = N of string * t list

let rec print1 pref (N (s, tl)) =
  printf "%s" s;
  if tl <> [] then
  let w = String.length s in
  let pref' = pref ^ String.make w ' ' in
  match tl with
  | [t'] -> printf "---"; print1 (pref' ^ "␣␣") t'
  | _ -> printf "-"; print2 pref' "+-" tl

and print2 pref start = function
 | [s] ->
  printf "'-"; print1 (pref ^ "␣␣") s
 | s :: sons ->
  printf "%s" start; print1 (pref ^ "|␣") s;
  printf "\n"; printf "%s" pref;
  print2 pref "|-" sons

 let old_print t =
  print1 "" t

(* Problème 1 (correction): off-by-one, pref' devrait avoir un espace extra pour aligner les | avec le +
   Probleme 2 (lisibilite): noms de fonctions + absence de commentaires ! 

   Problème 3 (style): if tl<>[] au lieu d'un cas de plus dans le pattern match
   Problème 3bis: manque d'indentation pour le if
   Problème 4: pas de tests
   Problème 5: warnings de pattern match non-exhaustif
   Autre changement: ajout d'un \n à ce qui est imprimé pour la lisibilité des tests *)

let rec print_tree pref (N (s, tl)) : unit =
  printf "%s" s;
  let w = String.length s in
  (* w + 1 pour correspondre à s et au premier '-' dans "-+-" *)
  let pref' = pref ^ String.make (w + 1) ' ' in
  match tl with
  | [] -> ()
  | [t'] -> printf "---"; print_tree (pref' ^ "␣␣") t'
  | _ -> printf "-"; print_tree_level pref' "+-" tl

and print_tree_level pref start = function
  | [] -> assert(false)
  | [s] ->
    printf "'-"; print_tree (pref ^ "␣␣") s
  | s :: sons ->
    printf "%s" start; print_tree (pref ^ "|␣") s;
    printf "\n"; printf "%s" pref;
    print_tree_level pref "|-" sons

let print t =
  print_tree "" t;
  printf "\n"

let tests () =
  print (N ("abc", []));
  print (N ("abc", [N ("def", [])]));
  print (N ("abc", [N ("def", []); N ("zy", [])]));
  print (N ("abc", [N ("def", []); N ("zy", []); N("r", [])]));
  print (N ("abc", []));
  print (N ("abc", [N ("def", [N ("f", [])])]));
