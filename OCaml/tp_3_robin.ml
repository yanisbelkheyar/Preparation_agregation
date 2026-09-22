(* Questions listée à l'oral:
 * 1) Fonction qui génère les entiers, type unit -> (unit -> int)
 * 2) Fonction genere_get_set
 * 3) Fonction version_memoisee: ('a -> 'b) -> ('a -> 'b) *)

let genere_compteur () : unit -> int =
  let n = ref (-1) in
  fun () -> (incr n; !n)

let genere_get_set (init: 'a) : (unit -> 'a) * ('a -> unit) =
  let v = ref init in
  (fun () -> !v) , (fun x -> v := x)

let version_memoisee (f: 'a -> 'b) =
  let memoire = Hashtbl.create 16 in
  fun (x: 'a) -> match Hashtbl.find_opt memoire x with
    | Some result -> result
    | None -> begin
      let resultat = f x in
      Hashtbl.add memoire x resultat;
      resultat
    end

let test_genere_compteur () =
  let c1 = genere_compteur () in
  assert(c1 () = 0);
  assert(c1 () = 1);
  let c2 = genere_compteur () in
  assert(c2 () = 0);
  assert(c1 () = 2);
  assert(c2 () = 1)

let test_genere_get_set () =
  let g, s = genere_get_set 3 in
  assert(g () = 3);
  s 5;
  assert (g () = 5);
  let g2, s2 = genere_get_set 4.12 in
  s2 3.12;
  assert(g2 () = 3.12)

let test_version_memoisee () =
  let f x = x * x + 42 in
  let f2 = version_memoisee f in
  assert(f 0 = f2 0);
  assert(f 32 = f2 32);
  assert(f 105 = f2 105);
  assert(f 0 = f2 0);
  assert(f 32 = f2 32);
  assert(f 105 = f2 105)

(* Question 1 *)
type variable = char
type qbf =
    Vrai 
  | Faux 
  | V of variable
  | Ou of qbf * qbf
  | Et of qbf * qbf
  | Not of qbf
  | Exists of variable * qbf
  | Forall of variable * qbf

module Variable =
  struct
    type t = variable
    let compare = Stdlib.compare
  end
module VSet = Set.Make(Variable)
type vset = VSet.t

(* Question 2 *)

let rec variables_libres (formule: qbf) : vset =
  match formule with
    V v -> VSet.singleton v
  | Vrai | Faux -> VSet.empty
  | Not f -> variables_libres f
  | Ou (f1, f2) | Et (f1, f2) -> VSet.union (variables_libres f1) (variables_libres f2)
  | Exists (v, f) | Forall (v, f) -> VSet.remove v (variables_libres f)

let phi =
  Ou ((V 'x'), 
      (Exists ('y', Ou (Not (V 'z'), Exists ('x', 
        Et (Not (V 'x'),
          Et (V 'y', Faux))))))) 
let test_variables_libres () =
  assert(VSet.equal (variables_libres phi) (VSet.of_list ['x'; 'z']))

(* Question 3 *)

let rec substitution (formule: qbf) (v: variable) (constante: qbf) =
  assert(constante = Vrai || constante = Faux);
  (* This if is optional, it would be sound without it, but it reduces allocator thrashing by avoiding rebuilding formulas that don't need to change *)
  if VSet.mem v (variables_libres formule) then
    match formule with
      V v2 when v2 = v -> constante
    | Vrai | Faux | V _ -> formule
    | Exists (v2, _) when v2 <> v -> formule
    | Forall (v2, _) when v2 <> v -> formule
    | Exists (v2, f) -> Exists (v2, (substitution f v constante))
    | Forall (v2, f) -> Forall (v2, (substitution f v constante))
    | Not f -> Not (substitution f v constante)
    | Et (f1, f2) -> Et ((substitution f1 v constante), (substitution f2 v constante))
    | Ou (f1, f2) -> Ou ((substitution f1 v constante), (substitution f2 v constante))
  else
    formule

let test_substitution () =
  assert(substitution phi 'x' Vrai =
    Ou (Vrai, 
      (Exists ('y', Ou (Not (V 'z'), Exists ('x', 
        Et (Not (V 'x'),
          Et (V 'y', Faux))))))))

(* Question 4 *)

let rec expansion_quantificateurs_naif (formule: qbf) =
  match formule with
  | Exists (v, f) -> let f = expansion_quantificateurs_naif f in Ou (substitution f v Vrai, substitution f v Faux)
  | Forall (v, f) -> let f = expansion_quantificateurs_naif f in Et (substitution f v Vrai, substitution f v Faux)
  | Not f -> Not (expansion_quantificateurs_naif f)
  | Ou (f1, f2) -> let f1 = expansion_quantificateurs_naif f1 in let f2 = expansion_quantificateurs_naif f2 in Ou (f1, f2)
  | Et (f1, f2) -> let f1 = expansion_quantificateurs_naif f1 in let f2 = expansion_quantificateurs_naif f2 in Et (f1, f2)
  | Vrai | Faux | V _ -> formule

(* Cette version fait la substitution et l'expansion ensemble, en une seule passe *)
module VMap = Map.Make(Variable)
type v_f_map = qbf VMap.t
let rec expansion_quantificateurs_optim (formule: qbf) =
  let rec expand_and_substitute (formule: qbf) (map: v_f_map) : qbf =
    match formule with
    | V v -> 
      begin match VMap.find_opt v map with
        | None -> formule
        | Some f -> f
      end
    | Vrai | Faux -> formule
    | Not f -> Not (expand_and_substitute f map)
    | Et (f1, f2) -> Et ((expand_and_substitute f1 map), (expand_and_substitute f2 map))
    | Ou (f1, f2) -> Ou ((expand_and_substitute f1 map), (expand_and_substitute f2 map))
    | Exists (v, f) -> begin
      let map = VMap.remove v map in
      Ou (expand_and_substitute f (VMap.add v Vrai map),
          expand_and_substitute f (VMap.add v Faux map))
      end
    | Forall (v, f) -> begin
      let map = VMap.remove v map in
      Et (expand_and_substitute f (VMap.add v Vrai map),
          expand_and_substitute f (VMap.add v Faux map))
      end
  in
  expand_and_substitute formule VMap.empty

let test_expansion () =
  assert(expansion_quantificateurs_naif  (Exists ('x', Ou (V 'x', V 'y'))) = Ou (Ou (Vrai, V 'y'), Ou (Faux, V 'y')));
  assert(expansion_quantificateurs_optim (Exists ('x', Ou (V 'x', V 'y'))) = Ou (Ou (Vrai, V 'y'), Ou (Faux, V 'y')));
  assert(expansion_quantificateurs_naif  (Forall ('x', Ou (V 'x', V 'y'))) = Et (Ou (Vrai, V 'y'), Ou (Faux, V 'y')));
  assert(expansion_quantificateurs_optim (Forall ('x', Ou (V 'x', V 'y'))) = Et (Ou (Vrai, V 'y'), Ou (Faux, V 'y')));
  assert(expansion_quantificateurs_naif phi = expansion_quantificateurs_optim phi)

(* Question 5 *)

let evaluer_naif (formule: qbf) : bool =
  assert(VSet.is_empty (variables_libres formule));
  let rec evaluer_no_quantif (f: qbf) : bool =
    match f with
    | Vrai -> true
    | Faux -> false
    | Ou (f1, f2) -> evaluer_no_quantif f1 || evaluer_no_quantif f2
    | Et (f1, f2) -> evaluer_no_quantif f1 && evaluer_no_quantif f2
    | Not f' -> not (evaluer_no_quantif f')
    | _ -> assert(false)
  in evaluer_no_quantif (expansion_quantificateurs_optim formule)

let test_eval eval =
  assert(eval (Forall ('x', (Ou (V 'x', Not (V 'x'))))) = true);
  assert(eval (Exists ('x', (Et (V 'x', Not (V 'x'))))) = false)
let test_evaluation_naif () =
  test_eval evaluer_naif

(* Question 6 *)

let evaluer_optim : qbf -> bool =
  let rec evaluer_with_env (env: bool VMap.t) (f: qbf) =
    let recur = evaluer_with_env env in
    match f with
    | Vrai -> true
    | Faux -> false
    | V v -> VMap.find v env
    | Not f' -> not (recur f')
    | Ou (f1, f2) -> recur f1 || recur f2
    | Et (f1, f2) -> recur f1 && recur f2
    | Exists (v, f') -> evaluer_with_env (VMap.add v true env) f' 
                     || evaluer_with_env (VMap.add v false env) f'
    | Forall (v, f') -> evaluer_with_env (VMap.add v true env) f' 
                     && evaluer_with_env (VMap.add v false env) f'
  in
  evaluer_with_env (VMap.empty)

let test_evaluation_optim () =
  test_eval evaluer_optim

let _ = begin
  test_genere_compteur ();
  test_genere_get_set ();
  test_version_memoisee ();
  test_variables_libres ();
  test_substitution ();
  test_expansion ();
  test_evaluation_naif ();
  test_evaluation_optim ()
end