let
  nixpkgs = import <nixpkgs> { };
in
{
  a = nixpkgs.lib.stringLength "hello" ;
}
