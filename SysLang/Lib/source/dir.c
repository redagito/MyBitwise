#include "syl/dir.h"

#include <string.h>

#include "syl/path.h"

#ifdef _WIN32
#include <errno.h>
#include <io.h>
#elif defined LINUX
#include <dirent.h>
#else
#error "Not implemented for this operating system"
#endif

/**
 * Returns true if the entry should be excluded/skipped when iterating
 */
static bool dir_excluded(dir_list_iter_t *iter) {
  return iter->valid && strcmp(iter->name, ".") == 0 ||
         strcmp(iter->name, "..") == 0;
}

void dir_list_free(dir_list_iter_t *iter) {
  if (iter->valid) {
    iter->valid = false;
    iter->error = false;

#ifdef _WIN32
    _findclose((intptr_t)iter->handle);
#elif defined LINUX
    closedir(iter->handle);
#else
#error "Not implemented for this operating system"
#endif

    iter->handle = NULL;
  }
}

#ifdef _WIN32
static void dir_update_win32(dir_list_iter_t *iter, bool done,
                             struct _finddata_t *fileinfo) {
  iter->valid = !done;
  iter->error = done && errno != ENOENT;
  if (!done) {
    iter->size = fileinfo->size;
    memcpy(iter->name, fileinfo->name, sizeof(iter->name) - 1);
    iter->name[MAX_PATH - 1] = 0;
    iter->is_dir = fileinfo->attrib & _A_SUBDIR;
  }
}
#endif

void dir_list_next(dir_list_iter_t *iter) {
  if (!iter->valid) {
    return;
  }

  do {
#ifdef _WIN32
    struct _finddata_t fileinfo;
    int result = _findnext((intptr_t)iter->handle, &fileinfo);
    dir_update_win32(iter, result != 0, &fileinfo);
    if (result != 0) {
      dir_list_free(iter);
      return;
    }
#elif defined LINUX
    struct dirent *entry = readdir(iter->handle);
    if (entry == NULL) {
      dir_list_free(iter);
      return;
    }
    path_copy(iter->name, entry->d_name);
    iter->is_dir = entry->d_type & DT_DIR;
#else
#error "Not implemented for this operating system"
#endif
  } while (dir_excluded(iter));
}

void dir_list(dir_list_iter_t *iter, const char *path) {
  memset(iter, 0, sizeof(*iter));
#ifdef _WIN32
  path_copy(iter->base, path);
  char filespec[MAX_PATH];
  path_copy(filespec, path);
  path_join(filespec, "*");

  struct _finddata_t fileinfo;
  intptr_t handle = _findfirst(filespec, &fileinfo);
  iter->handle = (void *)handle;
  dir_update_win32(iter, handle == -1, &fileinfo);
  if (dir_excluded(iter)) {
    dir_list_next(iter);
  }
#elif defined LINUX
  DIR *dir = opendir(path);
  if (dir == NULL) {
    iter->valid = false;
    iter->error = true;
    return;
  }
  iter->handle = dir;
  path_copy(iter->base, path);
  iter->valid = true;
  dir_list_next(iter);
#else
#error "Not implemented for this operating system"
#endif
}