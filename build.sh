#!/bin/bash

set -eu
cd "$(dirname "$0")"

# --- Unpack Arguments
for arg in "$@"; do declare $arg='1'; done
if 	[ ! -v release ];		then debug=1; fi
if 	[ ! -v clang ];			then gcc=1; fi
if 	[ -v asan ];			then asan=1; fi
if 	[ -v debug ];			then echo "[debug mode]"; fi
if 	[[ -v asan && ! -v clang ]]; 	then echo "[asan requires clang]"; exit 1
else					asan_flags="-fsanitize=address -fno-omit-frame-pointer"; echo "[asan enabled]"; fi
if 	[ -v release ];			then echo "[release mode]"; fi
if 	[ -v clang ];			then compiler="${CC=clang}"; echo "[clang compile]"; fi
if 	[ -v gcc ];			then compiler="${CC=gcc}"; echo "[gcc compile]"; fi

# --- Unpack Command Line Build Arguments
auto_compile_flags=''

# --- This will be unused
# --- Get current Git commit id
git_hash=$(git describe --always --dirty)
git_hash_full=$(git rev-parse HEAD)

# --- Compile/Link Definitions

clang_common="-I../src -I../include -std=c99 -rdynamic -DBUILD_GIT_HASH=\"$git_hash\" 
		-DBUILD_GIT_HASH_FULL=\"$git_hash_full\" -Wall -Wextra -Wno-unused-function 
		-Wno-unused-value -Wno-unused-variable -Wno-compare-distinct-pointer-types"

clang_debug="$compiler ${asan_flags} -g -O0 ${clang_common} ${auto_compile_flags}"
clang_release="$compiler ${asan_flags} -g -O3 -DBUILD_DEBUG=0 ${clang_common} ${auto_compile_flags}"
clang_link="-lrt"
clang_out="-o"

gcc_common="-I../src -I../include -std=c99 -rdynamic -DBUILD_GIT_HASH=\"$git_hash\" 
		-DBUILD_GIT_HASH_FULL=\"$git_hash_full\" -Wall -Wextra -Wno-unused-function 
		-Wno-unused-value -Wno-unused-variable -Wno-compare-distinct-pointer-types"

gcc_debug="$compiler -g -O0 ${gcc_common} ${auto_compile_flags}"
gcc_release="$compiler -g -O3 -DBUILD_DEBUG=0 ${gcc_common} ${auto_compile_flags}"
gcc_link="-lrt"
gcc_out="-o"

# --- Per-Build Settings
link_os_gfx="-lX11 -lXext"

# --- Choose Compile/Link lines
if [ -v gcc ];			then compile_debug="$gcc_debug"; fi
if [ -v gcc ];			then compile_release="$gcc_release"; fi
if [ -v gcc ];			then compile_link="$gcc_link"; fi
if [ -v gcc ];			then out="$gcc_out"; fi
if [ -v clang ]; 		then compile_debug="$clang_debug"; fi
if [ -v clang ]; 		then compile_release="$clang_release"; fi
if [ -v clang ]; 		then compile_link="$clang_link"; fi
if [ -v clang ]; 		then out="$clang_out"; fi
if [ -v debug ]; 		then compile="$compile_debug"; fi
if [ -v release ];		then compile="$compile_release"; fi


# --- Prep directories
mkdir -p src
mkdir -p include
mkdir -p build

if [ -v clean ]; then echo "[clean build]"; rm -rf build/*; fi

# --- Build Everything
cd build
if [ -v main ]; 	then didbuild=1 && $compile ../src/main.c $compile_link $link_os_gfx $out app; fi

if [ ! -v didbuild ]; then
	echo "[Warning] no valid build target specified";
fi

cd ..

if [ -v didbuild ] && [ -v run ]; then 
	echo "[running build]"; 
	echo "";
	./build/app; 
fi
