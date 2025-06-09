# shell.nix
{ pkgs ? import <nixpkgs> {} }:

pkgs.mkShell {
  buildInputs = [
    pkgs.lua54Packages.lua
    pkgs.sdl3
    pkgs.sdl3-ttf
    pkgs.pkg-config
  ];

  shellHook = ''
    echo "Welcome to the MoonCon dev environment!"
  '';
}
