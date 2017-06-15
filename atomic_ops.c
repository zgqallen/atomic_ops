/*
** generic/common/%M%:		%I%	%G%	%U%
**
** This file implements atomic functions for different platforms.
**
*/

#include "atomic_ops.h"

/* ------------------------- ATOMIC OPs for Winx64 -------------------------- */
#if defined(_MSC_VER) /* Microsoft Visual C++ */
#include <intrin.h>
#include <windows.h>
/*
** References:
** https://msdn.microsoft.com/en-us/library/1s26w950.aspx
*/
int32_t syb_atomic_cas32(volatile int32_t *ptr, int32_t oldval, int32_t newval)
{
	return _InterlockedCompareExchange(ptr, newval, oldval);
}

int32_t syb_atomic_add32(volatile int32_t *ptr, int32_t incr)
{
	if(incr == 1)
		return InterlockedIncrement(ptr);
	else
		return InterlockedExchangeAdd(ptr, incr);
}

int64_t syb_atomic_add64(volatile int64_t *ptr, int64_t incr)
{
	return InterlockedExchangeAdd64(ptr, incr);
}

int32_t syb_atomic_dec32(volatile int32_t *ptr, int32_t decr)
{
	if(decr == 1)
		return InterlockedDecrement(ptr);
	else
		return InterlockedExchangeAdd(ptr, -1*(decr));
}

int64_t syb_atomic_dec64(volatile int64_t *ptr, int64_t decr)
{
	return InterlockedExchangeAdd64(ptr, -1*(decr));
}

int32_t syb_atomic_and32(volatile int32_t *ptr, int32_t mask)
{
	return _InterlockedAnd(ptr, mask);
}

int32_t syb_atomic_or32(volatile int32_t *ptr, int32_t mask)
{
	return _InterlockedOr(ptr, mask);
}

/* ----------------------- ATOMIC OPs for Liunx with GCC ------------------- */
#elif defined(__GNUC__) && ( __GNUC__ * 100 + __GNUC_MINOR__ >= 401 )
/*
** References:
** https://gcc.gnu.org/onlinedocs/gcc-4.1.0/gcc/Atomic-Builtins.html
** https://gcc.gnu.org/onlinedocs/gcc/_005f_005fsync-Builtins.html
*/
int32_t syb_atomic_cas32(volatile int32_t *ptr, int32_t oldval, int32_t newval)
{
	return __sync_val_compare_and_swap(ptr, oldval, newval);
}

int32_t syb_atomic_add32(volatile int32_t *ptr, int32_t incr)
{
	return __sync_add_and_fetch(ptr, incr);
}

int64_t syb_atomic_add64(volatile int64_t *ptr, int64_t incr)
{
	return __sync_add_and_fetch(ptr, incr);
}

int32_t syb_atomic_dec32(volatile int32_t *ptr, int32_t decr)
{
	return __sync_sub_and_fetch(ptr, decr);
}

int64_t syb_atomic_dec64(volatile int64_t *ptr, int64_t decr)
{
	return __sync_sub_and_fetch(ptr, decr);
}

int32_t syb_atomic_and32(volatile int32_t *ptr, int32_t mask)
{
	return __sync_and_and_fetch(ptr, mask);
}

int32_t syb_atomic_or32(volatile int32_t *ptr, int32_t mask)
{
	return __sync_or_and_fetch(ptr, mask);
}

/* ----------------------- ATOMIC OPs for IBM AIX and PLinux ------------------ */
#elif defined(__xlC__) /* IBM XL C/C++ for both AIX and Linux*/
/*
** References:
** https://www.ibm.com/support/knowledgecenter/SSGH3R_13.1.2/com.ibm.xlcpp131.aix.doc/compiler_ref/bifs_sync_atomic.html
*/
int32_t syb_atomic_cas32(volatile int32_t *ptr, int32_t oldval, int32_t newval)
{
	/* __asm__ __volatile__ ("	sync \n" ::: "memory");*/
	__compare_and_swap((volatile int *)ptr, (int *)&oldval, newval);
	__isync();
	return oldval;
}

int32_t syb_atomic_add32(volatile int32_t *ptr, int32_t incr)
{
	if(incr == 1)
		return ((__fetch_and_add(ptr, incr)) + 1); /* returns ++(*val) */
	else
		return __fetch_and_add(ptr, incr);
	
}

int64_t syb_atomic_add64(volatile int64_t *ptr, int64_t incr)
{
        return __fetch_and_addlp(ptr, incr);
}

int32_t syb_atomic_dec32(volatile int32_t *ptr, int32_t decr)
{
	if(decr == 1)
        	return ((__fetch_and_add(ptr, -1*(decr))) - 1); /* returns --(*val) */
	else
		return __fetch_and_add(ptr, -1*(decr));
}

int64_t syb_atomic_dec64(volatile int64_t *ptr, int64_t decr)
{
        return __fetch_and_addlp(ptr, -1*(decr));
}

int32_t syb_atomic_and32(volatile int32_t *ptr, int32_t mask)
{
        return __fetch_and_and((volatile uint32_t*)ptr, mask);
}

int32_t syb_atomic_or32(volatile int32_t *ptr, int32_t mask)
{
        return  __fetch_and_or((volatile uint32_t*)ptr, mask);
}

/* ----------------------- ATOMIC OPs for SUN X64 and SPARC ------------------- */
#elif (defined(sun) || defined(__sun)) /* Solaris & sunx86 */
# include <atomic.h>
/*
** References:
** http://www.unix.com/apropos-man/all/0/atomic/
** http://www.polarhome.com/service/man/?qf=atomic_add_32_nv&af=0&tf=2&of=SunOS
** http://docs.oracle.com/cd/E23824_01/html/821-1465/atomic-cas-3c.html
*/
int32_t syb_atomic_cas32(volatile int32_t *ptr, int32_t oldval, int32_t newval)
{
	return atomic_cas_32((volatile uint32_t *)ptr, oldval, newval);
}

int32_t syb_atomic_add32(volatile int32_t *ptr, int32_t incr)
{
	if(incr == 1)
		return	atomic_inc_32_nv((volatile uint32_t *)ptr);  /* returns ++(*val) */
	else
		return  atomic_add_32_nv((volatile uint32_t *)ptr, incr);
}

int64_t syb_atomic_add64(volatile int64_t *ptr, int64_t incr)
{
	return atomic_add_64_nv((volatile uint64_t *)ptr, incr);
}

int32_t syb_atomic_dec32(volatile int32_t *ptr, int32_t decr)
{
	if(decr == 1)
		return atomic_dec_32_nv((volatile uint32_t *)ptr);  /* returns --(*val) */
	else
		return atomic_add_32_nv((volatile uint32_t *)ptr, -1*(decr));
}

int64_t syb_atomic_dec64(volatile int64_t *ptr, int64_t decr)
{
	return atomic_add_64_nv((volatile uint64_t *)ptr, -1*(decr));
}

int32_t syb_atomic_and32(volatile int32_t *ptr, int32_t mask)
{
	return atomic_and_32_nv((volatile uint32_t *)ptr, mask);
}

int32_t syb_atomic_or32(volatile int32_t *ptr, int32_t mask)
{
	return atomic_or_32_nv((volatile uint32_t *)ptr, mask);
}

/* ----------------------- ATOMIC OPs for HPUX X64  --------------------------- */
#elif defined(__hpux) && (defined(_HPUX_SOURCE) || defined(__HP_cc)) /* HP-UX */
#include <machine/sys/inline.h>
#include <machine/sys/builtins.h> /* LONG for 32 bit and QUAD for 64bit */
/*
** References:
** http://citeseerx.ist.psu.edu/viewdoc/download?doi=10.1.1.129.5445&rep=rep1&type=pdf
*/
int32_t syb_atomic_cas32(volatile int32_t *ptr, int32_t oldval, int32_t newval)
{
	/* Move the old value into register _AREG_CCV because
	** this is the register that ptr will be compared against 
	*/
	_Asm_mov_to_ar(_AREG_CCV, oldval);
	/* Do the compare and swap */
	return _Asm_cmpxchg(_SZ_W, _SEM_ACQ, ptr, newval, _LDHINT_NONE);
}

int32_t syb_atomic_add32(volatile int32_t *ptr, int32_t incr)
{
	if(incr == 1)
		return (__ATOMIC_INCREMENT_LONG(ptr) + 1); /* returns ++(*val) */
	else
		return __ATOMIC_ADD_LONG(ptr, incr);
}

int64_t syb_atomic_add64(volatile int64_t *ptr, int64_t incr)
{
	return __ATOMIC_ADD_QUAD(ptr, incr);
}

int32_t syb_atomic_dec32(volatile int32_t *ptr, int32_t decr)
{
	if(decr == 1)
		return (__ATOMIC_DECREMENT_LONG(ptr) - 1); /* returns --(*val) */
	else
		return (__ATOMIC_ADD_LONG(ptr, -1*(decr)));
}

int64_t syb_atomic_dec64(volatile int64_t *ptr, int64_t decr)
{
	return (__ATOMIC_ADD_QUAD(ptr, -1*(decr)));
}

int32_t syb_atomic_and32(volatile int32_t *ptr, int32_t mask)
{
	return __ATOMIC_AND_LONG(ptr, mask);
}

int32_t syb_atomic_or32(volatile int32_t *ptr, int32_t mask)
{
	return __ATOMIC_OR_LONG(ptr, mask);
}

#else
# error "ATOMIC not supported on this platform."
#endif
