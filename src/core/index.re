
let coreAttribute = "Core attribute with " ++ MyModule.someAttribute;

module Shell = {
  type t = (string, int);

  let test = "test";
  let main = (n) => {
    /* let argv = Sys.argv; */
    n + 1
  };
};

let add1 = (n) => n + 1;
