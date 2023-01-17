{ pkgs }: {
	deps = [
    pkgs.curlFull.dev
    pkgs.clang_12
		pkgs.ccls
		pkgs.gdb
		pkgs.gnumake
    pkgs.curlpp
    pkgs.fmt
	];
}