/*
 * ssa_log.c (Created on 2021-02-26)
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

#include "ssa_error.h"
#include <stdarg.h>
#include <time.h>

FILE * ssa_log_stream SSA_NO_EXPORT = NULL;

__attribute__ ((__format__ (__printf__, 4, 5))) void
ssa_error_at_line (error_t ssa_errno, const char *file, int line,
                   const char *fmt, ...)
{
  fflush (stdout); /* Needs to be done before printing */

  FILE *use_stream = ssa_log_stream;

  if (use_stream == NULL)
    {
      use_stream = stderr;
    }

  /***************************************************************************
   *                           STRUCTURE OF MESSAGE                          *
   ***************************************************************************
   * if fmt is not empty:                                                    *
   *     "TIME SSA: file: line: formatted string: ssa_strerror(ssa_errno)"   *
   * otherwise:                                                              *
   *     "TIME SSA: file: line: ssa_strerror(ssa_errno)"                     *
   ***************************************************************************/
  /* Get current time in proper format */
  time_t current_time;
  struct tm *time_info;
  char time_str[9]; /* HH:MM:SS\0 */
  time (&current_time);
  time_info = localtime (&current_time);
  strftime (time_str, sizeof (time_str), "%H:%M:%S", time_info);

  /* Name of library */
  fprintf (use_stream, "%s SSA: %s: %i", time_str, file, line);
  if ((fmt != NULL) || (*fmt != '\0'))
    {
      /* VA arg related */
      va_list argp;
      va_start (argp, fmt);

      vfprintf (use_stream, fmt, argp);
      va_end (argp);

      fprintf (use_stream, ": ");
    }

  fprintf (use_stream, "%s\n", ssa_strerror (ssa_errno));

  /* flush before exit */
  fflush (use_stream);

  /* libc error() calls exit(), but atexit() may not be done properly */
}
