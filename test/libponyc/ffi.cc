#include <gtest/gtest.h>
#include "util.h"


// FFI type checking tests

#define TEST_ERROR(src) DO(test_error(src, "expr"))
#define TEST_COMPILE(src) DO(test_compile(src, "expr"))


class FFITest : public PassTest
{};

TEST_F(FFITest, NoDeclaration)
{
  const char* src =
    "class Foo\n"
    "  fun f() =>\n"
    "    @foo()";

  TEST_ERROR(src);
}



TEST_F(FFITest, Declaration)
{
  const char* src =
    "use @foo[U32]()\n"

    "class Foo\n"
    "  fun f() =>\n"
    "    @foo()";

  TEST_COMPILE(src);
}


TEST_F(FFITest, DeclarationArgs)
{
  const char* src =
    "use @foo[U32](a: Bool, b: U32)\n"

    "class Foo\n"
    "  fun f(x: U32) =>\n"
    "    @foo(true, x)";

  TEST_COMPILE(src);
}


TEST_F(FFITest, DeclarationBadArg)
{
  const char* src =
    "use @foo[U32](a: Bool, b: U32)\n"

    "class Foo\n"
    "  fun f(x: U32) =>\n"
    "    @foo(true, false)";

  TEST_ERROR(src);
}


TEST_F(FFITest, DeclarationTooFewArgs)
{
  const char* src =
    "use @foo[U32](a: Bool, b: U32)\n"

    "class Foo\n"
    "  fun f(x: U32) =>\n"
    "    @foo(true)";

  TEST_ERROR(src);
}


TEST_F(FFITest, DeclarationTooManyArgs)
{
  const char* src =
    "use @foo[U32](a: Bool, b: U32)\n"

    "class Foo\n"
    "  fun f(x: U32) =>\n"
    "    @foo(true, x, false)";

  TEST_ERROR(src);
}



TEST_F(FFITest, DeclarationVoidStarPointer)
{
  const char* src =
    "use @foo[U32](a: Pointer[None])\n"

    "class Foo\n"
    "  fun f(x: Pointer[U32]) =>\n"
    "    @foo(x)";

  TEST_COMPILE(src);
}


TEST_F(FFITest, DeclarationVoidStarUSize)
{
  const char* src =
    "use @foo[U32](a: Pointer[None])\n"

    "class Foo\n"
    "  fun f(x: USize) =>\n"
    "    @foo(x)";

  TEST_COMPILE(src);
}


TEST_F(FFITest, DeclarationVoidStarNotPointer)
{
  const char* src =
    "use @foo[U32](a: Pointer[None])\n"

    "class Foo\n"
    "  fun f(x: U8) =>\n"
    "    @foo(x)";

  TEST_ERROR(src);
}


TEST_F(FFITest, DeclarationEllipsisNoArgs)
{
  const char* src =
    "use @foo[U32](...)\n"

    "class Foo\n"
    "  fun f(x: U32) =>\n"
    "    @foo()";

  TEST_COMPILE(src);
}


TEST_F(FFITest, DeclarationEllipsisOneArg)
{
  const char* src =
    "use @foo[U32](...)\n"

    "class Foo\n"
    "  fun f(x: U32) =>\n"
    "    @foo(true)";

  TEST_COMPILE(src);
}


TEST_F(FFITest, DeclarationEllipsisMultipleArgs)
{
  const char* src =
    "use @foo[U32](...)\n"

    "class Foo\n"
    "  fun f(x: U32) =>\n"
    "    @foo(true, x)";

  TEST_COMPILE(src);
}


TEST_F(FFITest, DeclarationEllipsisNotFirst)
{
  const char* src =
    "use @foo[U32](a: Bool, ...)\n"

    "class Foo\n"
    "  fun f(x: U32) =>\n"
    "    @foo(true, x)";

  TEST_COMPILE(src);
}


TEST_F(FFITest, DeclarationEllipsisNotLast)
{
  const char* src =
    "use @foo[U32](..., a: Bool)";

  TEST_ERROR(src);
}


TEST_F(FFITest, DeclarationAlias)
{
  const char* src =
    "use @foo[None](x: Foo iso)\n"

    "class Foo\n"
    "  fun f() =>\n"
    "    let x: Foo iso = Foo\n"
    "    @foo(x)";

  TEST_ERROR(src);
}
