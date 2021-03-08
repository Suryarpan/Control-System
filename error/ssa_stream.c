/*
 * ssa_stream.c (Created on 2021-02-24)
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
#include <stdarg.h>
#include <stdio.h>
#include <time.h>

/* default stream and handler */
FILE *ssa_stream SSA_NO_EXPORT                         = NULL;
ssa_stream_handler_t *ssa_stream_handler SSA_NO_EXPORT = NULL;

ssa_stream_handler_t *
ssa_set_stream_handler (ssa_stream_handler_t *new_handler)
{
  ssa_stream_handler_t *prev_handler = ssa_stream_handler;
  ssa_stream_handler                 = new_handler;
  return prev_handler;
}

FILE *
ssa_set_stream (FILE *new_stream)
{
  FILE *prev_stream = ssa_stream;
  if (prev_stream == NULL)
    {
      prev_stream = stderr;
    }
  ssa_stream = new_stream;
  return prev_stream;
}

SSA_NO_EXPORT void
ssa_stream_printf (const char *label, const char *file, int line,
                   const char *reason)
{
  FILE *use_stream = ssa_stream;
  if (use_stream == NULL)
    {
      use_stream = stderr;
    }
  if (ssa_stream_handler)
    {
      (*ssa_stream_handler) (label, file, line, reason);
      return;
    }
  fprintf (use_stream, "SSA: %s:%d: %s: %s", file, line, label, reason);
}
