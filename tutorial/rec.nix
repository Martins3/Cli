# 一个 {} 是无法 reference 的，可以用 rec 来缓解
rec {
  x = y - 100;
  y = 123;
}.x
