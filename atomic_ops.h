#ifndef __atomic_ops__
#define __atomic_ops__

/*
** generic/include/%M%:		%I%	%G%	%U%
**
**	This defines common atomic operations.
*/
#include <stdint.h>

/***************************************************************************
** Function prototypes
****************************************************************************/
/*
** syb_atomic_cas32
** Purpose:
**     Compare an int32_t value with oldval, if they are the same swap the value with newval.
**
** Parameters:
**     int32_t *ptr 
**         pointer to the value.
**     int32_t oldval 
**         the value to compare it to.
**     int32_t newval
**         what to swap it with if *ptr equals oldval.
**
** Returns:
**    the old value of *ptr.
**
*/
int32_t syb_atomic_cas32(volatile int32_t *ptr, int32_t oldval, int32_t newval);

/*
** syb_atomic_add<type>
** Purpose:
**  	Increase an addition of incr to the value stroed in ptr for specificed type.
**	If incr equals to 1, it will self-increament and return new value.
**
** Parameters:
**     <type> *ptr 
**         pointer to the value.
**     <type> incr
**	   addition needs to increament. 
**
** Returns:
**    the new value of *ptr if incr == 1, and possiable new or old value for
**    others depends on platform's implementation.
*/
int32_t syb_atomic_add32(volatile int32_t *ptr, int32_t incr);
int64_t syb_atomic_add64(volatile int64_t *ptr, int64_t incr);

/*
** syb_atomic_dec<type>
** Purpose:
**      Decrease an addition of decr to the value stroed in ptr for specificed type.
**      If decr equals to 1, it will self-decrement and return new value.
**
** Parameters:
**     <type> *ptr 
**         pointer to the value.
**     <type> decr
**         addition needs to decrement.
**
** Returns:
**    the new value of *ptr if incr == 1, and possiable new or old value for
**    others depends on platform's implementation.
*/
int32_t syb_atomic_dec32(volatile int32_t *ptr, int32_t decr);
int64_t syb_atomic_dec64(volatile int64_t *ptr, int64_t decr);

/*
** syb_atomic_and/or32
** Purpose:
**	The bitwise AND/OR of 32bit mask to a 32bit value stored in ptr.
**
** Parameters:
**     int32_t *ptr
**         pointer to the value.
**     int32_t mask
**         32bit mask need to AND/OR.
**
** Returns:
**    Return new value.
*/
int32_t syb_atomic_and32(volatile int32_t *ptr, int32_t mask);
int32_t syb_atomic_or32(volatile int32_t *ptr, int32_t mask);

/***************************************************************************
** Macro - External
****************************************************************************/
#ifndef __atomic__
#define __atomic__
#endif /* !__atomic__ */

/* ATOMIC_ADD & ATOMIC_SUB is 64-bit implementation by default - history legacy. */
#define ATOMIC_ADD(sum, incr) 		syb_atomic_add64((volatile int64_t *)(sum), (int64_t)(incr))
#define ATOMIC_SUB(sum, decr) 		syb_atomic_dec64((volatile int64_t *)(sum), (int64_t)(decr))
#define ATOMIC_ADD_32(sum, incr) 	syb_atomic_add32((volatile int32_t *)(sum), (int32_t)(incr))
#define ATOMIC_SUB_32(sum, decr) 	syb_atomic_dec32((volatile int32_t *)(sum), (int32_t)(decr))

#define ATOMIC_INC(val)			syb_atomic_add32((volatile int32_t *)(val), (int32_t)(1))
#define ATOMIC_DEC(val) 		syb_atomic_dec32((volatile int32_t *)(val), (int32_t)(1))
/* returns ++(*val) */
#define ATOMIC_INC_RETURN(val)		syb_atomic_add32((volatile int32_t *)(val), (int32_t)(1))
/* returns --(*val) */
#define ATOMIC_DEC_RETURN(val)		syb_atomic_dec32((volatile int32_t *)(val), (int32_t)(1))
#define ATOMIC_INC_BIGINT(val) 		syb_atomic_add64((volatile int64_t *)(val), (int64_t)(1))
#define ATOMIC_DEC_BIGINT(val)		syb_atomic_dec64((volatile int64_t *)(val), (int64_t)(1))

#define ATOMIC_AND(val, mask)		syb_atomic_and32((volatile int32_t *)(val), (int32_t)(mask))
#define ATOMIC_OR(val, mask)		syb_atomic_or32((volatile int32_t *)(val), (int32_t)(mask))

#if defined(CAS32)
#undef CAS32
#endif
#define CAS32(x, y, z) syb_atomic_cas32((volatile int32_t *)(x), (int32_t)(y), (int32_t)(z))

#define XLOCK_FREE      0       /* The lock is unlocked */
#define XLOCK_BUSY      1       /* The lock is locked */
#define XLOCK_LOCK(lv)  \
while (1) \
{ \
        while (lv == XLOCK_BUSY); \
        if (CAS32(&(lv), XLOCK_FREE, XLOCK_BUSY) == XLOCK_FREE) break; \
}

#define XLOCK_UNLOCK(lv)        lv = XLOCK_FREE;

#endif /* !__atomic_ops__ */

