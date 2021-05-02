/*
 * ssa_types.h (Created on 2021-03-22)
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

#ifndef SSA_TYPES_H
#define SSA_TYPES_H

#ifdef SSA_BUILD_GUARD
#  include "ssa_config.h"
#  include "ssa_export.h"
#elif !defined(SSA_INTERNAL_HEADER)
#  error Never include "ssa_types.h" directly. Use "ssa/ssa.h"
#endif /* !SSA_BUILD_GUARD */

#include <stddef.h>

typedef struct
{
  size_t size;
  double *coeff_arr;
} ssa_poly;

typedef struct
{
  size_t size;
  float *coeff_arr;
} ssa_polyf;

typedef struct
{
  size_t size;
  long double *coeff_arr;
} ssa_polyld;

ssa_poly *
ssa_poly_alloc (size_t size);

ssa_polyf *
ssa_polyf_alloc (size_t size);

ssa_polyld *
ssa_polyld_alloc (size_t size);

void
ssa_poly_free (ssa_poly *polynm);

void
ssa_polyf_free (ssa_polyf *polynm);

void
ssa_polyld_free (ssa_polyld *polynm);

#endif /* !SSA_TYPES_H */
