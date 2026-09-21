(* implementation de la fonction pstree *)

open Printf

(* arbre de processus, N(s, pl) represente
le processus de nom s et qui appelle
les processus dans pl *)
type process = N of string * process list

let rec pstree (prefix: string) (N (s, pl): process): unit =
	printf "%s" s;
	(* prefix pour indentation *)
	let l = String.length s in (* w -> l *)
	(* nom de variable avec apostrophe *)
	let new_prefix = prefix ^ String.make (l+1) ' ' in
	match pl with
	| [] -> ()
	| [p] -> printf "---"; pstree (new_prefix ^ "  ") p
	| _ -> printf "-"; pstree_list new_prefix "+-" pl

and pstree_list (prefix: string) (start: string) (pl: process list): unit =
	match pl with (* plus clair avec match explicite *)
	| [] -> failwith "Cas liste vide impossible"
	| [p] ->
		printf "`-"; pstree (prefix ^ "  ") p
	| p::l ->
		printf "%s" start; pstree (prefix ^ "| ") p;
		printf "\n"; printf "%s" prefix;
		pstree_list prefix "|-" l
;;

let print (p: process): unit = pstree "" p
;;

let test () = print
	(N("main", [
		N("func1", []);
		N("func2", [
			N("malloc", [])
		]);
		N("scanf", [
			N("print", [
				N("malloc", [
					N("func3", []);
					N("func4", [
						N("malloc", [])
					]);
					N("func5", []);
				]);
				N("calloc", [
					N("realloc", [])
				])
			])
		]);
		N("print", [])
	])
)
