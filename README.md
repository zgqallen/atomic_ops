==================================================================
atomic_ops(ADD/SUB/INC/DEC/AND/OR) for different platforms.
==================================================================</br>

Linux platform built with gcc, it will use builtin __sync_fetch*;</br>
For IBM AIX and PLinux using XLC, it will use xlc builtin __fetch_and*;</br>
For SUN X64 and SPARC64, it will use OS atomic function implemented in <atomic.h>; </br>
For HPUX64 , it will use __ATOMIC* implementation in  <machine/sys/builtins.h>;</br>
For WinX64, it will use Interlocked* implementation by windows and _Interlocked* by compiler builtin.</br>
