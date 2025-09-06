/* Zig language support definitions for GDB, the GNU debugger.

   Copyright (C) 1992-2025 Free Software Foundation, Inc.

   This file is part of GDB.

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.  */


#ifndef GDB_ZIG_LANG_H
#define GDB_ZIG_LANG_H

struct ui_file;
struct language_arch_info;
struct type_print_options;
struct parser_state;

#include "compile/compile.h"

extern void zig_language_arch_info (struct gdbarch *gdbarch,
				  struct language_arch_info *lai);

#endif /* GDB_ZIG_LANG_H */
