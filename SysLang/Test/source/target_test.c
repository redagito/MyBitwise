#include "testy.h"

#include <syl/target.h>

TESTY_TEST(target) {
  TESTY_CHECK(OS_WIN32 == os_from_string("win32"));
  TESTY_CHECK(OS_LINUX == os_from_string("linux"));
  TESTY_CHECK(OS_INVALID == os_from_string(""));
  TESTY_CHECK(OS_INVALID == os_from_string("winlux"));

  TESTY_CHECK(strcmp("win32", os_to_string(OS_WIN32)) == 0);
  TESTY_CHECK(strcmp("linux", os_to_string(OS_LINUX)) == 0);
  TESTY_CHECK(NULL == os_to_string(OS_INVALID));
  TESTY_CHECK(NULL == os_to_string((os_t)9999));

  TESTY_CHECK(ARCH_X86 == arch_from_string("x86"));
  TESTY_CHECK(ARCH_X64 == arch_from_string("x64"));
  TESTY_CHECK(ARCH_INVALID == arch_from_string(""));
  TESTY_CHECK(ARCH_INVALID == arch_from_string("foo"));

  TESTY_CHECK(strcmp("x86", arch_to_string(ARCH_X86)) == 0);
  TESTY_CHECK(strcmp("x64", arch_to_string(ARCH_X64)) == 0);
  TESTY_CHECK(NULL == arch_to_string(ARCH_INVALID));
  TESTY_CHECK(NULL == arch_to_string((arch_t)9999));

  TESTY_CHECK(target_is_excluded_filename("", OS_WIN32, ARCH_X86) == false);
  TESTY_CHECK(target_is_excluded_filename("foo", OS_WIN32, ARCH_X86) == false);
  TESTY_CHECK(target_is_excluded_filename("x86", OS_WIN32, ARCH_X86) == false);
  TESTY_CHECK(target_is_excluded_filename("x64", OS_WIN32, ARCH_X86) == false);
  TESTY_CHECK(target_is_excluded_filename("win32", OS_WIN32, ARCH_X86) ==
              false);
  TESTY_CHECK(target_is_excluded_filename("linux", OS_WIN32, ARCH_X86) ==
              false);
  TESTY_CHECK(target_is_excluded_filename("foo_bar", OS_WIN32, ARCH_X86) ==
              false);
  TESTY_CHECK(target_is_excluded_filename("foo_bar_xfoo", OS_WIN32, ARCH_X86) ==
              false);
  // These are OK to fail as the file name is required to NOT start with a '_'
  TESTY_CHECK(target_is_excluded_filename("_win32", OS_WIN32, ARCH_X86) ==
              false);
  TESTY_CHECK(target_is_excluded_filename("_linux", OS_WIN32, ARCH_X86) ==
              false);
  TESTY_CHECK(target_is_excluded_filename("_x86", OS_WIN32, ARCH_X86) == false);
  TESTY_CHECK(target_is_excluded_filename("_x64", OS_WIN32, ARCH_X86) == false);

  TESTY_CHECK(target_is_excluded_filename("foo_win32", OS_WIN32, ARCH_X86) ==
              false);
  TESTY_CHECK(target_is_excluded_filename("foo_x86", OS_WIN32, ARCH_X86) ==
              false);
  TESTY_CHECK(target_is_excluded_filename("foo_win32_x86", OS_WIN32,
                                          ARCH_X86) == false);
  TESTY_CHECK(target_is_excluded_filename("foo_x86_win32", OS_WIN32,
                                          ARCH_X86) == false);

  TESTY_CHECK(target_is_excluded_filename("foo_linux", OS_WIN32, ARCH_X86) ==
              true);
  TESTY_CHECK(target_is_excluded_filename("foo_linux_foo", OS_WIN32,
                                          ARCH_X86) == false);
  TESTY_CHECK(target_is_excluded_filename("foo_linux", OS_LINUX, ARCH_X86) ==
              false);
  TESTY_CHECK(target_is_excluded_filename("foo_x64", OS_WIN32, ARCH_X86) ==
              true);
  TESTY_CHECK(target_is_excluded_filename("foo_x64_bar", OS_WIN32, ARCH_X86) ==
              false);
  TESTY_CHECK(target_is_excluded_filename("foo_x64", OS_WIN32, ARCH_X64) ==
              false);

  TESTY_CHECK(
      target_is_excluded_filename("foo_linux_x64", OS_WIN32, ARCH_X86) == true);
  TESTY_CHECK(
      target_is_excluded_filename("foo_x64_linux", OS_WIN32, ARCH_X86) == true);
  TESTY_CHECK(target_is_excluded_filename("foo_linux_x64", OS_LINUX,
                                          ARCH_X64) == false);
  TESTY_CHECK(target_is_excluded_filename("foo_x64_linux", OS_LINUX,
                                          ARCH_X64) == false);

  TESTY_CHECK(
      target_is_excluded_filename("foo_win32_x64", OS_WIN32, ARCH_X86) == true);
  TESTY_CHECK(
      target_is_excluded_filename("foo_x64_win32", OS_WIN32, ARCH_X86) == true);
  TESTY_CHECK(target_is_excluded_filename("foo_win32_x64", OS_WIN32,
                                          ARCH_X64) == false);
  TESTY_CHECK(target_is_excluded_filename("foo_x64_win32", OS_WIN32,
                                          ARCH_X64) == false);

  TESTY_CHECK(
      target_is_excluded_filename("foo_linux_x86", OS_WIN32, ARCH_X86) == true);
  TESTY_CHECK(
      target_is_excluded_filename("foo_x86_linux", OS_WIN32, ARCH_X86) == true);
  TESTY_CHECK(target_is_excluded_filename("foo_linux_x86", OS_LINUX,
                                          ARCH_X86) == false);
  TESTY_CHECK(target_is_excluded_filename("foo_x86_linux", OS_LINUX,
                                          ARCH_X86) == false);
}