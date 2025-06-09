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
   echo "__   __   ___     ___   __    _"
   echo "|    |  .'   \`. .'   \`. |\\   |"
   echo "|\\  /|  |     | |     | | \\  |"
   echo "| \\/ |  |     | |     | |  \\ |"
   echo "/    /   \`.__.'  \`.__.' |   \\|"
   echo "                              "
   echo "                      ___    ___   __    _"
   echo "                    .'   \\ .'   \`. |\\   |"
   echo "                    |      |     | | \\  |"
   echo "                    |      |     | |  \\ |"
   echo "                     \`.__,  \`.__.' |   \\|"
'';

}
