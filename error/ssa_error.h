/*
 * ssa_error.h (Created on 2020-08-19)
 *
 * Copyright (C) 2020 Suryarpan Chowdhury
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <https://www.gnu.org/licenses/>.
 */

#ifndef SSA_ERROR_H
#define SSA_ERROR_H

#ifdef SSA_BUILD_GUARD
#  include "ssa_config.h"
#  include "ssa_export.h"
#elif !defined(SSA_INTERNAL_HEADER)
#  error Never include "ssa_error.h" directly. Use "ssa/ssa.h"
#endif /* !SSA_BUILD_GUARD */

#include <stdarg.h>
#include <stdio.h>

#undef BEGIN_DECLS
#undef END_DECLS
#ifdef __cplusplus
#  define BEGIN_DECLS                                                         \
    extern "C"                                                                \
    {
#  define END_DECLS }
#else
#  define BEGIN_DECLS /* empty */
#  define END_DECLS   /* empty */
#endif

BEGIN_DECLS

/*****************************************************************************
 *                   Error value enum and mapping to string                  *
 *****************************************************************************/

enum /* SSA error values */
{
  SSA_SUCCESS,  /* Return success */
  SSA_FAILURE,  /* Return failure */
  SSA_ERANGE,   /* output range error */
  SSA_EDOM,     /* input domain error */
  SSA_EFAULT,   /* faulty/invalid pointer */
  SSA_EINVAL,   /* invalid argument(s) provided */
  SSA_ESANITY,  /* sanity checked failed (very bad) */
  SSA_ENOMEM,   /* malloc or friends failed */
  SSA_ERUNAWAY, /* iteration out of control */
  SSA_EMAXITER, /* exceeded max number of iterations */
  SSA_EZERODIV, /* tried to divide by zero */
  SSA_EBADTOL,  /* user specified an invalid tolerance */
  SSA_ETOL,     /* failed to reach the specified tolerance */
  SSA_EBADLEN,  /* matrix, vector lengths are not conformant */
  SSA_ENOTSQR,  /* matrix not square */
  SSA_EOF       /* end of file */
};

/* Error type */
typedef const unsigned char ssa_error_t;

/* error value to string mapping */
const char *
ssa_strerror (ssa_error_t err_val);

/*****************************************************************************
 *                         Error Stream & Reporting                          *
 *****************************************************************************/

/* Stream handler type */
typedef void
ssa_stream_handler_t (const char *label, const char *file, int line,
                      const char *reason);

ssa_stream_handler_t *
ssa_set_stream_handler (ssa_stream_handler_t *new_handler);

FILE *
ssa_set_stream (FILE *new_stream);

/* Useful for printing to stream (default: stderr) */
void
ssa_stream_printf (const char *label, const char *file, int line,
                   const char *reason);

/*****************************************************************************
 *                     Error Handling Related Functions                      *
 *****************************************************************************/

/* Error handler type */
typedef void
ssa_error_handler_t (const char *reason, const char *file, int line,
                     ssa_error_t ssa_errno);

ssa_error_handler_t *
ssa_set_error_handler (ssa_error_handler_t *new_handler);

ssa_error_handler_t *
ssa_set_error_handler_off (void);

void
ssa_handle_error (const char *reason, const char *file, int line,
                  ssa_error_t ssa_errno);

/*****************************************************************************
 *                     Some useful error reporting define                    *
 *****************************************************************************/

/* This is for general purpose error reporting
 * Function must have following signature:
 * error_t
 * func(...){...}
 */
#define SSA_ERROR_RET(reason, ssa_errno)                                      \
  do                                                                          \
    {                                                                         \
      ssa_handle_error (reason, __FILE__, __LINE__, ssa_errno);               \
      return ssa_errno;                                                       \
    }                                                                         \
  while (0)

/* This is for error reporting from general function with any signature
 * [any return type]
 * func(...){...}
 */
#define SSA_ERROR_VAL(reason, ssa_errno, value)                               \
  do                                                                          \
    {                                                                         \
      ssa_handle_error (reason, __FILE__, __LINE__, ssa_errno);               \
      return value;                                                           \
    }                                                                         \
  while (0)

/* Error reporting from void returning function
 * void
 * func(...){...}
 */
#define SSA_ERROR_VOID(reason, ssa_errno)                                     \
  do                                                                          \
    {                                                                         \
      ssa_handle_error (reason, __FILE__, __LINE__, ssa_errno);               \
      return;                                                                 \
    }                                                                         \
  while (0)

/* This is for error reporting from malloc type function
 * void *
 * func(...){...}
 */
#define SSA_ERROR_NULL(reason, ssa_errno)                                     \
  SSA_ERROR_VAL (reason, ssa_errno, NULL)

END_DECLS

#endif // !SSA_ERROR_H
