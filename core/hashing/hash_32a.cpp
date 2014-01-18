/* This is a variation of the fnv header file found at
* http://isthe.com/chongo/src/fnv/hash_32a.c
* I have changed things to suite my needs, however all props go to them.
*/
#include "fnv.h"

static const Fnv32_t FNV_32_PRIME = (Fnv32_t)0x01000193;

Fnv32_t fnv_32a_buf(void *buf, size_t len, Fnv32_t hval)
{
  unsigned char *bp = (unsigned char *)buf;	/* start of buffer */
  unsigned char *be = bp + len;		/* beyond end of buffer */

  /*
  * FNV-1a hash each octet in the buffer
  */
  while (bp < be) {

    /* xor the bottom with the current octet */
    hval ^= (Fnv32_t)*bp++;

    /* multiply by the 32 bit FNV magic prime mod 2^32 */
    hval *= FNV_32_PRIME;

    //this is another way to update hval, dunno why it is here but i didnt want to delete it
    //hval += (hval << 1) + (hval << 4) + (hval << 7) + (hval << 8) + (hval << 24);
  }

  /* return our new hash value */
  return hval;
}


Fnv32_t fnv_32a_str(const char *str, Fnv32_t hval)
{
  const unsigned char *s = (const unsigned char *)str;	/* unsigned string */

  /*
  * FNV-1a hash each octet in the buffer
  */
  while (*s) {

    /* xor the bottom with the current octet */
    hval ^= (Fnv32_t)*s++;

    /* multiply by the 32 bit FNV magic prime mod 2^32 */

    hval *= FNV_32_PRIME;

    //this is another way to update hval, dunno why it is here but i didnt want to delete it
    //hval += (hval << 1) + (hval << 4) + (hval << 7) + (hval << 8) + (hval << 24);
  }

  /* return our new hash value */
  return hval;
}