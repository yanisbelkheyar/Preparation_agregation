(* Changements par rapport à la version présentée:
 * - Utilisation de Domain pour la version parallèle plutôt que Thread (qui est en fait une librairie autre et pas la "vraie" concurrence fournie par ocaml depuis la v5)
 * - Ajout d'une version plus optimisée de la version parallèle, qui utilise plusieurs threads pour la fusion (la complexité va de linéaire en le nombre de threads à logarithmique)
 * - Test sur un grand texte (merci Corentin pour l'avoir fourni sur discord), avec mesure de la taille du modèle.
     Cela confirme que l'utilisation mémoire sur du texte en langue naturelle est bien plus raisonnable que la complexité dans le pire cas.
     Egalement mesure du temps passé, montrant que la parallélisation est un gain quand n est petit, mais la fusion est trop couteuse quand n est grand (sur ce texte).
 *)

(* Question 4 *)

type n_gram = {
  profondeur: int;
  mutable le_plus_vu: char;
  mutable occurences_max: int;
  mutable occurences_total: int;
  occurences: int array;
  fils: n_gram option array;
};;

(* Question 5 *)

let num_ascii : int = 128;;
exception ChaineNonAscii;;

let allouer_n_gram_vide (profondeur: int) : n_gram =
  {
    profondeur = profondeur;
    le_plus_vu = ' ';
    occurences_max = 0;
    occurences_total = 0;
    occurences = Array.make num_ascii 0;
    fils = Array.make num_ascii None;
  }
;;

let ajouter_n_occurences (c: char) (c_int: int) (noeud: n_gram) (n: int) : unit =
  assert(c_int = Char.code c);
  assert(c_int < num_ascii);
  let num_occurences: int = noeud.occurences.(c_int) + n in
  noeud.occurences.(c_int) <- num_occurences;
  if num_occurences > noeud.occurences_max then begin
    noeud.occurences_max <- num_occurences;
    noeud.le_plus_vu <- c
  end
;;

let ajouter_occurence (c: char) (c_int: int) (noeud: n_gram) : unit =
  ajouter_n_occurences c c_int noeud 1
;;

let rec ajouter_char_au_modele (texte: string) (i: int) (c: char) (c_int: int) (modele: n_gram) (n: int) : unit =
  assert(c_int = Char.code c);
  assert(c_int < num_ascii);

  ajouter_occurence c c_int modele;

  let new_i = i - 1 in
  if modele.profondeur < n && new_i >= 0 then begin
    let c_previous = texte.[new_i] in
    let c_previous_int = Char.code(c_previous) in
    let noeud_suivant: n_gram = match modele.fils.(c_previous_int) with
      | Some noeud -> noeud
      | None -> begin
        let noeud = allouer_n_gram_vide (modele.profondeur + 1) in
        modele.fils.(c_previous_int) <- Some noeud;
        noeud
      end
    in
    assert(noeud_suivant.profondeur = modele.profondeur + 1);
    ajouter_char_au_modele texte new_i c c_int noeud_suivant n
  end
;;

let construire_n_grams (texte: string) (n: int) : n_gram =
  let racine: n_gram = allouer_n_gram_vide 0 in

  for i = 0 to String.length texte - 1 do
    let c = texte.[i] in
    let c_int = Char.code c in
    begin if c_int >= num_ascii then
      raise ChaineNonAscii
    end;
    ajouter_char_au_modele texte i c c_int racine n
  done;

  racine
;;

(* Question 6 *)

(* Stratégie choisie: on utilise le niveau de profondeur maximale pour lequel on a au moins une occurence
 * C'est à dire que si les n caractères précédents ont déjà été vus, on utilise le modèle comme un modèle de n-gram,
   sinon si les n-1 caractères précédents ont déjà été us, on l'utilise comme un modèle de (n-1)-gram, etc.. *)

exception TexteTropCourt;;
(* Prédit le caractère suivant à partir des derniers caractères de
 - si i >0: texte_prefixe ^ sequence[0..i)
 - si i =0: texte_prefixe
 - si i < 0: texte_prefixe en ignorant les -i derniers caracteres *)
let rec predire_caractere (noeud: n_gram) (texte_prefixe: string) (sequence: bytes) (i: int) : char =
  let t_len = String.length texte_prefixe in
  let get_char_int j : int =
    let c = if j > 0 then
        Bytes.get sequence (j - 1)
      else if t_len > (- j) then
        texte_prefixe.[t_len + j - 1]
      else
        raise TexteTropCourt
    in
    Char.code c
  in
  try
    match noeud.fils.(get_char_int i) with
      Some noeud_suivant -> predire_caractere noeud_suivant texte_prefixe sequence (i-1)
    | None -> noeud.le_plus_vu
  with
    TexteTropCourt -> noeud.le_plus_vu
;;

let predire_sequence (racine: n_gram) (texte: string) (n: int) : string =
  let resultat = Bytes.create n in
  for i = 0 to n - 1 do
    Bytes.set resultat i (predire_caractere racine texte resultat i)
  done;
  Bytes.to_string resultat
;;

let test_n (n: int) = construire_n_grams "Bonjour, comment allez-vous ? Ca va, ca va aller bien mieux." n;;
assert(predire_sequence (test_n 0) "Bonjour, " 10 = "          ");;
(* Vérifié à la main, et correspond au résultat du code C *)
assert(predire_sequence (test_n 1) "Bonjour, " 10 = "cour, cour");;
(* Vérifié à la main *)
assert(predire_sequence (test_n 2) "Bonjour, " 10 = "comment al");;

(* Question 7 *)

(* Approche choisie: segmenter le texte en n chunks, chacun de taille texte_len/nombre_threads (à 1 près)
    , et avoir chaque thread produire son propre modele.
  Cela implique que les différents threads lise simultanément certaines parties du texte, puisque construire un
    modèle sur texte[i, j) implique de lire texte[i-n, j), donc les il y a chevauchement des segments lus. Ce n'est pas
    un problème: ces accès sont purements en lecture (et de toute façon le texte est une string immutable), donc ils ne
    causent pas de data race.
  Le principal avantage de cette approche est de rendre la fusion relativement simple (par rapport à faire des segments qui ne
    se chevauchent pas, et qui demanderaient de faire des réparations à la jointure) *)

(* Construit un modèle de n-gram sur texte[debut, fin). *)
let construire_n_grams_partiel (racine: n_gram) (texte: string) (debut: int) (fin: int) (n: int) : n_gram =
  assert(racine = allouer_n_gram_vide 0);
  for i = debut to fin - 1 do
    let c = texte.[i] in
    let c_int = Char.code c in
    begin if c_int >= num_ascii then
      raise ChaineNonAscii
    end;
    ajouter_char_au_modele texte i c c_int racine n
  done;

  racine
;;

(* Note: on pourrait remplacer la version de construire_n_grams de la question 5 par celle-ci *)
let construire_n_grams_2 (texte: string) (n: int): n_gram =
  construire_n_grams_partiel (allouer_n_gram_vide 0) texte 0 (String.length texte) n
;;
let test_2_versions (texte: string) (n: int) : bool =
  let resultat = ref true in
  for i = 0 to n do
    resultat := !resultat && (construire_n_grams_2 texte n = construire_n_grams texte n)
  done;
  !resultat
;;
assert(test_2_versions "Bonjour, comment allez-vous ? Ca va, ca va aller bien mieux." 5);;

let construire_n_grams_array_parallele (texte: string) (n: int) (nombre_threads: int): n_gram array =
  assert(nombre_threads >= 1);
  let texte_len = String.length texte in
  (* Attention: utiliser Array.make ici créerait un tableau où toutes les cases pointent vers le même noeud.
      Ce que l'on veut ici est un noeud différent pour chaque case *)
  let resultat = Array.init nombre_threads (fun _ -> allouer_n_gram_vide 0) in
  
  let per_thread_fun ((i, debut, fin): int * int * int): n_gram =
    construire_n_grams_partiel resultat.(i) texte debut fin n
  in
  let frontiere (i: int) = i * texte_len / nombre_threads in
  let threads = Array.init (nombre_threads - 1) (fun i ->
    let j = i + 1 in (* +1 pour laisser le premier slot de resultat au thread principal *)
    Domain.spawn (fun () -> per_thread_fun (j, frontiere j, frontiere (j + 1)))
  )
  in
  resultat.(0) <- per_thread_fun (0, 0, frontiere 1);
  for i = 0 to nombre_threads - 2 do
    resultat.(i + 1) <- Domain.join threads.(i)
  done;
  resultat
;;

(* Question 8 *)

(* Fusionne src et dst, stocke le résultat dans dst *)
let rec ajouter_n_gram (dst: n_gram) (src: n_gram) : unit =
  assert(dst.profondeur = src.profondeur);
  for i = 0 to num_ascii - 1 do
    ajouter_n_occurences (Char.chr i) i dst (src.occurences.(i))
  done;
  for i = 0 to num_ascii - 1 do
    match dst.fils.(i), src.fils.(i) with
      _, None -> ()
      | None, (Some node as node_option) -> dst.fils.(i) <- node_option
      | Some dst_fils, Some src_fils -> ajouter_n_gram dst_fils src_fils
  done
;;

(* naif car single-threadé. Si devient un bottleneck, une solution récursive ou chaque *)
let fusion_n_grams_array_naif (arr: n_gram array) : n_gram =
  let len = Array.length arr in
  assert(len >= 1);
  let resultat = arr.(0) in
  for i = 1 to len - 1 do
    ajouter_n_gram resultat arr.(i)
  done;
  resultat
;;

let construire_n_grams_parallele_naif  (texte: string) (n: int) (nombre_threads: int) : n_gram =
  let n_grams = construire_n_grams_array_parallele (texte: string) (n: int) (nombre_threads: int) in
  fusion_n_grams_array_naif n_grams
;;

let taille_max_pour_spawn = 3
(* Même sémantique que construire_n_grams_partiel mais a le droit d'utiliser jusqu'à max_threads.
   * Ne spawn pas de thread supplémentaire si le segment de texte à traiter est de taille < taille_max_pour_spawn *)
let rec construire_n_grams_partiel_parallele (texte: string) (debut: int) (fin: int) (n: int) (max_threads: int) : n_gram =
  assert(max_threads >= 1);
  if String.length texte < taille_max_pour_spawn || max_threads = 1 then
    construire_n_grams_partiel (allouer_n_gram_vide 0) texte debut fin n
  else begin
    let milieu = (debut + fin) / 2 in
    let max_threads_1 = max_threads / 2 in
    let max_threads_2 = max_threads - max_threads_1 in
    let d = Domain.spawn (fun () -> construire_n_grams_partiel_parallele texte debut milieu n max_threads_1) in
    let modele_2 = construire_n_grams_partiel_parallele texte milieu fin n max_threads_2 in
    let modele_1 = Domain.join d in
    ajouter_n_gram modele_2 modele_1;
    modele_2
  end
;;

let construire_n_grams_parallele_optim (texte: string) (n: int) (max_threads: int) : n_gram =
  construire_n_grams_partiel_parallele texte 0 (String.length texte) n max_threads
;;

(* Pas juste =, car le_plus_vu peut légitimement être différent: il peut y avoir plusieurs caractères avec la même probabilité *)
let rec est_equivalent_n_grams noeud1 noeud2 =
  noeud1.profondeur = noeud2.profondeur
  && noeud1.occurences_total = noeud2.occurences_total
  && noeud1.occurences_max = noeud2.occurences_max
  && noeud1.occurences = noeud2.occurences
  && begin
    let fils_equivalents = ref true in 
    for i = 0 to num_ascii - 1 do
      fils_equivalents := !fils_equivalents && match noeud1.fils.(i), noeud2.fils.(i) with
        None, None -> true
        | Some a, Some b -> est_equivalent_n_grams a b
        | _ -> false
    done;
    !fils_equivalents
  end
;;

let test_construction_parallele (texte: string) (n: int) (nombre_threads: int) =
  est_equivalent_n_grams (construire_n_grams texte n)
                         (construire_n_grams_parallele_naif texte n nombre_threads)
  && est_equivalent_n_grams (construire_n_grams texte n)
                            (construire_n_grams_parallele_optim texte n nombre_threads)
;;

assert(test_construction_parallele "Bonjour, comment allez-vous ? Ca va, ca va aller bien mieux." 0 1);;
assert(test_construction_parallele "Bonjour, comment allez-vous ? Ca va, ca va aller bien mieux." 0 2);;
assert(test_construction_parallele "Bonjour, comment allez-vous ? Ca va, ca va aller bien mieux." 0 5);;
assert(test_construction_parallele "Bonjour, comment allez-vous ? Ca va, ca va aller bien mieux." 1 2);;
assert(test_construction_parallele "Bonjour, comment allez-vous ? Ca va, ca va aller bien mieux." 3 1);;
assert(test_construction_parallele "Bonjour, comment allez-vous ? Ca va, ca va aller bien mieux." 3 2);;
assert(test_construction_parallele "Bonjour, comment allez-vous ? Ca va, ca va aller bien mieux." 3 5);;

let print_allocs (texte: string) (n: int) (l: int) : unit =
  let l = Stdlib.min l (String.length texte) in
  let word_size = 8 in
  let bytes_allocated () =
    let (minor_words, promoted_words, major_words) = Gc.counters () in
    word_size * int_of_float (minor_words +. major_words -. promoted_words)
  in
  let b1 = bytes_allocated () in
  let modele = construire_n_grams_partiel (allouer_n_gram_vide 0) texte 0 l n in
  let b2 = bytes_allocated () in
  Printf.printf "[Sequentiel] Construire le modèle de n-grams pour n=%d sur %#d caractères a pris %#d octets d'allocations, pour un modèle de %#d octets\n"
    n l (b2 - b1) (word_size * (Obj.reachable_words (Obj.repr modele)));
;;

(* Commenté car Unix.gettimeofday requires doing '#load "unix.cma"'
let print_timings (texte: string) (n: int) (l: int) (max_threads: int) : unit =
  let l = Stdlib.min l (String.length texte) in
  let t1_start: float = Unix.gettimeofday () in
  let _ = construire_n_grams_partiel (allouer_n_gram_vide 0) texte 0 l n in
  let t1_end: float = Unix.gettimeofday () in
  Gc.major ();
  let t2_start: float = Unix.gettimeofday () in
  let _ = construire_n_grams_partiel_parallele texte 0 l n max_threads in
  let t2_end: float = Unix.gettimeofday () in
  Printf.printf "Timings, construction n=%d, sur %d caractères: séquentiel = %f s, parallele[max_threads = %d] = %f s\n" n l (t1_end -. t1_start) max_threads (t2_end -. t2_start)
;;
*)

(* Commenté car ça prend quelques secondes, mais marche sans problème
let texte_long = In_channel.with_open_text "vingt_mille_lieues.txt" In_channel.input_all;;

let _ = print_timings texte_long 3 10 8;;
let _ = print_timings texte_long 3 100 8;;
let _ = print_timings texte_long 3 1000 8;;
let _ = print_timings texte_long 3 10000 8;;
let _ = print_timings texte_long 3 100000 8;;
let _ = print_timings texte_long 3 1000000 8;;
let _ = print_timings texte_long 2 1000000 8;;
let _ = print_timings texte_long 4 1000000 8;;
let _ = print_timings texte_long 5 1000000 8;;

assert(test_construction_parallele texte_long 3 1);;
assert(test_construction_parallele texte_long 3 5);;
assert(test_construction_parallele texte_long 3 8);;

let _ = print_allocs texte_long 3 10;;
let _ = print_allocs texte_long 3 100;;
let _ = print_allocs texte_long 3 1000;;
let _ = print_allocs texte_long 3 10000;;
let _ = print_allocs texte_long 3 100000;;
let _ = print_allocs texte_long 3 1000000;;
let _ = print_allocs texte_long 2 1000000;;
let _ = print_allocs texte_long 4 1000000;;
let _ = print_allocs texte_long 5 1000000;;
*)
