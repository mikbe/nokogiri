#include <xml_syntax_error.h>

static void dealloc(xmlErrorPtr ptr)
{
  free(ptr);
}

/*
 * call-seq:
 *  column
 *
 * Column number or 0 if not available
 */
static VALUE column(VALUE self)
{
  xmlErrorPtr error;
  Data_Get_Struct(self, xmlError, error);
  return INT2NUM(error->int2);
}

/*
 * call-seq:
 *  int1
 *
 * Extra number information
 */
static VALUE int1(VALUE self)
{
  xmlErrorPtr error;
  Data_Get_Struct(self, xmlError, error);
  return INT2NUM(error->int1);
}

/*
 * call-seq:
 *  str3
 *
 * Extra string information
 */
static VALUE str3(VALUE self)
{
  xmlErrorPtr error;
  Data_Get_Struct(self, xmlError, error);
  if(error->str3)
    return rb_str_new2(error->str3);
  return Qnil;
}

/*
 * call-seq:
 *  str2
 *
 * Extra string information
 */
static VALUE str2(VALUE self)
{
  xmlErrorPtr error;
  Data_Get_Struct(self, xmlError, error);
  if(error->str2)
    return rb_str_new2(error->str2);
  return Qnil;
}

/*
 * call-seq:
 *  str1
 *
 * Extra string information
 */
static VALUE str1(VALUE self)
{
  xmlErrorPtr error;
  Data_Get_Struct(self, xmlError, error);
  if(error->str1)
    return rb_str_new2(error->str1);
  return Qnil;
}

/*
 * call-seq:
 *  line
 *
 * Get the line number of the error
 */
static VALUE line(VALUE self)
{
  xmlErrorPtr error;
  Data_Get_Struct(self, xmlError, error);
  return INT2NUM(error->line);
}

/*
 * call-seq:
 *  file
 *
 * Get the filename for the error
 */
static VALUE file(VALUE self)
{
  xmlErrorPtr error;
  Data_Get_Struct(self, xmlError, error);
  if(error->file)
    return rb_str_new2(error->file);

  return Qnil;
}

/*
 * call-seq:
 *  level
 *
 * Get the error level
 */
static VALUE level(VALUE self)
{
  xmlErrorPtr error;
  Data_Get_Struct(self, xmlError, error);
  return INT2NUM((short)error->level);
}

/*
 * call-seq:
 *  code
 *
 * Get the error code
 */
static VALUE code(VALUE self)
{
  xmlErrorPtr error;
  Data_Get_Struct(self, xmlError, error);
  return INT2NUM(error->code);
}

/*
 * call-seq:
 *  domain
 *
 * Get the part of the library that raised this exception
 */
static VALUE domain(VALUE self)
{
  xmlErrorPtr error;
  Data_Get_Struct(self, xmlError, error);
  return INT2NUM(error->domain);
}

/*
 * call-seq:
 *  message
 *
 * Get the human readable message.
 */
static VALUE message(VALUE self)
{
  xmlErrorPtr error;
  Data_Get_Struct(self, xmlError, error);
  return rb_str_new2(error->message);
}

void Nokogiri_error_handler(void * ctx, xmlErrorPtr error)
{
  // FIXME: I'm interneting this.  I *think* the pointer passed in here gets
  // freed on its own, thats why I copy it.
  // The files are *in* the computer.
  xmlErrorPtr ptr = calloc(1, sizeof(xmlError));
  xmlCopyError(error, ptr);

  VALUE err = Data_Wrap_Struct(cNokogiriXmlSyntaxError, NULL, dealloc, ptr);
  VALUE block = rb_funcall(mNokogiri, rb_intern("error_handler"), 0);
  rb_funcall(block, rb_intern("call"), 1, err);
}

VALUE cNokogiriXmlSyntaxError;
void init_xml_syntax_error()
{
  VALUE nokogiri = rb_define_module("Nokogiri");
  VALUE xml = rb_define_module_under(nokogiri, "XML");
  VALUE klass = rb_define_class_under(xml, "SyntaxError", rb_eSyntaxError);
  cNokogiriXmlSyntaxError = klass;

  rb_define_method(klass, "message", message, 0);
  rb_define_method(klass, "domain", domain, 0);
  rb_define_method(klass, "code", code, 0);
  rb_define_method(klass, "level", level, 0);
  rb_define_method(klass, "file", file, 0);
  rb_define_method(klass, "line", line, 0);
  rb_define_method(klass, "str1", str1, 0);
  rb_define_method(klass, "str2", str2, 0);
  rb_define_method(klass, "str3", str3, 0);
  rb_define_method(klass, "int1", int1, 0);
  rb_define_method(klass, "column", column, 0);
}
