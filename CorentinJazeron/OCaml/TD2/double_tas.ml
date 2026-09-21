(* Element d'un tas *)
type 'a elt = {
  cle : 'a;
  mutable imin : int;   (* position dans le tas min *)
  mutable imax : int;   (* position dans le tas max *)
}

(* Un tas binaire générique stocké dans un tableau redimensionnable *)
type 'a tas = {
  mutable t : 'a elt array;
  mutable n : int;                      (* nombre d'éléments *)
  avant : 'a elt -> 'a elt -> bool;     (* a doit-il être au-dessus de b ? *)
  maj : 'a elt -> int -> unit;          (* met à jour la position de l'élément *)
}

(* Un double tas = deux tas, un min et un max *)
type 'a double_tas = { tmin : 'a tas; tmax : 'a tas }

(*--- Operations de tas ---*)

(* Échange deux cases en maintenant les positions à jour *)
let echanger (h: 'a tas) (i: int) (j: int): unit =
  let x = h.t.(i) and y = h.t.(j) in
  h.t.(i) <- y;
  h.t.(j) <- x;
  h.maj y i;
  h.maj x j

(* Fait monter la case i dans le tas h *)
let rec monter (h: 'a tas) (i: int): unit =
  if i > 0 then begin
    let p = (i - 1) / 2 in
    if h.avant h.t.(i) h.t.(p) then begin
      echanger h i p;
      monter h p
    end
  end

(* Fait descendre la case i dans le tas h *)
let rec descendre (h: 'a tas) (i: int): unit =
  let g = 2 * i + 1 and d = 2 * i + 2 in
  let m = ref i in
  if g < h.n && h.avant h.t.(g) h.t.(!m) then m := g;
  if d < h.n && h.avant h.t.(d) h.t.(!m) then m := d;
  if !m <> i then begin
    echanger h i !m;
    descendre h !m
  end

(* Ajout en O(log n) (doublement du tableau : O(1) amorti) *)
let ajouter (h: 'a tas) (e: 'a elt): unit =
  if h.n = Array.length h.t then begin
    let nt = Array.make (max 1 (2 * h.n)) e in
    Array.blit h.t 0 nt 0 h.n;
    h.t <- nt
  end;
  h.t.(h.n) <- e;
  h.maj e h.n;
  h.n <- h.n + 1;
  monter h (h.n - 1)

(* Retire l'élément situé en position i, en O(log n) :
  on le remplace par le dernier, puis on rétablit la propriété de tas
  (l'élément déplacé peut devoir monter OU descendre) *)
let supprimer (h: 'a tas) (i: int): unit =
  let dernier = h.n - 1 in
  if i = dernier then h.n <- dernier
  else begin
    echanger h i dernier;
    h.n <- dernier;
    monter h i;
    descendre h i
  end

(*--- Operations de double tas ---*)

(* Crée un double tas vide *)
let creer (): 'a double_tas = {
  tmin = { t = [||]; n = 0;
           avant = (fun a b -> a.cle < b.cle);
           maj = (fun e i -> e.imin <- i) };
  tmax = { t = [||]; n = 0;
           avant = (fun a b -> a.cle > b.cle);
           maj = (fun e i -> e.imax <- i) };
}

(* Teste si d est vide *)
let est_vide (d: 'a double_tas): bool = d.tmin.n = 0

(* Renvoie la taille de d *)
let taille (d: 'a double_tas): int = d.tmin.n

(* O(log n) : insertion de x dans chacun des deux tas de d *)
let inserer (d: 'a double_tas) (x: 'a): unit =
  let e = { cle = x; imin = -1; imax = -1 } in
  ajouter d.tmin e;
  ajouter d.tmax e

(* O(log n) : on retire la racine du tas min de d, puis on retire
  le même élément du tas max de d grâce à sa position e.imax *)
let extraire_min (d: 'a double_tas): 'a =
  if est_vide d then failwith "extraire_min : structure vide";
  let e = d.tmin.t.(0) in
  supprimer d.tmin 0;
  supprimer d.tmax e.imax;
  e.cle

(* O(log n) : symétrique *)
let extraire_max (d: 'a double_tas): 'a =
  if est_vide d then failwith "extraire_max : structure vide";
  let e = d.tmax.t.(0) in
  supprimer d.tmax 0;
  supprimer d.tmin e.imin;
  e.cle

(*--- Test ---*)

let () =
  let d = creer () in
  List.iter (inserer d) [5; 3; 17; 10; 84; 19; 6; 22; 9; 1];
  (* Extraction alternée min / max : 1 84 3 22 5 19 6 17 9 10 *)
  while not (est_vide d) do
    let m = extraire_min d in
    Printf.printf "min = %d\n" m;
    if not (est_vide d) then begin
      let x = extraire_max d in
      Printf.printf "max = %d\n" x
    end
  done