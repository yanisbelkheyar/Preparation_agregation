(* Un noeud represente un p-gramme (avec p sa profondeur dans l'arbre) :
  il stocke ses successeurs et leurs occurences *)
type noeud = {
  c : char; (* caractère associé à ce noeud *)
  mutable occ : int; (* nombre d'occurences associé à ce noeud *)
  mutable tot : int; 
    (* somme des occurences de successeurs : denominateur de P(s|p-gramme) *)
  mutable succ : noeud list; 
    (* fils tq fils.c = s : (p+1)-gramme allonge du caractere s *)
}

(* Le modele est un arbre, la profondeur p porte les p-grammes
  et la hauteur est bornée par n *)
type modele = {
  h : int; (* Hauteur de l'arbre, i.e. N dans l'enonce *)
  root : noeud; (* Essentiellement juste la liste des 1-grammes *)
}



(* Descend d'un cran depuis n en suivant le caractere s :
  renvoie le fils correspondant en le creant si besoin,
  et met a jour les compteurs liés à l'ajout de s dans le modele *)
let descendre (n: noeud) (s: char): noeud =
  n.tot <- n.tot + 1;
  match List.find_opt (fun f -> f.c = s) n.succ with
  | Some f -> f.occ <- f.occ + 1; f
  | None -> let
    f = {c = s; occ = 1; tot = 0; succ = []} in
    n.succ <- f::(n.succ); f
;;

(* Genere le modele de max-gramme associé au texte cont :
  contient tout les p-grammes de cont pour p allant de 1 à max *)
let generer_modele (cont: string) (max: int): modele =
  let len = String.length cont in
  let r = {c = '\000'; occ = 0; tot = 0; succ = []} in

  (* Insere cont[i...i+p] en tant que p-gramme a la suite de tmp *)
  let rec inserer (tmp: noeud) (i: int) (p: int): unit =
    if p >= 0 && i < len then inserer (descendre tmp cont.[i]) (i+1) (p-1)
  in

  for i = 0 to len-1 do
    inserer r i max
  done;

  {h = max; root = r}
;;



(* Genere le modele de max-gramme associé au texte cont[deb...fin] :
  contient tout les p-grammes de cont pour p allant de 1 à max, et
  considere aussi le texte cont[fin+1...fin+max] pour calculer les occurences,
  sans pourtant inserer les p-grammes demarrant a l'interieur de cette portion *)
let generer_modele_branch (cont: string) (deb: int) (fin: int) (max: int): modele =
  (* borne exclusive : il faut max+1 caracteres a partir de fin *)
  let len = min (String.length cont) (fin + max + 1) in
  let r = {c = '\000'; occ = 0; tot = 0; succ = []} in

  let rec inserer (tmp: noeud) (i: int) (p: int): unit =
    if p > 0 && i < len then inserer (descendre tmp cont.[i]) (i+1) (p-1)
  in

  for i = deb to fin do
    inserer r i (max+1)
  done;

  {h = max; root = r}
;;

(* Fusionne le noeud a et le noeud b en supposant que a.c = b.c,
  et stocke le resultat dans a *)
let rec fusionner (a: noeud) (b: noeud): unit =
  a.occ <- a.occ + b.occ;
  a.tot <- a.tot + b.tot;
  List.iter (fun fb ->
    match List.find_opt (fun fa -> fa.c = fb.c) a.succ with
    | Some fa -> fusionner fa fb
    | None -> a.succ <- fb::(a.succ))
    b.succ
;;

(* Genere le modele de max-gramme associé au texte cont :
  contient tout les p-grammes de cont pour p allant de 1 à max,
  et utilise nb_threads pour ça *)
let generer_parallele (cont: string) (max: int) (nb_threads: int): modele =
  let len = String.length cont in
  if len = 0 || nb_threads <= 1 then generer_modele cont max
  else begin
    let p = min nb_threads len in
    let taille = (len + p - 1) / p in
    let results = Array.make p None in

    (* chaque thread n'ecrit que dans results.(i) : pas besoin de verrous *)
    let init (i: int) (): unit =
      let deb = i * taille in
      let fin = (min len (deb + taille)) - 1 in
      if deb <= fin then
        results.(i) <- Some (generer_modele_branch cont deb fin max)
    in

    let threads = Array.init p (fun i -> Thread.create (init i) ()) in
    Array.iter Thread.join threads;

    let m = {h = max; root = {c = '\000'; occ = 0; tot = 0; succ = []}} in
    Array.iter (function
      | None -> ()
      | Some mi -> fusionner m.root mi.root)
      results;
    m
  end
;;



(* A partir du noeud n, descend le plus loin possible
  avec la chaine de caracteres cont *)
let extend_n_gramme (n: noeud) (cont: string): noeud option =
  let len = String.length cont in
  (* A partir du noeud n, descend le plus loin possible
  avec la chaine de caracteres cont[i...len-1] *)
  let rec extend_n_gramme_aux (i: int) (n: noeud): noeud option =
    if i >= len then Some n else
    (* cherche un eventuel successeur pour l'appel recursif *)
    try let new_n = List.find (fun m -> m.c = cont.[i]) n.succ
    in extend_n_gramme_aux (i+1) new_n
    with Not_found -> None
  in extend_n_gramme_aux 0 n
;;

(* Renvoie une prediction aleatoire de caractere succedant
  au p-gramme n, en utilisant les occurence pour ponderer *)
let recuperer_rand (n: noeud): char =
  if n.tot <= 0 then '\000'
  else
    let rec aux (acc: int) (l: noeud list): char =
      match l with
      | [] -> '\000' (* impossible si tot = somme des occ *)
      | f::q -> if acc <= f.occ then f.c else aux (acc - f.occ) q
    in aux ((Random.int n.tot) + 1) n.succ
;;

(* Avec le modele de n-grammes m, predit une suite possible
  pour la chaine de caracteres seq, de taille maximale lim *)
let rec predit_suite (m: modele) (seq: string) (lim: int): string =
  if lim <= 0 then seq else

  (* Choisit un caractere avec une prediction calculee sur les
    occurences des successeurs depuis un k-gramme a la fin de seq,
    pour k entre 0 et i, avec probabilite 2/3^(m.h-k+1) pour
    k different de 1, et 1/3^(m.h) pour k = 1 *)
  let rec choisit_p_gramme (i: int): char =
      if (i < 1) then '\000' else
      try let to_extend = String.sub seq ((String.length seq)-i) i in

      match extend_n_gramme m.root to_extend with
      | None -> choisit_p_gramme (i-1)
      | Some f -> (* Avec une chance sur deux on prend un i-gramme, et avec
        une chance chance sur trois on appelle recursivement pour un (i-1)-gramme *)
        if f.tot > 0 && (0 < Random.int 2 || i <= 1) then
          recuperer_rand f
        else
          choisit_p_gramme (i-1)

        with Invalid_argument _ -> choisit_p_gramme (i-1)
  in

  let next_char = choisit_p_gramme m.h in
  if next_char = '\000' then
    seq
  else
    predit_suite m (seq ^ (String.make 1 next_char)) (lim-1)
;;



(* Fonction de test *)
let test (): unit =
  let m = generer_modele "" 5 in
  let p = predit_suite m "Bonjour" 4 in
  Printf.printf "Modèle vide : %s\n" p;
  assert (String.length p = 7);

  let m = generer_modele "Bonjour, comment allez-vous ? Ca va, ca va aller bien mieux." 0 in
  let p = predit_suite m "Bonjour" 4 in
  Printf.printf "Modèle de 0-gramme : %s\n" p;
  assert (String.length p = 7);

  let m = generer_modele "abcde" 1 in
  let p = predit_suite m "a" 1000 in
  Printf.printf "Valeur de prediction trop grande : %s\n" p;
  assert (String.length p = 5);

  let m = generer_modele "abcde." 5 in
  let p = predit_suite m "a" (-1) in
  Printf.printf "Valeur de prediction negative : %s\n" p;
  assert (String.length p = 1);

  let m = generer_modele "a\nb\tc\na\nb" 2 in
  let p = predit_suite m "a" 4 in
  Printf.printf "%s\n" p;
  assert (String.length p = 5);

  let m = generer_modele "f(x) = [a+b] * {c-d} / 100% ; g(y) = [a+b] * 2" 4 in
  Printf.printf "%s\n" (predit_suite m "[" 15);

  let texte = "Merge two lists: Assuming that l1 and l2 are sorted according to the comparison function cmp, merge cmp l1 l2 will return a sorted list containing all the elements of l1 and l2. If several elements compare equal, the elements of l1 will be before the elements of l2. Not tail-recursive (sum of the lengths of the arguments)." in

  List.iter (fun n ->
    let m = generer_modele texte n in
    Printf.printf "N=%d %s\n" n (predit_suite m "a" 100))
    [1; 3; 10; 500]
;;

(* Fonction de test pour la predit_suite_parallele *)
let test_parallele (): unit =
  (* Teste si deux noeuds a et b sont identiques *)
  let rec identiques (a: noeud) (b: noeud): bool =
    let tri l = List.sort (fun x y -> compare x.c y.c) l in
    a.c = b.c && a.occ = b.occ && a.tot = b.tot
    && List.length a.succ = List.length b.succ
    && List.for_all2 identiques (tri a.succ) (tri b.succ)
  in
  let texte = "Bonjour, comment allez-vous ? Ca va, ca va aller bien mieux." in
  List.iter (fun p ->
    List.iter (fun n ->
      assert (identiques (generer_modele texte n).root
                         (generer_parallele texte n p).root))
      [0; 1; 2; 3; 5])
    [0; 1; 2; 3; 4; 8; 100]
;;