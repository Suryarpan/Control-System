#ifndef CTRL_ERROR_H
#define CTRL_ERROR_H

#include <errno.h>

/* define errno */
#define ctrl_errno errno

/* Following are the list of error codes */

/* Memory and Datatype related error */
#define CTRL_ENOMEM ENOMEM /* No memory */

/* Math related error */
#define CTRL_EDOM EDOM     /* Numerical error with domain of input */
#define CTRL_ERANGE ERANGE /* Out of array range */

/* Array related error (self defined) */
#define CTRL_EARRLEN 500 /* Bad array length */
#define CTRL_EARRLEN_STR "Array length is bad"

#define CTRL_EMATSIZE 501 /* Bad matrix size */
#define CTRL_EMATSIZE_STR "Matrix sizes not compatible"

void ctrl_perror(const char *s);

#endif // !CTRL_ERROR_H