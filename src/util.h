#ifndef __UTIL_H__
#define __UTIL_H__

/**
 * Flip the byte order in a double-precision float.
 * @param double A double-precision float.
 * @returns A double-precision float with the bytes flipped.
 */
double double_flip(double);

/**
 * Flip the byte order in a single-precision float.
 * @param float A single-precision float.
 * @returns A single-precision float with the bytes flipped.
 */
float float_flip(float);

/**
 * Flip the byte order in an integer.
 * @param int An integer.
 * @returns An integer with the bytes flipped.
 */
int int_flip(int);

/**
 * Flip the byte order in a short integer.
 * @param short A short integer.
 * @returns A short integer with the bytes flipped.
 */
short short_flip(short);

/**
 * A no-op stand-in for the flip function.
 * @param char A value to (not) flip.
 * @param int The length of the value to (not) flip, in bytes.
 */
void flip_noop(char*, int);

/**
 * Flips the bytes in a char string of the given length.
 * @param char A value to flip.
 * @param int The length of the value to flip, in bytes.
 */
void flip(char*, int);

#endif
