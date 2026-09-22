(*QUESTION 1*)
type qbf = 
  |Top 
  |Bottom 
  |Var of char 
  |Exists of char * qbf
  |Forall of char * qbf
  |Not of qbf 
  |And of qbf * qbf
  |Or of qbf * qbf

let () =
  Printf.printf "Question 1 ok.\n"

let my_qbf = Or(Var('x'), Exists('y', Or(Not(Var('z')), (Exists('x', And(Not(Var('x')),And(Var('y'),Bottom)))))))

(*QUESTION 2*)
let rec remove_doubles (l:'a list) : 'a list =
  match l with 
  |[] -> []
  |h::t -> if List.mem h t then remove_doubles t else h :: (remove_doubles t)

let get_free_var (phi : qbf) : char list =
  let rec aux (f : qbf) (linked : char list) : char list=
    match f with 
    |Top|Bottom -> []
    |Var(x) -> if List.mem x linked then [] else [x]
    |Not(f1) ->  aux f1 linked
    |And(f1, f2)|Or(f1,f2) -> (aux f1 linked) @ (aux f2 linked)
    |Exists(x,f1)|Forall(x,f1) -> aux f1 (x::linked)
  in remove_doubles (aux phi [])

let rec list_eq (l1 : 'a list) (l2 : 'a list) : bool =
  match l1 with
  |[] -> true
  |h::t -> if(not(List.mem h l2)) then false else list_eq t l2

let () =
  assert(get_free_var (Top) = []);
  assert(get_free_var (Var 'c') = ['c']);
  assert(list_eq (get_free_var (Or(Exists('x', And(Var 'x', Var 'y')), Var('z'))))  ['y'; 'z']);
  assert(list_eq (get_free_var my_qbf) ['x'; 'z']);
  assert(list_eq [1;2;3] [2;1;3]);
  assert(remove_doubles [1;1;2;2;1;2] = [1;2]);
  Printf.printf "Question 2 ok.\n"

(*QUESTION 3*)
exception Qbf_not_constant
let set_var (phi : qbf) (x : char) (constant : qbf) : qbf =
  (*Precondition : constant est Top ou Bottom*)
  (match constant with
  |Top|Bottom-> ()
  |_-> raise Qbf_not_constant);
  let rec aux (f : qbf) : qbf =
      match f with
      |Top|Bottom -> f
      |Var(y) when y=x -> constant
      |Var(_) -> f
      |Not(f1) ->  Not(aux f1)
      |And(f1, f2) -> And(aux f1, aux f2)
      |Or(f1,f2) -> Or(aux f1, aux f2)
      |Exists(y,_) when y=x -> f (*Si la variable est liée alors pas de changement dans la formule*)
      |Exists(y,f1) -> Exists(y, aux f1)
      |Forall(y,_) when y=x-> f (*Si la variable est liée alors pas de changement dans la formule*)
      |Forall(y,f1) -> Forall(y, aux f1)
  in aux phi

let () =
  assert(set_var (Var('x')) 'x' Top = Top);
  assert(set_var my_qbf 'y' Bottom = my_qbf);
  assert(set_var my_qbf 'x' Top = Or(Top, Exists('y', Or(Not(Var('z')), (Exists('x', And(Not(Var('x')),And(Var('y'),Bottom))))))));
  Printf.printf("Question 3 ok.\n")

(*QUESTION 4*)
let expand_quantifiers (phi : qbf) : qbf =
  let rec aux (f:qbf) : qbf =
    match f with
    |Top|Bottom -> f
    |Var _ -> f (*Si on arrive ici, la variable est libre*)
    |Not(f1) -> Not(aux f1)
    |And(f1,f2) -> And(aux f1, aux f2)
    |Or(f1,f2) -> Or(aux f1, aux f2)
    |Exists(x,f1) -> Or(aux (set_var f1 x Top), aux (set_var f1 x Bottom))
    |Forall(x,f1) -> And(aux (set_var f1 x Top), aux (set_var f1 x Bottom))
  in aux phi

let () =
  assert(expand_quantifiers (Exists('x', Or(Var('x'), Var('y')))) = Or(Or(Top, Var('y')), Or(Bottom, Var('y'))));
  assert(expand_quantifiers (Forall('x', Or(Var('x'), Var('y')))) = And(Or(Top, Var('y')), Or(Bottom, Var('y'))));
  Printf.printf("Question 4 ok.\n")

(*QUESTION 5*)
exception Unexpected_qbf
let evaluate_closed_qbf (phi : qbf) : bool =
  (*Precondition : phi est close.*)
  assert(get_free_var phi = []);
  let rec aux (f:qbf) : bool =
    (*Precondition : f n'a pas de Forall, Exists ou Var*)
    match f with
    |Top -> true
    |Bottom -> false
    |And(f1,f2) -> (aux f1) && (aux f2)
    |Or(f1,f2) -> (aux f1) || (aux f2)
    |Not(f1) -> not(aux f1)
    |_ -> raise Unexpected_qbf
  in aux (expand_quantifiers phi)

let () =
  assert(evaluate_closed_qbf(Top));
  assert(evaluate_closed_qbf(Not(Bottom)));
  assert(evaluate_closed_qbf(Exists('x', Var('x'))));
  assert(not(evaluate_closed_qbf(Not(And(Top, Forall('x', Exists('y', Or(Var('x'), Var('y')))))))));
  Printf.printf("Question 5 ok.\n")

(*QUESTION 6*)
let evaluate_qbf (phi : qbf) (env : (char, qbf) Hashtbl.t) : bool =
  (*Précondition : toutes les variables libres de phi ont une qbf assignée dans env.*)
  assert(List.fold_left (fun acc h-> acc && (Hashtbl.mem env h)) true (get_free_var phi));
  let rec evaluate_free_var (f:qbf) (free_var : char list) (local_env : (char, qbf) Hashtbl.t): qbf =
    match free_var with 
    |[] -> f
    |h::t -> evaluate_free_var (set_var f h (Hashtbl.find local_env h)) t local_env
  in let rec evaluate (f:qbf) : bool =
    match f with
    |Top -> true
    |Bottom -> false
    |And(f1,f2) -> (evaluate f1) && (evaluate f2)
    |Or(f1,f2) -> (evaluate f1) || (evaluate f2)
    |Not(f1) -> not(evaluate f1)
    |Exists(x,f1) -> evaluate (evaluate_free_var f1 [x] (let t0 = Hashtbl.create 2 in Hashtbl.add t0 x Top; t0)) 
      || evaluate (evaluate_free_var f1 [x] (let t0 = Hashtbl.create 2 in Hashtbl.add t0 x Bottom; t0))
    |Forall(x,f1) -> evaluate (evaluate_free_var f1 [x] (let t0 = Hashtbl.create 2 in Hashtbl.add t0 x Top; t0)) 
      && evaluate (evaluate_free_var f1 [x] (let t0 = Hashtbl.create 2 in Hashtbl.add t0 x Bottom; t0))
    |_ -> raise Unexpected_qbf (*Var ont normalement disparu, au début pour les variables libres, à l'évaluation des quantificateurs pour les variables liées*)
  in evaluate (evaluate_free_var phi (get_free_var phi) env)

let () =
  let env = Hashtbl.create 10 in
  Hashtbl.add env 'x' Bottom;
  Hashtbl.add env 'z' Top;
  assert(evaluate_qbf (Var('z')) env);
  assert(not(evaluate_qbf my_qbf env));
  assert(evaluate_qbf (Or(Var('x'), Exists('y', Or(Not(Var('z')), (Exists('x', And(Not(Var('x')),And(Var('y'),Top)))))))) env);
  Printf.printf "Question 6 ok.\n"