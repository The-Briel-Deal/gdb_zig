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

#include "c-lang.h"
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
#include "valprint.h"
#include "value.h"

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
    return language_defn::read_var_value (var, var_block, frame);
  }

  void value_print (struct value *val, struct ui_file *stream,
		    const struct value_print_options *options) const override
  {
    return language_defn::value_print (val, stream, options);
  }

  void printptr (struct value *val, struct ui_file *stream, int recurse,
		 const struct value_print_options *options) const
  {
    const type *real_type = check_typedef (val->type ());
    gdb_assert (real_type->code () == TYPE_CODE_PTR);
    type *real_target_type = check_typedef (real_type->target_type ());
    switch (real_target_type->code ())
      {
      case TYPE_CODE_STRING:
      case TYPE_CODE_ARRAY:
	if (real_target_type->is_string_like ())
	  {
	    int len = -1;
	    gdb::unique_xmalloc_ptr<gdb_byte> buffer;

	    struct type *char_type;
	    const char *charset;

	    struct value *derefed_str = value_ind (val);

	    c_get_string (derefed_str, &buffer, &len, &char_type, &charset);
	    printstr (stream, real_target_type, buffer.get (), len, NULL,
		      false, options);
	    return;
	  }
	break;
      default:
	break;
      }
    error (_ ("Value with unsupported target type \"%s\" passed to "
	      "`zig_language:printptr()`"),
	   real_target_type->name ());
  }

  void printstr (struct ui_file *stream, struct type *type,
		 const gdb_byte *string, unsigned int length,
		 const char *user_encoding, int force_ellipses,
		 const struct value_print_options *options) const override
  {
    const char *default_encoding = "UTF-8";
    if (user_encoding != NULL)
      {
	error (_ ("User provided string encodings are not currently supported "
		  "in zig, everything is assumed to be UTF-8. User provided "
		  "\"%s\" as the string encoding."),
	       user_encoding);
      }
    generic_printstr (stream, check_typedef (type->target_type ()), string,
		      length, default_encoding, force_ellipses, '"', 1,
		      options);
  }
  void
  value_print_inner (struct value *val, struct ui_file *stream, int recurse,
		     const struct value_print_options *options) const override
  {
    type *real_type = check_typedef (val->type ());

    if (real_type->code () == TYPE_CODE_PTR)
      {
	printptr (val, stream, recurse, options);
      }
    else
      {
	c_value_print_inner (val, stream, recurse, options);
      }
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
