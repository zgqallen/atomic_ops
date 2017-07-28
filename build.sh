##
#!/bin/bash

if [ $# != 1 ]; then
	echo "usage: build.sh LIX|AIX|PLIX|HPUX|SUNX64|SPARC64"
	exit
fi

case "$1" in
	"LIX")
		echo "gcc atomic_ops.c unittest.c -o unittest_lix -lpthread"
		gcc atomic_ops.c unittest.c -o unittest_lix -lpthread
		;;
	"AIX")
		echo "xlc atomic_ops.c unittest.c -q64 -o unittest_aix -lpthread"
		xlc atomic_ops.c unittest.c -q64 -o unittest_aix -lpthread
		;;
	"PLIX")
		echo "xlc atomic_ops.c unittest.c -q64 -o unittest_plix -lpthread"
		xlc atomic_ops.c unittest.c -q64 -o unittest_plix -lpthread
		;;
	"HPUX")
		echo "cc atomic_ops.c unittest.c -D__ia64 +DD64 -o unittest_hpux -lpthread"
		cc atomic_ops.c unittest.c -D__ia64 +DD64 -o unittest_hpux -lpthread
		;;
	"SUNX64")
		echo "/usr/global/opt/SunStudio12.2/bin/cc atomic_ops.c unittest.c -m64 -o unittest_sunx64 -lpthread"
		/usr/global/opt/SunStudio12.2/bin/cc atomic_ops.c unittest.c -m64 -o unittest_sunx64 -lpthread
		;;
	"SPARC64")
		echo "/usr/global/opt/SunStudio12.1/bin/cc atomic_ops.c unittest.c -m64 -xchip=ultra3 -o unittest_sunx64 -lpthread"
		/usr/global/opt/SunStudio12.1/bin/cc atomic_ops.c unittest.c -m64 -xchip=ultra3 -o unittest_sparc64 -lpthread
		;;
		
esac
