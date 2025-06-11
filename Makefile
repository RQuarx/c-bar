all:
	CC=clang CC_LD=lld meson setup -Dbuildtype=release target