
let rec min_liste l =
  match l with
  | []-> raise (Failure "Liste vide")
  | [x] -> x
  (* | x :: y -> min y;; *)
  | x :: y -> let next_min = min_liste y in if x<=next_min then x else next_min;;

let min_arr arr =
  let l = Array.length arr in 
  if l = 0 then failwith "Tableau vide"
  else
    let m = ref arr.(0) in 
    for i=1 to l-1 do
      if arr.(i)< !m then m:=arr.(i) 
    done;
    !m;;
    
let somme_arr arr =
  let sum= ref 0 in 
  Array.iter (fun x -> sum:=!sum+x) arr;
  !sum;;

let est_trie arr =
  let flag= ref true in 
  let l =Array.length arr in
  let i= ref 0 in
  while !flag && (!i<(l-1)) do
    flag:=!flag && arr.(!i)<arr.(!i+1);
    i:=!i+1;
  done;
  !flag;;

let dicho_moi (x:int) (arr: int array)=
let rec recur min max=
  if min=max then if arr.(min)=x then min else failwith "pas là"
  else if min > max then failwith "pas là"
  else 
    let mid = (max+min)/2 in
    if arr.(mid)>=x then
      recur min mid
    else recur (mid+1) max
  in recur 0 (Array.length arr);;

let dicho (x:'int) (t: int array): int =
  let rec dicho_ x min max t : int =
    if min = max then if t.(min) = x then min else raise Not_found
    else if min > max then raise Not_found
    else 
      let mid = (min+max)/2 in
      if t.(mid) >= x then dicho_ x min mid t
      else dicho_ x (mid+1) max t
  in
  let res = dicho_ x 0 (Array.length t) t in
  res
;;

let produit_facteurs (n: int)=
 let rec recur (m:int) (l:int list) =
  let q = ref 2 in
  let flag = ref false in
  while (not(!flag)) && !q<m do
    if ((m mod !q) = 0) then
      flag:=true
    else 
      q:=!q+1
  done;
  if !q=m then m::l else  recur (m/ !q) (!q::l)
in
recur n []
;;
    
let u = [|1; 2; 3; 4;10;15|];;
(* print_endline (string_of_int (produit_facteurs 15));; *)
List.iter print_endline (List.map string_of_int (produit_facteurs 14783245));;

