let
  s = { foo = "bar"; a-b = "baz"; "123" = "num"; };
  longName = { a = 3; b = 4; };
  double1 = x: x * 2;
  double = { a, b }: a * b;
  mul = s@{ a, b, ... }: a * b * s.c;
  module = import ./module.nix;
  myattrset = { a = 1; b = 2; };
in
with myattrset;
{
  longName.a = 4;
  s.a-b = "gg";
  s.foo = "G";
  /* longName.b = double { a = 4; b = 10; }; */
  longName.b = mul { a = 4; b = 10; c = 10; };
  a = module { a = 5; b = 4; };

  current = builtins.currentSystem;

  /* d = derivation { name = "myname"; builder = "mybuilder"; system = "mysystem"; }; */

  src = { a = a; };

  union = { a = "b"; } // { c = "d"; };
}



/*
  lec
  object = {
  foo = "foo val";
  bar = "bar val";
  };
  in
  with object; [
  foo
  bar
  ]
*/
