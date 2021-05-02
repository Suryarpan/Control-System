/*
 * ssa_strerror.c (Created on 2021-02-23)
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

const char *
ssa_strerror (ssa_error_t err_val)
{
  switch (err_val)
    {
      case SSA_EDOM:
        return "input domain error";
      case SSA_ERANGE:
        return "output range error";
      case SSA_EFAULT:
        return "invalid pointer";
      case SSA_EINVAL:
        return "invalid argument(s) supplied by user";
      case SSA_ESANITY:
        return "sanity checked failed (very bad)";
      case SSA_ENOMEM:
        return "malloc or friends failed";
      case SSA_ERUNAWAY:
        return "iterative process out of control";
      case SSA_EMAXITER:
        return "exceeded max number of iterations";
      case SSA_EZERODIV:
        return "tried to divide by zero";
      case SSA_EBADTOL:
        return "user specified an invalid tolerance";
      case SSA_ETOL:
        return "failed to reach the specified tolerance";
      case SSA_EBADLEN:
        return "matrix, vector lengths are not conformant";
      case SSA_ENOTSQR:
        return "matrix not square";
      case SSA_EOF:
        return "end of file";
      default:
        return "unknown error code";
    }
}
