{
  description = "Logger for unix-like userspace applications";

  inputs = {
    nixpkgs.url = "nixpkgs/nixpkgs-unstable";
    flake-utils.url = "github:numtide/flake-utils";
  };

  outputs = { self, nixpkgs, flake-utils }:
  flake-utils.lib.eachDefaultSystem (system: let
      pkgs = import nixpkgs { inherit system; };
    in {
      packages.default = pkgs.stdenv.mkDerivation {
        name = "logoon";
        src = ./.;

        nativeBuildInputs = [ pkgs.clang ];

        installPhase = ''mkdir -p $out/bin; install -m0755 build/logoon $out/bin'';
      };
    }
  );
}

