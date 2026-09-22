type var = string

type p = 
| VAR of var 
| AND of p * p
| OR of p * p
| NOT of p
| EXIST of var * p
| FORALL of var * p;;


let variables_libres (f:p) : (var list) = 
    let rec trouver_variables (f:p) (variables_liee:var list) : (var list) = 
    match f with 
    | VAR (x:var) -> if not (List.mem x variables_liee) then [x] else []
    | EXIST ((x:var),(f:p)) -> (trouver_variables f (x::variables_liee))
    | FORALL ((x:var),(f:p)) -> (trouver_variables f (x::variables_liee))
    | AND ((f1:p),(f2:p)) -> (trouver_variables f1 variables_liee) @ (trouver_variables f2 variables_liee)
    | OR ((f1:p),(f2:p)) -> (trouver_variables f1 variables_liee) @ (trouver_variables f2 variables_liee)
    | NOT (f:p) -> (trouver_variables f variables_liee)
    in 
    trouver_variables f []


type substitution = {
    variable : var;
    constant : var;
};;


let substitue_variable_constante (sub:substitution) (f:p) : p = 
    let rec substitue (sub:substitution) (f:p) (variables_liee:var list) : (p) = 
    match f with 
    | VAR (x:var) -> if not (List.mem x variables_liee) && x = sub.variable then VAR(sub.constant) else VAR(x)
    | EXIST ((x:var),(f:p)) -> EXIST(x,(substitue sub f (x::variables_liee)))
    | FORALL ((x:var),(f:p)) -> FORALL(x,(substitue sub f (x::variables_liee)))
    | AND ((f1:p),(f2:p)) -> AND((substitue sub f1 variables_liee), (substitue sub f2 variables_liee))
    | OR ((f1:p),(f2:p)) -> OR((substitue sub f1 variables_liee), (substitue sub f2 variables_liee))
    | NOT (f:p) -> NOT(substitue sub f variables_liee)
    in 
    substitue sub f []


  let expend (f:p) : p = 
    let rec expend_and_substitue (sub_list:substitution list) (f:p) : (p) = 
    match f with 
    | VAR (x:var) -> begin 
        let resultat = List.find_index (fun y -> y.variable = x) sub_list in 
        match resultat with
        | None -> VAR(x)
        | Some index -> VAR((List.nth sub_list index).constant)
        end
    | EXIST ((x:var),(f:p)) -> OR(
    (expend_and_substitue ({variable=x;constant="vrai"}::sub_list) f ),
    (expend_and_substitue ({variable=x;constant="faux"}::sub_list) f ))
    | FORALL ((x:var),(f:p)) -> AND(
    (expend_and_substitue ({variable=x;constant="vrai"}::sub_list) f ),
    (expend_and_substitue ({variable=x;constant="faux"}::sub_list) f ))
    | AND ((f1:p),(f2:p)) -> AND(
    (expend_and_substitue sub_list f1), 
    (expend_and_substitue sub_list f2))
    | OR ((f1:p),(f2:p)) -> OR(
    (expend_and_substitue sub_list f1), 
    (expend_and_substitue sub_list f2))
    | NOT (f:p) -> NOT(expend_and_substitue sub_list f)
    in 
    expend_and_substitue [] f



let est_close (f:p) : bool = 
    let list_variable_libre = variables_libres f in
    match list_variable_libre with
    | [] -> true
    | _ -> false

let evaluation (f:p) : (p) = 
    if est_close f then
        let rec applique_operateur (f:p) : var = 
            match f with 
            | VAR (x:var) -> x
            | AND ((f1:p),(f2:p)) -> if ((applique_operateur f1)="vrai") && ((applique_operateur f2) = "vrai") 
            then "vrai" else "faux"
            | OR ((f1:p),(f2:p)) -> if(applique_operateur f1)="vrai" || (applique_operateur f2)="vrai"
            then "vrai" else "faux"
            | NOT (f:p) -> if (applique_operateur f)="vrai"
            then "faux" else "vrai"

        in 
        VAR(applique_operateur (expend f))
    else 
        f


let est_close (f:p) : bool = 
    let list_variable_libre = variables_libres f in
    match list_variable_libre with
    | [] -> true
    | _ -> false



    open Stdlib


let evaluation_total (f:p) (environement:substitution list) : (p) = 
    let variable_libre: var list = variables_libres f in
    let substitution_variable: var list = List.map (fun x -> x.variable) environement in
    begin
        for i = 0 to (List.length variable_libre)-1 do
            assert(List.mem (List.nth variable_libre i) substitution_variable)
        done
    end;
    let rec substitue_tout_variable (f:p) (environement:substitution list): p = 
        match environement with
        | [] -> f
        | sub::rest -> substitue_tout_variable (substitue_variable_constante sub f) rest
    in 
    let formule_sans_var_libre: p = substitue_tout_variable (expend f) environement in
    let rec applique_operateur (f:p) : var = 
            match f with 
            | VAR (x:var) -> x
            | AND ((f1:p),(f2:p)) -> if ((applique_operateur f1)="vrai") && ((applique_operateur f2) = "vrai") 
            then "vrai" else "faux"
            | OR ((f1:p),(f2:p)) -> if(applique_operateur f1)="vrai" || (applique_operateur f2)="vrai"
            then "vrai" else "faux"
            | NOT (f:p) -> if (applique_operateur f)="vrai"
            then "faux" else "vrai"

        in 
        VAR(applique_operateur formule_sans_var_libre)


let f: p = OR(NOT(VAR("a")),VAR("b"));;
let f1: p = FORALL(("a"),OR(NOT(VAR("a")),VAR("b")));;
let f2: p = OR(VAR("a"),EXIST(("a"),OR(NOT(VAR("a")),VAR("b"))));;

evaluation_total f [{variable = "a"; constant = "false"};{variable = "b"; constant = "false"}] ;;
evaluation_total f1 [{variable = "a"; constant = "false"}] ;;
evaluation_total f2 [{variable = "a"; constant = "false"}] ;;