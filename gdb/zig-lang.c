/* C language support routines for GDB, the GNU debugger.

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

#include "extract-store-integer.h"
#include "symtab.h"
#include "gdbtypes.h"
#include "expression.h"
#include "parser-defs.h"
#include "language.h"
#include "zig-lang.h"
#include "cp-abi.h"
#include "cp-support.h"
#include "gdbcore.h"
#include "gdbarch.h"
#include "utils.h"

void
zig_language_arch_info (struct gdbarch *gdbarch,
			struct language_arch_info *lai)
{
  const struct builtin_type *builtin = builtin_type (gdbarch);

  /* Helper function to allow shorter lines below.  */
  auto add = [&] (struct type *t) { lai->add_primitive_type (t); };

  add (builtin->builtin_int);
  add (builtin->builtin_long);
  add (builtin->builtin_short);
  add (builtin->builtin_char);
  add (builtin->builtin_float);
  add (builtin->builtin_double);
  add (builtin->builtin_void);
  add (builtin->builtin_long_long);
  add (builtin->builtin_signed_char);
  add (builtin->builtin_unsigned_char);
  add (builtin->builtin_unsigned_short);
  add (builtin->builtin_unsigned_int);
  add (builtin->builtin_unsigned_long);
  add (builtin->builtin_unsigned_long_long);
  add (builtin->builtin_long_double);
  add (builtin->builtin_complex);
  add (builtin->builtin_double_complex);
  add (builtin->builtin_decfloat);
  add (builtin->builtin_decdouble);
  add (builtin->builtin_declong);

  lai->set_string_char_type (builtin->builtin_char);
  lai->set_bool_type (builtin->builtin_int);
}

/* Class representing the Zig language.  */

class zig_language : public language_defn
{
public:
  zig_language ()
    : language_defn (language_zig)
  { /* Nothing.  */
  }

  /* See language.h.  */

  const char *name () const override
  {
    return "zig";
  }

  /* See language.h.  */

  const char *natural_name () const override
  {
    return "Zig";
  }

  /* See language.h.  */

  const std::vector<const char *> &filename_extensions () const override
  {
    static const std::vector<const char *> extensions = { ".zig" };
    return extensions;
  }

  /* See language.h.  */
  void language_arch_info (struct gdbarch *gdbarch,
			   struct language_arch_info *lai) const override
  {
    zig_language_arch_info (gdbarch, lai);
  }

  /* See language.h.  */

  bool can_print_type_offsets () const override
  {
    return true;
  }

  /* See language.h.  */

  void print_type (struct type *type, const char *varstring,
		   struct ui_file *stream, int show, int level,
		   const struct type_print_options *flags) const override
  {
    c_print_type (type, varstring, stream, show, level, la_language, flags);
  }

  struct value *read_var_value (struct symbol *var,
				const struct block *var_block,
				const frame_info_ptr &frame) const override
  {
    value *val = language_defn::read_var_value (var, var_block, frame);
    // TODO: Remove this once I get this working
    gdb_printf ("GF_DBG:\n"
		"  val->type()->name() = '%s'\n"
		"  val->type()->code() = '%d'\n"
		"  check_typedef(val->type())->code() = '%d'\n"
		"  val->enclosing_type()->name() = '%s'\n"
		"  val->enclosing_type()->code() = '%d'\n"
		"  val->contents().data() = '%.5s'\n",
		val->type ()->name (), val->type ()->code (),
		check_typedef (val->type ())->code (),
		val->enclosing_type ()->name (),
		val->enclosing_type ()->code (), val->contents ().data ());
    return val;
  }

  /* See language.h.  */

  bool store_sym_names_in_linkage_form_p () const override
  {
    return true;
  }

  /* See language.h.  */

  enum macro_expansion macro_expansion () const override
  {
    return macro_expansion_no;
  }
};

/* Single instance of the C language class.  */

static zig_language zig_language_defn;
