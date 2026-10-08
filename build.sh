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


if      [[ -v gdb && -v run  ]];        then echo "[gdb and run are mutually exclusive]"; exit 1
elif 	[[ -v gdb && ! -v asan  && ! -v run ]];      then echo "[gdb]"; fi
if 	[[ -v asan && ! -v clang ]]; 	then echo "[asan requires clang]"; exit 1
elif    [[ -v asan && -v clang ]];      then asan_flags="-fsanitize=address -fno-omit-frame-pointer"; echo "[asan enabled]";
else 					asan_flags="";fi

if 	[ -v debug ];			then echo "[debug mode]"; fi
if 	[ -v release ];			then echo "[release mode]"; fi
if 	[ -v profile ];			then echo "[profile mode]"; fi
if 	[ -v cahe ];			then echo "[performing cache audit]"; fi
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
link_os_linux_wayland_gfx="-lwayland-client"

link_os_gfx="$link_os_linux_wayland_gfx"

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
mkdir -p ./prof
mkdir -p ./prof/raw
mkdir -p ./prof/data

if [ -v clean ]; then 
	echo "[clean build]"
	rm -rf build/*
fi

# --- Build Everything
if [ -v main ]; then 
	didbuild=1 
	echo ""
	cmd="$compile src/main.c $compile_link $link_os_gfx $out build/app"
	if [ -v verbose ]; then echo "[Command]: $cmd"; fi
	$cmd
	echo ""
elif [ -v token ]; then 
	didbuild=1 
	echo ""
	$compile src/token_main.c $compile_link $link_os_gfx $out build/app 
	echo ""
fi

if [ ! -v didbuild ]; then
	echo "[Warning: no valid build target specified]"
	exit 1
fi

cd ..

if [ -v didbuild ] && [ -v run ]; then 
	echo "[running build]"
	./build/app; 
elif [ -v didbuild ] && [ -v gdb ]; then
	echo "[running gdb]"
	gdb -q -x env/debug_setup.gdb --args ./build/app 
elif [ -v didbuild ] && [ -v profile ]; then
	
	echo "[running profile]"
	perf record -F 999 -o ./prof/raw/perf.data -g ./build/app

	if [ -f gmon.out ]; then
		mv gmon.out ./prof/raw
	fi

	echo "[results saved to prof_profile_summary.txt]"
	perf report --header -i ./prof/raw/perf.data --stdio > ./prof/data/prof_profile_summary.txt
	
	echo "[--- Profile Restults ---]"
	if [ -v verbose ]; then
		less -S ./prof/data/prof_profile_summary.txt
	else
		echo "[run with build option 'verbose' to automatically show profile results]"
		grep -E "^[[:space:]]+[0-9]+\.[0-9]+%" ./prof/data/prof_profile_summary.txt | head -n 5
	fi

	echo ""
elif [ -v didbuild ] && [ -v cache ]; then
	
	echo "[running cache audit]"
	perf stat -d -r 3 -o ./prof/data/prof_cache_audit_summary.txt ./build/app

	echo "[recording cache miss locations]"
	perf record -e cache-misses,bus-cycles -o ./prof/raw/cache.data ./build/app 

	if [ -f gmon.out ]; then
		mv gmon.out ./prof/raw
	fi

	echo "[results saved to prof_cache_audit_summary.txt]"
	perf report -i ./prof/raw/cache.data --stdio  --percent-limit 0.1 >> ./prof/data/prof_cache_audit_summary.txt
	
	echo "[--- Cache Audit Restults ---]"
	if [ -v verbose ]; then
		less -SR ./prof/data/prof_cache_audit_summary.txt
	else
		echo "[run with build option 'verbose' to automatically show cache audit results]"
		grep "cache-misses" ./prof/data/prof_cache_audit_summary.txt | head -n 3
	fi
	echo ""
else
	echo "[just building no run target specified]"
fi
