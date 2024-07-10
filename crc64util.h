#ifndef CRC64UTIL_H
#define CRC64UTIL_H


#include <stdlib.h>
#include <stdint.h>



/* Fill in the CRC-64 constants table. */
void crc64_init();

/**
 * Return the CRC-64 of buf[0..len-1] with initial crc, processing eight bytes
 * at a time.  This selects one of two routines depending on the endianess of
 * the architecture.  A good optimizing compiler will determine the endianess
 * at compile time if it can, and get rid of the unused code and table.  If the
 * endianess can be changed at run time, then this code will handle that as
 * well, initializing and using two tables, if called upon to do so.
 */
uint64_t crc64(uint64_t crc, void* buf, size_t len);








#endif // CRC64UTIL_H
