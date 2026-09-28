#use "topfind";;
#require "graphics";; 

open  Graphics;;
open Float;;

let () = 
let w=800 and h=800 in
let window=" "^(string_of_int w)^"x"^(string_of_int h) in


let trace_drapeau () =
Graphics.set_color (blue) ;
Graphics.fill_rect 0 0 w h ;
Graphics.set_color (yellow);
let fw=float_of_int w in
let fh=float_of_int h in
let x_star rad = int_of_float ((fw/.2.)+.((fw/.4.) *.  cos ((float_of_int rad) *. pi /. 6.))) in
for i=0 to 12 do
  Graphics.fill_circle (x_star i)
  (int_of_float ((fh/.2.)+.(fw/.4.) *.  (sin ((float_of_int i) *. pi /. 6.)))) 40;
done;
ignore (Graphics.read_key ());
Graphics.close_graph () in

let draw_diag ()=
for i=0 to h do
  Graphics.plot i i;
  Unix.sleepf 0.1 
done
in

(* let draw_rond () =
  Graphics.wait_next_event [Key_pressed] in
  Graphics.fill_circle 500 400 40
in *)

Graphics.open_graph window ;
draw_diag ();
ignore (Graphics.read_key ());
