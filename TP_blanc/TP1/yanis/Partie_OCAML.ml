(*Model de N-gramme*)
open Stdlib


(* type enregistrement pour le stockage des n-gramme et des models*)

type n_gramme = {
  caractere : string;
  suivant : string;
  longeur : int;
}

type model_n_gramme = {
  ensemble_n_gramme : n_gramme list;
  longeur : int;
}

type model_ensemble_n_gramme = {
  ensemble : model_n_gramme list; 
}


(* ----- Initialisation d'un n_gramme ----- *)


(* 1. Vérifier si un élément est dans la liste*)
let contient 
(liste_n_gramme_occurence: (string*int) list) 
(elt:string)
: bool =
  List.mem elt (List.map fst liste_n_gramme_occurence)

;;

assert(contient [] "rien" = false);;
assert(contient [("patate",1)] "rien" = false);;
assert(contient [("aga",2);("baa",3)] "agaa" = false);;
assert(contient [("a",2);("b",3)] "a" = true);;


(* 2. Ajoute l'élément en tête s'il n'est pas déjà présent *)
let ajouter_si_absent 
(liste_n_gramme_occurence: (string*int) list) 
(elt:string)
: (string*int) list =
  if List.length liste_n_gramme_occurence = 0 
    || String.length elt = String.length ( fst (List.hd liste_n_gramme_occurence))
  then 
    if contient liste_n_gramme_occurence elt then
      let resultat:(string*int) list = List.map 
        (fun elt_courant -> 
          if (fst elt_courant) = elt 
            then ((fst elt_courant), (+) (snd elt_courant) 1) 
        else elt_courant ) 
       liste_n_gramme_occurence in
      resultat
   else
      (elt,1) :: liste_n_gramme_occurence 
  else 
    liste_n_gramme_occurence
  ;;

assert(ajouter_si_absent [] "a" = [("a",1)]);;
assert(ajouter_si_absent [("a",2)] "aa" = [("a",2)]);;
assert(ajouter_si_absent [("aa",2);("bb",3)] "cc" = [("cc",1);("aa",2);("bb",3)]);;
assert(ajouter_si_absent [("a",2);("b",3)] "b" = [("a",2);("b",4)]);;

let rec ajouter_list_si_absent 
(liste_n_gramme_occurence: (string*int) list) 
(list_element: string list)
: (string*int) list = 
  match list_element with
| [] -> liste_n_gramme_occurence
| elt::r ->  ajouter_list_si_absent (ajouter_si_absent liste_n_gramme_occurence elt) r

;;

assert(ajouter_list_si_absent [] ["a"] = [("a",1)]);;
assert(ajouter_list_si_absent [("a",2);("b",1)] ["a";"b"] =
         [("a",3);("b",2)]);;
assert(ajouter_list_si_absent [("a",1);("b",1)] ["c";"d";"e"] =
         [("e",1);("d",1);("c",1);("a",1);("b",1)]);; 
assert(ajouter_list_si_absent [("a",1);("b",1)] ["b";"c";"d"] =
         [("d",1);("c",1);("a",1);("b",2)]);; 


(* Prend une liste de n_gramme de meme taille et
   crée le dictionnaire d'occurence des n_gramme suivant *)
let cree_dictionnaire_occurence 
(liste_n_gramme_suivant: string list) 
: (string*int) list =
  let list_n_gramme_occurence:(string*int) list =
    ajouter_list_si_absent [] liste_n_gramme_suivant 
in list_n_gramme_occurence

;;

assert(cree_dictionnaire_occurence ["a"] = [("a",1)]);;
assert(cree_dictionnaire_occurence ["a";"b";"a";"b"] =
         [("b",2);("a",2)]);;
assert(cree_dictionnaire_occurence ["c";"d";"e";"a";"c";"a"] =
         [("a",2);("e",1);("d",1);("c",2)]);;
assert(cree_dictionnaire_occurence ["b";"c";"d"] =
         [("d",1);("c",1);("b",1)]);; 


(* Prend une liste d'occurence
   Retourne la chaine de caratére avec le plus grand nombre d'occurence*)
let max_occurence_n_gramme 
(liste_n_gramme_occurence: (string*int) list) 
: string = 
  if liste_n_gramme_occurence = [] then 
    ""
  else
    let max_occurence: int ref = ref 0 in 
    let rec trouver_max 
    (liste_n_gramme_occurence: (string*int) list) 
    (suivant: string)
    : string = 
      match liste_n_gramme_occurence with
      | [] -> suivant
      | elt::rest -> if (snd elt) > !max_occurence 
          then 
            begin 
            max_occurence := snd elt;
            trouver_max rest (fst elt)
          end
        else trouver_max rest suivant
    in
    trouver_max liste_n_gramme_occurence ""
  
;;

assert(max_occurence_n_gramme [] = "");;
assert(max_occurence_n_gramme [("a",1)] = "a");;
assert(max_occurence_n_gramme [("a",2);("b",2)] = "a");;
assert(max_occurence_n_gramme [("c",2);("d",1);("e",1);("a",3)] = "a");; 
assert(max_occurence_n_gramme  [("bb",1);("cd",1);("dd",1)] = "bb");; 

(*Initialise un n-gramme a partir de la chaine de caractére et
  de la liste de n_gramme suivant*)
let initialise_n_gramme 
(taille_n_gramme:int) 
(chaine:string) 
(list_n_gramme_suivant: string list)
: n_gramme =
  let list_occurence : (string*int) list =
    cree_dictionnaire_occurence list_n_gramme_suivant in
  let chaine_suivant : string =
    max_occurence_n_gramme list_occurence in 
  let n_gramme_courant : n_gramme = {caractere = chaine;
                                     suivant = chaine_suivant;
                                     longeur = taille_n_gramme} in 
  n_gramme_courant

;;

assert(initialise_n_gramme 1 "p" ["a"] =
         {caractere = "p";
          suivant = "a";
          longeur = 1});;
assert(initialise_n_gramme 2 "ta" ["pa";"ta";"ta";"b "] =
         {caractere = "ta";
          suivant = "ta";
          longeur = 2});;


(*
(* Initialise un model de n-gramme pour une taille de n-gramme donné *)
let split_text_n_gramme 
(texte:string) 
(taille_n_gramme:int) 
: model_n_gramme = 
  let tableau_sous_mot:string array = Array.init 
  (String.length texte) 
  (fun i -> String.sub texte (i) (min taille_n_gramme ((String.length texte) - i))) in
  let list_n_gramme:n_gramme list = List.init 
  (Array.length tableau_sous_mot) 
  (fun i -> 
    if not (Array.mem tableau_sous_mot.(i) (Array.sub tableau_sous_mot 0 i)) then
      let list_sous_mot_suivant:string list = List.init (Array.length tableau_sous_mot) 
        (fun j -> if tableau_sous_mot.(j) = tableau_sous_mot.(i) && j < (Array.length tableau_sous_mot) - 1 then
            tableau_sous_mot.(j+1) else "") in 
      (initialise_n_gramme taille_n_gramme tableau_sous_mot.(i) list_sous_mot_suivant)
        else 
          let empty_n_gramme: n_gramme = {caractere = ""; suivant =""; longeur = 0} in 
          empty_n_gramme) in
  let resultat:model_n_gramme = {ensemble_n_gramme = list_n_gramme; longeur = taille_n_gramme} in
  resultat

;;

let text_test:string = "Bonjour" in 
begin
  assert(split_text_n_gramme "" 1 = {ensemble_n_gramme = []; longeur = 1});
  assert(split_text_n_gramme "le le le l" 2 = {ensemble_n_gramme = [
    {caractere="le"; suivant = " l"; longeur = 2}; 
    {caractere="e "; suivant = "le"; longeur = 2};
    {caractere=" l"; suivant = "e "; longeur = 2};
    ]; longeur = 2});
  assert(split_text_n_gramme text_test 3 = {ensemble_n_gramme = [
    {caractere="Bon"; suivant = "jou"; longeur = 3};
    {caractere="onj"; suivant = "our"; longeur = 3};
    {caractere="njo"; suivant = "ur"; longeur = 3};
    {caractere="jou"; suivant = "r ";longeur = 3};
    {caractere="our "; suivant = ""; longeur = 3};
    ]; longeur = 3})
end

*)

let test = 
  let list_n_gramme = ["a";"b";"c";"a";"d";"e";"c";"c"] in 
  let dico_occ = cree_dictionnaire_occurence list_n_gramme in
  List.iter (fun elt -> Printf.printf "%d %s, " (snd elt) (fst elt)) dico_occ;
  Printf.printf "\n";
  let n_gramme_suivant = max_occurence_n_gramme dico_occ in
  Printf.printf "%s" n_gramme_suivant
