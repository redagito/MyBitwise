#include "testy.h"

#include <string.h>

#include <syl/path.h>

TESTY_TEST(path) {
  // path_normalize
  char path1[] = "C:\\foo\\bar\\barfoo";
  path_normalize(path1);
  TESTY_ASSERT(strcmp(path1, "C:/foo/bar/barfoo") == 0);

  char path2[] = "C:\\foo\\bar\\";
  path_normalize(path2);
  TESTY_ASSERT(strcmp(path2, "C:/foo/bar") == 0);

  char path3[] = "C:/foo\\bar\\";
  path_normalize(path3);
  TESTY_ASSERT(strcmp(path3, "C:/foo/bar") == 0);

  char path4[] = "C:\\foo\\bar\\foo.exe";
  path_normalize(path4);
  TESTY_ASSERT(strcmp(path4, "C:/foo/bar/foo.exe") == 0);

  // path_copy
  char path[MAX_PATH];
  path_copy(path, "C:\\a\\b\\");
  TESTY_ASSERT(strcmp(path, "C:/a/b") == 0);

  path_copy(path, "/a/b/c.d");
  TESTY_ASSERT(strcmp(path, "/a/b/c.d") == 0);

  path_copy(
      path,
      "/aaaaaaaaa/aaaaaaaaa/aaaaaaaaa/aaaaaaaaa/aaaaaaaaa/aaaaaaaaa/aaaaaaaaa/"
      "aaaaaaaaa/aaaaaaaaa/aaaaaaaaa/aaaaaaaaa/aaaaaaaaa/aaaaaaaaa/aaaaaaaaa/"
      "aaaaaaaaa/aaaaaaaaa/aaaaaaaaa/aaaaaaaaa/aaaaaaaaa/aaaaaaaaa/bbbbbbbbb/"
      "bbbbbbbbb/bbbbbbbbb/bbbbbbbbb/bbbbbbbbb/bbbbbbbbb/bbbbbbbbb/");
  TESTY_ASSERT(
      strcmp(path,
             "/aaaaaaaaa/aaaaaaaaa/aaaaaaaaa/aaaaaaaaa/aaaaaaaaa/aaaaaaaaa/"
             "aaaaaaaaa/aaaaaaaaa/aaaaaaaaa/aaaaaaaaa/aaaaaaaaa/aaaaaaaaa/"
             "aaaaaaaaa/aaaaaaaaa/aaaaaaaaa/aaaaaaaaa/aaaaaaaaa/aaaaaaaaa/"
             "aaaaaaaaa/aaaaaaaaa/bbbbbbbbb/bbbbbbbbb/bbbbbbbbb/bbbbbbbbb/"
             "bbbbbbbbb/bbbbbbbb") == 0);

  // path_join
  path_copy(path, "/a/b/c/");
  TESTY_ASSERT(strcmp(path, "/a/b/c") == 0);
  path_join(path, "d");
  TESTY_ASSERT(strcmp(path, "/a/b/c/d") == 0);
  path_join(path, "/e");
  TESTY_ASSERT(strcmp(path, "/a/b/c/d/e") == 0);
  path_join(path, "/f/");
  TESTY_ASSERT(strcmp(path, "/a/b/c/d/e/f") == 0);
  path_join(path, "g/");
  TESTY_ASSERT(strcmp(path, "/a/b/c/d/e/f/g") == 0);
  path_join(path, "h.i");
  TESTY_ASSERT(strcmp(path, "/a/b/c/d/e/f/g/h.i") == 0);
  path_join(path, "/j.k");
  TESTY_ASSERT(strcmp(path, "/a/b/c/d/e/f/g/h.i/j.k") == 0);
  path_join(path, "\\l\\m/n\\o/p\\");
  TESTY_ASSERT(strcmp(path, "/a/b/c/d/e/f/g/h.i/j.k/l/m/n/o/p") == 0);

  // path_file
  TESTY_ASSERT(strcmp(path_file(""), "") == 0);
  TESTY_ASSERT(strcmp(path_file("/"), "") == 0);
  TESTY_ASSERT(strcmp(path_file("/a/b/c"), "c") == 0);
  TESTY_ASSERT(strcmp(path_file("/a/b/c/"), "") == 0);
  TESTY_ASSERT(strcmp(path_file("aaaa"), "aaaa") == 0);
  TESTY_ASSERT(strcmp(path_file("C:/a/b/c.d"), "c.d") == 0);

  // path_ext
  TESTY_ASSERT(strcmp(path_ext(""), "") == 0);
  TESTY_ASSERT(strcmp(path_ext("."), "") == 0);
  TESTY_ASSERT(strcmp(path_ext(".exe"), "exe") == 0);
  TESTY_ASSERT(strcmp(path_ext("/foo/bar"), "/foo/bar") == 0);

  // path_absolute
  path_copy(path, "C:/foo/bar/../");
  path_absolute(path);
  TESTY_ASSERT(strcmp(path, "C:/foo") == 0);
  path_copy(path, "C:/foo/bar/../..");
  path_absolute(path);
  TESTY_ASSERT(strcmp(path, "C:") == 0);
}