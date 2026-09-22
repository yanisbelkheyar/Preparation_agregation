(* Question 1 *)

let min (arr: 'a array) : 'a option =
    Array.fold_left (fun acc e -> match acc with
        None -> Some e
        | Some e2 -> if e < e2 then Some e else acc) None arr
;;

assert(min [||] = None);;
assert(min [|42|] = Some 42);;
assert(min [|0; 1; 2|] = Some 0);;
assert(min [|2; 1; 0|] = Some 0);;
assert(min [|41; 42; 40; 41; 40; 42|] = Some 40);;
assert(min[|0.5|] = Some 0.5);;
assert(min[|0.; 1.; 2.3|] = Some 0.);;
assert(min[|0.6; 0.56; 0.5|] = Some 0.5);;

let sum_arr (arr: int array) : int =
    Array.fold_left (fun acc e -> acc + e) 0 arr
;;

assert(sum_arr [||] = 0);;
assert(sum_arr [|42|] = 42);;
assert(sum_arr [|2; 2|] = 4);;
assert(sum_arr [|2; -3; 0|] = -1);;

exception NonTrie
let est_trie_croissant (arr: 'a array): bool =
    if (Array.length arr < 2) then true else
    try
        let _ = Array.fold_left (fun last e -> if e < last then raise NonTrie else e) arr.(0) arr in
        true
    with
        NonTrie -> false
;;

assert(est_trie_croissant [||]);;
assert(est_trie_croissant [|42|]);;
assert(est_trie_croissant [|0; 1; 2|]);;
assert(not (est_trie_croissant [|2; 1; 0|]));;
assert(not (est_trie_croissant [|2; 1; 3|]));;
assert(est_trie_croissant [|0.; 0.5; 0.5; 2014.32|]);;
assert(est_trie_croissant [|40; 40|]);;
assert(est_trie_croissant [|40; 41; 41; 42|]);;

let array_to_list (arr: 'a array) : 'a list =
    Array.fold_right (fun e acc -> e :: acc) arr []
;;

assert(array_to_list [||]  = []);;
assert(array_to_list [|0.3|]  = [0.3]);;
assert(array_to_list [|0; 1; 2; 3|]  = [0; 1; 2; 3]);;

let list_to_array (l: 'a list) : 'a array =
    let n = List.length l in
    let arr = Array.init n (fun _ -> List.hd l) in
    if n == 0 then arr else begin
        List.iteri (fun i e -> arr.(i) <- e) l;
        arr
    end
;;

assert(list_to_array [] = [||]);;
assert(list_to_array [0] = [|0|]);;
assert(list_to_array ['a'; 'b'; 'c'] = [|'a'; 'b'; 'c'|]);;

(* Exercice 2 *)
let incr_all_elems (arr: int array) : unit =
    Array.iteri (fun i x -> arr.(i) <- x + 1) arr
;;

let test_incr_all_elems (arr1: int array) (arr2: int array) : bool =
    incr_all_elems arr1;
    arr1 = arr2
;;

assert(test_incr_all_elems [||] [||]);;
assert(test_incr_all_elems [|42|] [|43|]);;
assert(test_incr_all_elems [|1; 2|] [|2; 3|]);;

let rec itere (f: 'a -> 'a) (n: int) (x: 'a): 'a =
    if n < 0 then
        failwith "itere with negative value"
    else if n == 0 then
        x
    else
        itere f (n - 1) (f x)
;;

assert(itere (fun n -> n * 2) 3 1 = 8);;
assert(itere (fun n -> n + 1) 4 0 = 4);;

let teste_preds (ps: ('a -> bool) list) (x: 'a) : bool list =
    List.map (fun p -> p x) ps
;;

assert(teste_preds [((>) 0); ((>) 1); ((>) 2); ((>) 3); ((<) 3); ((<) 2); ((<) 1); ((<) 0)] 2 = [false; false; false; true; false; false; true; true]);;

let sum (f: int -> int) (low: int) (high: int) =
    let rec sum_aux i acc =
        if i > high then acc else sum_aux (i + 1) (acc + f i)
    in sum_aux low 0
;;

assert (sum (fun i -> i) 1 100 = 5050);;
assert (sum (fun i -> i * i) 0 3 = 0 + 1 + 4 + 9);;

let exo_6 x () = x;;
(* Utile pour initialiser un tableau si on n'avait que Array.init plutôt que Array.make ?
   Pas exactement vu qu'il faut une fonction qui prend un int, mais quelque chose de ce genre pour une autre structure de donnée *)

let exo_6bis x =
    let count = ref 0 in
    ((fun () -> begin incr count; x end), count)
;;

let exo_6bisbis x =
    let count = ref 0 in
    ((fun () -> begin incr count; x end), (fun () -> !count))
;;

(* Exercice 7 *)
(* Liste doublement chainée, *MUTABLE* *)

type 'a double_linked_list = 
    Cell of {
        value: 'a;
        mutable prev: 'a double_linked_list;
        mutable next: 'a double_linked_list
    }
    | Nil ;;

let cons (x: 'a) (xs: 'a double_linked_list) : 'a double_linked_list =
    match xs with
        Nil -> Cell {value = x; prev = Nil; next = xs}
        | Cell c -> begin
            let old_prev = c.prev in
            let result = Cell {value = x; prev = old_prev; next = xs} in
            c.prev <- result;
            result
        end
;;

(* Insère x dans l, juste avant le premier élément pour lequel f renvoie vrai. Si f renvoie faux pour toute la liste, insère x à la toute fin *)
let rec insert_before_first_true (x: 'a) (f: 'a -> bool) (l: 'a double_linked_list) : 'a double_linked_list =
    match l with
    Nil -> cons x l
    | Cell c when f c.value -> begin
            c.prev <- Nil;
            cons x l
        end
    | Cell c -> begin
            c.next <- insert_before_first_true x f c.next;
            l
        end
;;

let list_to_double_linked (l: 'a list) : 'a double_linked_list =
    List.fold_right (fun x acc -> cons x acc) l Nil
;;

let rec fold_right (f: 'a -> 'acc -> 'acc) (l: 'a double_linked_list) (acc: 'acc) =
    match l with
    Nil -> acc
    | Cell c -> f c.value (fold_right f c.next acc)
;;

let double_linked_to_list (l: 'a double_linked_list) : 'a list =
    fold_right (fun x acc -> x :: acc) l []
;;

let test_on_double_linked (f: 'a double_linked_list -> 'a double_linked_list) (l_arg: 'a list) (l_expected: 'a list) : bool =
    double_linked_to_list (f (list_to_double_linked l_arg)) = l_expected
;;

assert(test_on_double_linked (insert_before_first_true 22 ((<) 25)) [] [22]);;
assert(test_on_double_linked (insert_before_first_true 22 ((<) 25)) [10; 20; 30; 40; 50] [10; 20; 22; 30; 40; 50]);;

let rec get_nth (n: int) (l: 'a double_linked_list): 'a double_linked_list =
    if n < 0 then failwith "get_nth, n < 0"
    else if n == 0 then
        l
    else match l with
        Nil -> failwith "get_nth, found Nil"
        | Cell c -> get_nth (n - 1) c.next
;;

let split_head (l: 'a double_linked_list) : ('a * ('a double_linked_list) * ('a double_linked_list)) option =
    match l with
        Nil -> None
        | Cell {value = x; next = old_next; prev = old_prev} ->
            begin match old_next with
                Nil -> ()
                | Cell c_next -> begin
                    assert(c_next.prev == l);
                    c_next.prev <- old_prev
                end
            end;
            begin match old_prev with
                Nil -> ()
                | Cell c_prev -> begin
                    assert(c_prev.next == l);
                    c_prev.next <- old_next
                end
            end;
            Some (x, old_prev, old_next) 
;;

(* Note: to test prevs, I'd have to add yet another function: reverse *)
assert(let l = list_to_double_linked [10; 20; 30; 40; 50] in
    let l2 = get_nth 2 l in
    match split_head l2 with
        None -> false
        | Some (x, _prevs, nexts) ->
            (x, double_linked_to_list nexts, double_linked_to_list l) = (30, [40; 50], [10; 20; 40; 50]));;

(* Exercice 8 *)

(* {All elements, as a min heap, and as a max heap, number of elements}
    The two arrays are always the same size, always >= than the number of elements
    The arrays have triplets: element, index in the min_heap, index in the max_heap *)
type 'a heap_elem = 'a * int ref * int ref;;
type 'a double_heap = 'a heap_elem array * 'a heap_elem array * int;;

let get_value ((e, _, _): 'a heap_elem) : 'a = e;;
let extract_min_idx ((_, r, _): 'a heap_elem) : int ref = r;; 
let extract_max_idx ((_, _, r): 'a heap_elem) : int ref = r;;

let peek (h: 'a heap_elem array) (n: int) : 'a option =
    if n = 0 then
        None
    else
        Some (get_value h.(0))
;;
let peek_min ((min_heap, _, n) : 'a double_heap) : 'a option = peek min_heap n;;
let peek_max ((_, max_heap, n) : 'a double_heap) : 'a option = peek max_heap n;;

let swap_element (h: 'a heap_elem array) (extract: 'a heap_elem -> int ref) (k1: int) (k2: int) : unit =
    let tmp_tuple = h.(k1) in
    let r1 = extract h.(k1) in
    let r2 = extract h.(k2) in
    assert(!r1 = k1);
    assert(!r2 = k2);
    h.(k1) <- h.(k2);
    h.(k2) <- tmp_tuple;
    r1 := k2;
    r2 := k1
;;

(* Triplet d'un tas, de la comparaison par lequel c'est un tas min, et une façon d'extraire l'indice correspondant des noeuds *)
type 'a min_or_max_heap = ('a heap_elem array) * ('a -> 'a -> bool) * ('a heap_elem -> int ref);;
let get_min_heap ((min_heap, _, _): 'a double_heap) : 'a min_or_max_heap = min_heap, (<), extract_min_idx;;
let get_max_heap ((_, max_heap, _): 'a double_heap) : 'a min_or_max_heap = max_heap, (>), extract_max_idx;;
let get_size ((_, _, n): 'a double_heap) : int = n;;

let fix_heap_from_leaf ((h, cmp, extract): 'a min_or_max_heap) (i: int) : unit =
    (* returns the parent, or the same index if already at the root *)
    let parent (k: int) : int = (k - 1) / 2 in
    let k = ref i in
    let k_parent = ref (parent !k) in
    while cmp (get_value h.(!k)) (get_value h.(!k_parent)) do
        swap_element h extract !k (!k_parent);
        k := !k_parent;
        k_parent := parent !k
    done

let bubble_up (h: 'a heap_elem array) (extract: 'a heap_elem -> int ref) (i: int) : unit =
    let parent (k: int) : int = (k - 1) / 2 in
    let k = ref i in
    let k_parent = ref (parent !k) in
    while !k <> !k_parent do
        swap_element h extract !k (!k_parent);
        k := !k_parent;
        k_parent := parent !k
    done

(* La taille minimale des tableaux s'ils sont non-vides *)
let min_heap_size = 16;;

let insert (x: 'a) (dh: 'a double_heap) : 'a double_heap =
    let (min, max, n) = dh in
    assert(Array.length min = Array.length max);
    let node = (x, ref n, ref n) in
    let (min2, max2) =
        if n = Array.length min then
            let n2 = if n < min_heap_size then 16 else 2 * n in
            (Array.init n2 (fun i -> if i < n then min.(i) else node), 
             Array.init n2 (fun i -> if i < n then max.(i) else node))
        else begin
            min.(n) <- node;
            max.(n) <- node;
            (min, max)
        end
    in
    let new_dh = (min2, max2, n + 1) in
    fix_heap_from_leaf (get_min_heap new_dh) n;
    fix_heap_from_leaf (get_max_heap new_dh) n;
    new_dh
;;

let rec build_double_heap (l: 'a list) : 'a double_heap =
    let make_empty_array () : 'a heap_elem array = Array.init 0 (fun _ -> (List.hd l, ref 0, ref 0)) in
    let empty_double_heap: 'a double_heap = (make_empty_array (), make_empty_array (), 0) in
    List.fold_left (fun (dh: 'a double_heap) (x: 'a) -> insert x dh) empty_double_heap l
;;

(* l non-vide *)
let test_build_peek (l: 'a list) : bool =
    let min = List.fold_left Stdlib.min (List.hd l) l in
    let max = List.fold_left Stdlib.max (List.hd l) l in
    let dh = build_double_heap l in
    (peek_min dh = Some min) && (peek_max dh = Some max)
;;

assert(test_build_peek [0]);;
assert(test_build_peek [0; 1]);;
assert(test_build_peek [1; 0]);;
assert(test_build_peek [3.0; 1.0; 2.0]);;
assert(test_build_peek [4; 1; 2; 1; 3; 0; 5; 3; 4; 1; 2; 1; 3; 0; 5; 3; 4; 1; 2; 1; 3; 0; 5; 3]);;

let fix_heap_from_root ((h, cmp, extract): 'a min_or_max_heap) (n: int) =
    (* returns the index of the son of k with the lowest/highest value, or returns k if it has no son *)
    let maxson (k: int) =
        let k1 = k*2 + 1 in
        let k2 = k*2 + 2 in
        if k1 >= n then
            k
        else if k2 >= n then
            k1
        else begin
            if cmp (get_value h.(k1)) (get_value h.(k2)) then
                k1
            else
                k2
        end
    in
    let k = ref 0 in
    let k_son = ref (maxson !k) in
    while cmp (get_value h.(!k_son)) (get_value h.(!k)) do
        swap_element h extract !k !k_son;
        k := !k_son;
        k_son := maxson !k
    done
;;

let pop ((h_primary, _, extract_primary) as primary: 'a min_or_max_heap)
        ((h_secondary, cmp_secondary, extract_secondary) as secondary: 'a min_or_max_heap) 
        (n: int)
        (rebuild_double_heap: 'a heap_elem array -> 'a heap_elem array -> int -> 'a double_heap)
            : ('a * ('a double_heap)) option =
    if n = 0 then
        None
    else begin
        let (x, _, _) as node = h_primary.(0) in
        let idx_primary = !(extract_primary node) in
        assert(idx_primary = 0);
        let new_n = n - 1 in
        swap_element h_primary extract_primary 0 new_n;
        fix_heap_from_root primary new_n;

        let idx_secondary = !(extract_secondary node) in
        (* Now, it is tempting to assume that the node is a leaf in secondary, since it was a root in primary
           But as Thomas showed me, that is unsound if there are lots of elements with the same value
            (see test: "repeat_pop_fun pop_min [0; 0; 0; 0; 0; 0; 1; 0] 4")
           So instead, first we bubble the node to the root
            , remove it from the root by swapping it with the last element
            , and fix the heap for good since it now has a wrong root *)
        bubble_up h_secondary extract_secondary idx_secondary;
        swap_element h_secondary extract_secondary 0 new_n;
        fix_heap_from_leaf secondary new_n;
        Some (x, (rebuild_double_heap h_primary h_secondary new_n))
    end 

let pop_min (dh: 'a double_heap) : ('a * ('a double_heap)) option =
    pop (get_min_heap dh) (get_max_heap dh) (get_size dh) (fun h_min h_max n -> (h_min, h_max, n))
;;
let pop_max (dh: 'a double_heap) : ('a * ('a double_heap)) option =
    pop (get_max_heap dh) (get_min_heap dh) (get_size dh) (fun h_max h_min n -> (h_min, h_max, n))
;;

(* Only useful for tests *)
let repeat_pop_fun (pop_fun: 'a double_heap -> ('a * ('a double_heap)) option) (l_input: 'a list) (n: int) : ('a list) * ('a double_heap) =
    let dh : 'a double_heap ref = ref (build_double_heap l_input) in
    let l : 'a list ref = ref [] in
    for i = 0 to n - 1 do
        match pop_fun !dh with
            None -> failwith "test_pop_min"
            | Some (x, new_dh) -> begin
                l := x :: !l;
                dh := new_dh
            end
    done;
    (List.rev !l, !dh)
;;
assert (let (l, _) = repeat_pop_fun pop_min [4; 1; 2; 1; 3; 0; 5; 3] 4 in l = [0; 1; 1; 2]);;
assert (let (l, _) = repeat_pop_fun pop_max [4; 1; 2; 1; 3; 0; 5; 3] 4 in l = [5; 4; 3; 3]);;
assert (let (l, _) = repeat_pop_fun pop_max [0; 0; 0; 0; 0; 0; 1; 0] 4 in l = [1; 0; 0; 0]);;
assert (let (l, _) = repeat_pop_fun pop_min [2; 1; 0; 2; 0; 1; 1; 2] 4 in l = [0; 0; 1; 1]);;


