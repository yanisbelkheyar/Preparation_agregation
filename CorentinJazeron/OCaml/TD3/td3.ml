module Var = String
type var = Var.t

type const = Top | Bot

type qbf =
  	| Var of var
  	| Const of const
  	| And of qbf * qbf
  	| Or of qbf * qbf
  	| Not of qbf
  	| E of var * qbf
  	| A of var * qbf

module VarSet = Set.Make (Var)

let rec libres (f: qbf): VarSet.t =
  	match f with
  	| Var x -> VarSet.singleton x
  	| Const _ -> VarSet.empty
  	| And (f1, f2) | Or (f1, f2) -> VarSet.union (libres f1) (libres f2)
  	| Not f' -> libres f'
  	| E (x, f') | A (x, f') -> VarSet.remove x (libres f')

let var_libres (f: qbf): var list = VarSet.elements (libres f)

let rec replace (x: var) (v: const) (f: qbf): qbf =
    match f with
    | Var y when y = x -> Const v
    | Var _ | Const _ -> f
    | And (f1, f2) -> And (replace x v f1, replace x v f2)
    | Or (f1, f2) -> Or (replace x v f1, replace x v f2)
    | Not f' -> Not (replace x v f')
    | E (y, _) | A (y, _) when y = x -> f   (* x est lié ici : on s'arrête *)
    | E (y, f') -> E (y, replace x v f')
    | A (y, f') -> A (y, replace x v f')

let () =
	let f = (And (Var "z", E ("x", Or (Var "x", Var "y")))) in
	assert (var_libres f = ["y"; "z"]);
	assert (replace "y" Top f = And (Var "z", E ("x", Or (Var "x", Const Top))));
	assert (var_libres (replace "z" Top f) = ["y"])

let rec expension (f: qbf): qbf =
	match f with
	| Var _ | Const _ -> f
	| And (f1, f2) -> And (expension f1, expension f2)
	| Or (f1, f2) -> Or (expension f1, expension f2)
	| Not f' -> Not (expension f')
	| E (x, f') -> let exp = expension f' in Or (replace x Top exp, replace x Bot exp)
	| A (x, f') -> let exp = expension f' in And (replace x Top exp, replace x Bot exp)

let rec eval (f: qbf): bool =
	if var_libres f <> [] then failwith "La formule n'est pas close"
	else let exp_f = expension f in
	let rec aux_eval (f: qbf): bool =
		match f with
		| Var _ | E _ | A _ -> failwith "La formule après expension contient
			encore des variables ou des quantificateurs"
		| Const Top -> true
		| Const Bot -> false
		| Or (f1, f2) -> (aux_eval f1) || (aux_eval f2)
		| And (f1, f2) -> (aux_eval f1) && (aux_eval f2)
		| Not f' -> not (aux_eval f')
	in aux_eval exp_f

let rec eval_env (f: qbf) (env: VarSet.t): bool =
	match f with
	| Var x when (VarSet.mem x env) = true -> true
	| Var x -> false
	| Const Top -> true
	| Const Bot -> false
	| Or (f1, f2) -> (eval_env f1 env) || (eval_env f2 env)
	| And (f1, f2) -> (eval_env f1 env) && (eval_env f2 env)
	| Not f' -> not (eval_env f' env)
	| E (x, f') -> (eval_env f' (VarSet.remove x env)) || (eval_env f' (VarSet.add x env))
	| A (x, f') -> (eval_env f' (VarSet.remove x env)) && (eval_env f' (VarSet.add x env))

let () =
  let xor a b = And (Or (a, b), Not (And (a, b))) in
  let g = A ("x", E ("y", xor (Var "x") (Var "y"))) in
  assert (eval g && eval_env g VarSet.empty);
  assert (not (eval (E ("x", A ("y", xor (Var "x") (Var "y"))))));
  assert (eval_env (Var "x") (VarSet.singleton "x"));
  assert (try ignore (eval (Var "x")); false with Failure _ -> true);
  assert (eval_env (A ("x", E ("x", Not (Var "x")))) VarSet.empty)