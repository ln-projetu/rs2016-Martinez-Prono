#ifndef UTILS_H
#define UTILS_H


/**
 * This file contains all utils functions.
 *
 * @author Lucas MARTINEZ
 * @author Yann PRONO
 */


/**
 * @return the decimal value of an octal value.
 */
int octal_to_integer(int octal);
int octallong_to_integer(long octal);
void move_next_512b(int fd, int size, int write_mode);

#endif