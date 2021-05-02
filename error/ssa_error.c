/*
 * ssa_error.c (Created on 2021-02-23)
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
#include "ssa_export.h"
#include <stdio.h>
#include <stdlib.h>

#ifndef __GNUC__
#  define _unused /* Nothing */
#else
#  define _unused __attribute__ ((unused))
#endif

/* $ Address of error handler $
 * Note: Volatile pointer to avoid problems during optimization
 */
ssa_error_handler_t *volatile ssa_error_handler SSA_NO_EXPORT = NULL;

SSA_NO_EXPORT void
no_error_handler (const char *reason _unused, const char *file _unused,
                  int line _unused, ssa_error_t ssa_errno _unused)
{
/* Do nothing */
#ifndef __GNUC__
  (void)reason;
  (void)file;
  (void)line;
  (void)ssa_errno;
#endif
  return;
}

ssa_error_handler_t *
ssa_set_error_handler (ssa_error_handler_t *new_handler)
{
  ssa_error_handler_t *prev_handler = ssa_error_handler;
  ssa_error_handler                 = new_handler;
  return prev_handler;
}

ssa_error_handler_t *
ssa_set_error_handler_off (void)
{
  ssa_error_handler_t *prev_handler = ssa_error_handler;
  ssa_error_handler                 = no_error_handler;
  return prev_handler;
}

void
ssa_handle_error (const char *reason, const char *file, int line,
                  ssa_error_t ssa_errno)
{
  if (ssa_error_handler)
    {
      ssa_error_handler (reason, file, line, ssa_errno);
      return;
    }
  ssa_stream_printf ("ERROR", file, line, reason);
  fflush (stdout);
  fprintf (stderr, "SSA: Default error handler invoked: Aborting...\n");
  fflush (stderr);

  abort ();
}
