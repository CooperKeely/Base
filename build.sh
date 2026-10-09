#!/bin/bash

set -eu
cd "$(dirname "$0")"

# --- Unpack Arguments
for arg in "$@"; do declare $arg='1'; done

if 	[[ ! -v release  && ! -v profile ]] ;	
	then debug=1; 
elif	[ -v release ] 				
	then unset debug; 
fi

if 	[ ! -v gcc ];				then clang=1; fi


if 	[[ -v asan && ! -v clang ]]; 	then echo "[asan requires clang]"; exit 1
elif    [[ -v asan && -v clang ]];      then asan_flags="-fsanitize=address -fno-omit-frame-pointer"; echo "[asan enabled]";
else 					asan_flags="";fi

if 	[ -v debug ];			then echo "[debug mode]"; fi
if 	[ -v release ];			then echo "[release mode]"; fi
if 	[ -v verbose ];			then echo "[verbose on]"; fi
if 	[ -v clang ];			then compiler="${CC=clang}"; echo "[clang compile]"; fi
if 	[ -v gcc ];			then compiler="${CC=gcc}"; echo "[gcc compile]"; fi


# --- Unpack Command Line Build Arguments
auto_compile_flags=''

# --- This will be unused
# --- Get current Git commit id
git_hash=$(git describe --always --dirty)
git_hash_full=$(git rev-parse HEAD)

# --- Compile/Link Definitions
#-fno-omit-frame-pointer 
clang_common="-I../src -I ../include -std=c99 -rdynamic -DBUILD_GIT_HASH=\"$git_hash\" -DBUILD_GIT_HASH_FULL=\"$git_hash_full\" -Wall -Wextra -Wno-unused-function -Wno-unused-value -Wno-unused-variable -Wno-unused-parameter"

clang_debug="$compiler ${asan_flags} -g3 -O0 ${clang_common} ${auto_compile_flags}"
clang_release="$compiler ${asan_flags} -g -O3 -DBUILD_DEBUG=0 ${clang_common} ${auto_compile_flags}"
clang_profile="$compiler -pg -O0 -DBUILD_DEBUG=0 ${clang_common} ${auto_compile_flags}"
clang_link="-lm -ldl -lrt"
clang_out="-o"

gcc_common="-I../src -I../include -std=c99 -rdynamic -DBUILD_GIT_HASH=\"$git_hash\" -DBUILD_GIT_HASH_FULL=\"$git_hash_full\" -Wall -Wextra -Wno-unused-function -Wno-unused-value -Wno-unused-variable -Wno-unused-parameter"

gcc_debug="$compiler -g3 -O0 ${gcc_common} ${auto_compile_flags}"
gcc_release="$compiler -g -O3 -DBUILD_DEBUG=0 ${gcc_common} ${auto_compile_flags}"
gcc_profile="$compiler -g -O3 -DBUILD_DEBUG=0 ${gcc_common} ${auto_compile_flags}"
gcc_link="-lm -ldl -lrt"
gcc_out="-o"

# --- Per-Build Settings
link_os_linux_x11_gfx="-lxcb -lxcb-shm -lxcb-image -lxcb-icccm"
link_os_linux_wayland_gfx="-lwayland-client -lwayland-egl -lEGL"

link_os_gfx="$link_os_linux_wayland_gfx"
compile_xdg_gfx="include/xdg-shell-client-protocol.c"

# --- Choose Compile/Link lines
if [ -v gcc ]; then
	compile_debug="$gcc_debug";
	compile_release="$gcc_release";
	compile_profile="$gcc_profile";
	compile_link="$gcc_link";
	out="$gcc_out";
elif [ -v clang ]; then
	 compile_debug="$clang_debug";
 	 compile_release="$clang_release";
	 compile_profile="$clang_profile";
 	 compile_link="$clang_link";
 	 out="$clang_out";
fi

if [ -v release ]; then
	compile="$compile_release";
elif [ -v profile ]; then
	compile="$compile_profile";
else
	compile="$compile_debug"; 
fi

# --- Prep directories
mkdir -p src
mkdir -p include
mkdir -p build

if [ -v clean ]; then 
	echo "[clean build]"
	rm -rf build/*
fi

# --- Build Everything
if [ -v main ]; then 
	didbuild=1 
	echo ""

	cmd="$compile src/main.c $compile_xdg_gfx $compile_link $link_os_gfx $out build/app"
	if [ -v verbose ]; then echo "[Command]: $cmd"; fi

	$cmd

	echo ""
elif [ -v token ]; then 
	didbuild=1 
	echo ""

	ecmd="$compile src/token_main.c $compile_link $link_os_gfx $out build/app"
	if [ -v verbose ]; then echo "[Command]: $cmd"; fi

	$cmd

	echo ""
fi

if [ ! -v didbuild ]; then
	echo "[Warning: no valid build target specified]"
	exit 1
fi


if [ -v didbuild ] && [ -v run ]; then 
	echo "[running build]"
	./build/app; 
else
	echo "[just building no run target specified]"
fi
