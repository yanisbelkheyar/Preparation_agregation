let genere_compteur (): unit -> int =
	let cpt = ref (-1) in
	(fun () -> incr cpt; !cpt)

let () =
	let f = genere_compteur () in
	Printf.printf "%d\n%d\n%d\n" (f ()) (f ()) (f ())

let genere_get_set (v: int): (unit -> int) * (int -> unit) =
	let cpt = ref v in
	((fun () -> !cpt), (fun x -> cpt := x))

let () =
	let get, set = genere_get_set (6) in
	Printf.printf "%d\n" (get ());
	Printf.printf "%d\n%d\n" (set 5; get ()) (set 1; get ())

let version_memoisee (f: 'a -> 'b): 'a -> 'b =
	let tbl = Hashtbl.create 50 in
	(fun x ->
		match Hashtbl.find_opt tbl x with
		| None -> let res = f x in Hashtbl.add tbl x res; res
		| Some y -> y)

let () =
	let f = version_memoisee (fun x -> x*x) in
	Printf.printf "%d\n%d\n%d\n" (f 2) (f 6) (f 2)