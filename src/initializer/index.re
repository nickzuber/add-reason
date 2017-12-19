
module Shell = {
  type t = (string, int);

  let test = "test";
  let main = () => {
     let argv = ["a", "b", "c", "d"];
     let _ = List.map((x) => print_endline(x), argv);
  };

  let parse = (argv) => {
     let _ = List.map((x) => print_endline(x), argv);
  };

  let handleInit = () => Init.handle;
};

let add1 = (n) => n + 1;
