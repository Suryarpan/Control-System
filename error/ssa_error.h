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

#include "ssa_config.h"
#include "ssa_export.h"
#include <errno.h>
#include <stdarg.h>
#include <stdio.h>
#include <threads.h>

#undef __BEGIN_DECLS
#undef __END_DECLS
#ifdef __cplusplus
#  define __BEGIN_DECLS                                                       \
    extern "C"                                                                \
    {
#  define __END_DECLS }
#else
#  define __BEGIN_DECLS /* empty */
#  define __END_DECLS   /* empty */
#endif

__BEGIN_DECLS

#ifndef __STDC_LIB_EXT1__
typedef int error_t; /* This is the error type */
#endif

/*****************************************************************************
 *                   Error value enum and mapping to string                  *
 *****************************************************************************/

enum ssa_err_value
{
  SSA_SUCCESS  = 0,  /* Return success */
  SSA_FAILURE  = -1, /* Return failure */
  SSA_EDOM     = 1,  /* input domain error */
  SSA_ERANGE   = 2,  /* output range error */
  SSA_EFAULT   = 3,  /* faulty/invalid pointer */
  SSA_EINVAL   = 4,  /* invalid argument(s) provided */
  SSA_EFAILED  = 5,  /* general failure */
  SSA_ESANITY  = 6,  /* sanity checked failed (very bad) */
  SSA_ENOMEM   = 7,  /* malloc or friends failed */
  SSA_ERUNAWAY = 8,  /* iteration out of control */
  SSA_EMAXITER = 9,  /* exceeded max number of iterations */
  SSA_EZERODIV = 10, /* tried to divide by zero */
  SSA_EBADTOL  = 11, /* user specified an invalid tolerance */
  SSA_ETOL     = 12, /* failed to reach the specified tolerance */
  SSA_EUNDRFLW = 13, /* underflow */
  SSA_EOVRFLW  = 14, /* overflow  */
  SSA_EBADLEN  = 15, /* matrix, vector lengths are not conformant */
  SSA_ENOTSQR  = 16, /* matrix not square */
  SSA_EUNSUP   = 17, /* requested feature is not supported by the hardware */
  SSA_EUNIMPL  = 18, /* requested feature not (yet) implemented */
  SSA_ECACHE   = 19, /* cache limit exceeded */
  SSA_EOF      = 20  /* end of file */
};

/* err_val -> string mapping */
const char *
ssa_strerror (error_t err_val);

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

/* Useful for logging */
void
ssa_error_at_line (error_t ssa_errno, const char *file, int line,
                   const char *fmt, ...);

/*****************************************************************************
 *                     Error Handling Related Functions                      *
 *****************************************************************************/

/* Error handler type */
typedef void
ssa_error_handler_t (const char *reason, const char *file, int line,
                     error_t ssa_errno);

ssa_error_handler_t *
ssa_set_error_handler (ssa_error_handler_t *new_handler);

ssa_error_handler_t *
ssa_set_error_handler_off (void);

void
ssa_handle_error (const char *reason, const char *file, int line,
                  error_t ssa_errno);

/*****************************************************************************
 *                     Some useful error reporting define                    *
 *****************************************************************************/

#define SSA_ERROR_RET(reason, ssa_errno)                                      \
  do                                                                          \
    {                                                                         \
      ssa_handle_error (reason, __FILE__, __LINE__, ssa_errno);               \
      return ssa_errno;                                                       \
    }                                                                         \
  while (0)

#define SSA_ERROR_VAL(reason, ssa_errno, value)                               \
  do                                                                          \
    {                                                                         \
      ssa_handle_error (reason, __FILE__, __LINE__, ssa_errno);               \
      return value;                                                           \
    }                                                                         \
  while (0)

#define SSA_ERROR_VOID(reason, ssa_errno)                                     \
  do                                                                          \
    {                                                                         \
      ssa_handle_error (reason, __FILE__, __LINE__, ssa_errno);               \
      return;                                                                 \
    }                                                                         \
  while (0)

#define SSA_ERROR_NULL(reason, ssa_errno)                                     \
  SSA_ERROR_VAL (reason, ssa_errno, NULL)

#define SSA_ERROR_LOG(ssa_errno, fmt_str, ...)                                \
  ssa_error_at_line (ssa_errno, __FILE__, __LINE__, fmt_str, ...);

__END_DECLS

#endif // !SSA_ERROR_H
