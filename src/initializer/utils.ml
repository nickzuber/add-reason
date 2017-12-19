
let my_add_one n = n + 1

let create_symlink alias directory = 
  (* let pwd = Unix.getcwd () in *)
  let pwd = "UNIX" in
  let f1 = pwd ^ directory in (* Clean this *)
  let f2 = pwd ^ "/node_modules/" ^ alias in  (* Clean this *)
  Printf.printf "ln -sv %s %s" f1 f2
  (* Unix.symlink f1 f2 *)