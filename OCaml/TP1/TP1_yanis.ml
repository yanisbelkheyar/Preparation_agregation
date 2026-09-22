(* Question 1 : chercher le minimum d'un tableau*) 

let rec minlist = function
| [] -> None
| [x] -> x
| x::r -> min x (minlist r);;

let rec mintab tab = minlist (Array.to_list tab);;

(* Test : test de recherche de min*)

let test_min = 
    assert (mintab (Array.init 10 (fun x -> Some x)) = Some 0);
    assert (mintab [||] == None);
    assert (mintab (Array.init 10 (fun x -> Some (-x))) = Some (-9));;


(* Question 2 : faire la somme des valeur d'un tableau*)

let rec somme_list = function
| [] -> 0
| x::r-> (somme_list r) + x;;

let somme_tab tab = somme_list (Array.to_list tab);;

(* Test : creation de tableau + test de somme*)
let test_min = 
    assert (somme_tab (Array.init 10 (fun x -> x)) = 9*5);
    assert (somme_tab [||] == 0);
    assert (somme_tab (Array.init 10 (fun x -> (-x))) = (-9*5));;

(* Question 3 : Vérifier si un tableau d'entier est trié*)

let rec verifie_trier = function
| [] | [_] -> true
| x::(y::_ as r) ->  x<=y && verifie_trier r  

let verifie_tab tab = verifie_trier (Array.to_list tab)


(* Test de verification*)
let test_min = 
    assert (verifie_tab (Array.init 10 (fun x -> x)) == true);
    assert (verifie_tab [||] == true);
    assert (verifie_tab (Array.init 10 (fun x -> (x mod 3))) == false);
    assert (verifie_tab (Array.init 10 (fun x -> (-x))) == false);;


(* Question 4 : convertir un tableau en liste, une liste en tableau*)

(*TODO*)

(* Question 5 : Faire une recherche dichotomique dans un tableau trié*)

let rec dichotomique tableau element gauche droite = 
    if droite < gauche then
        false
    (*Si il ne rest qu'un seul element possible alors on renvoi si il est egale a l'élément rechercher*)
    else if droite = gauche  then 
        element = tableau.(gauche)
    else
    let moitie = (droite + gauche) / 2 in
        if element < tableau.(moitie) then 
            dichotomique tableau element gauche (moitie-1)
        else if element > tableau.(moitie) then 
            dichotomique tableau element (moitie+1) droite 
        else
            true

let dichotomique_rec tableau element = dichotomique tableau element 0 ((Array.length tableau) - 1);;

(* Faire des test en utilisant les assert*)


let test_min = 
    assert (dichotomique_rec (Array.init 10 (fun x -> x)) 5 == true);
    assert (dichotomique_rec [||] 1 == false);
    assert (dichotomique_rec (Array.init 10 (fun x -> (x mod 3))) 2 == true); 
    assert (dichotomique_rec (Array.init 10 (fun x -> (-x))) (-4) == true);;

(* Question 6 : Calculer la fonction factorielle*)


let rec factoriel x = if x > 1 then x * factoriel (x-1) else 1

(* Test en utilisant des assert*)
let test_factoriel = 
    assert (factoriel 3 = 6);
    assert (factoriel 10 = (2*3*4*5*6*7*8*9*10));
    assert (factoriel 1 = 1);
    assert (factoriel (-1) = 1);;

(* Question 7 : Calculer la décomposition en nombre premier*)

let factoriser x = let rec divisor x y = if (x mod y) = 0 then (print_int y; y:: divisor (x/y) (y+1)) else if y>=x then [] else divisor x (y+1) in divisor x 1;;

(* Test en utilisant des assert*)

let print_list l = let rec print_elt = function
    | [] -> print_newline ()
    | [x] -> print_int x
    | x::r -> print_int x
in print_elt l; print_newline();;


let test_factoriser = 
    assert (factoriser 3 = [1;3]);
    assert (factoriser 10 = [1;2;5]);
    assert (factoriser 1 = [1]);
    assert (factoriser (50) = [1;2;5]);; 
