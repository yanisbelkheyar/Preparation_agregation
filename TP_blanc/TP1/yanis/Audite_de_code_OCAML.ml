(* man pstree *)
open Printf

type t = N of string * t list

(* Correction : manque de lisibilité dans le code,
   manque de commentaire et de variable explicite
 Aucun test ni description de fonction *)

(* Fonctionalité : Affichage récursife de structure arboresante*)

(* Fonction print1 parcour récursivement les différent élément de la structure, 
   si un même niveau d'arboressance contient plusieurs éléments
   alors appelle récursif a print2*)

let rec print1 pref (N (s, tl)) =
  printf "%s" s; (*affiche s*) 
  if tl <> [] then (* si tl est different de la liste vide *)
    let w = String.length s in (*w prend la taille de la chaine s*)
    (*pref' n'est pas un caractére autorisé de même que t'
    let pref’ = pref ^ String.make w ’ ’ in
    match tl with
    | [t’] -> printf "---"; print1 (pref’ ^ "␣␣") t’
    | _ -> printf "-"; print2 pref’ "+-" tl
     *)
    let pref = pref ^ String.make w ' ' in 
      match tl with 
    | [t] -> printf "---"; print1 (pref ^ "␣␣") t  
    | _ -> printf "-"; print2 pref "+-" tl  
and print2 pref start = function
  | [] -> printf ""
  | [s] ->
     printf "‘-"; print1 (pref ^ "␣␣") s
  | s :: sons ->
     printf "%s" start; print1 (pref ^ "|␣") s;
     printf "\n"; printf "%s" pref;
     print2 pref "|-" sons

let print t =
  print1 "" t




(* -----  Alternative aux fonctions initial  ------*)

(*Fonction d´affichage des l´arborescense recursive*)
let rec print_arbrorescence
          (prefix:string)
          (N ((label:string), (rest_arbo:t list))) =
  printf "%s" label; (*affiche s*) 
  if rest_arbo <> [] then (* si tl est different de la liste vide *)
    let taille_chaine:int = String.length label in
    let prefix:string = prefix ^ String.make taille_chaine ' ' in 
      match rest_arbo with 
    | [t] -> printf "---"; print_arbrorescence (prefix ^ "␣␣") t  
    | _ -> printf "-"; print2 prefix "+-" rest_arbo  
else
  printf "\n"

(* Fonction d´affichage pour le cas de plusieurs éléments a un même niveau
   de l'arborescence*)
and print_element_meme_niveau
(prefix:string)
(debut:string) = function
  | [] -> printf ""
  | [s] ->
     printf "‘-"; print_arbrorescence (prefix ^ "␣␣") s
  | s :: enfant ->
     printf "%s" debut; print_arbrorescence (prefix ^ "|␣") s;
     printf "\n"; printf "%s" prefix;
     print_element_meme_niveau prefix "|-" enfant


(* Test *)
let arbre_vide = N ("", []) 
let () = print_arbrorescence "" arbre_vide

let () = printf "\n"

let arbre_profondeur_2 = N ("1", [N ("2", [])])
let () = print_arbrorescence "" arbre_profondeur_2

let () = printf "\n\n"
let arbre_multiple_noeud_meme_profondeur = N ("1", [
      N ("2", [
        N ("3",[]); 
        N("3.1",[]); 
        N ("3.2",[])])])
let () = print_arbrorescence "" arbre_multiple_noeud_meme_profondeur

let () = printf "\n\n"
let arbre_multiple_noeud_meme_profondeur_1 = N ("1", [
      N ("2", [
        N ("3",[]); 
        N("3.1",[]); 
        N ("3.2",[])]);
      N ("2.1", [
         N ("3.1.1",[]); 
        N("3.1.2",[]); 
        N ("3.1.3",[])]);
      ])

let () = print_arbrorescence "" arbre_multiple_noeud_meme_profondeur_1
