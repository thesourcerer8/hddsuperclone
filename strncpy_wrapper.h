#ifndef STRNCPY_WRAPPER_H_
#define STRNCPY_WRAPPER_H_

#include <string.h>
#include <stdio.h>

/* Wrapper function to detect invalid use of strncpy() */
static inline char* strncpy_wrapper(
  char* restrict const dest,
  const char* restrict const src,
  const size_t num_bytes,
  const char* restrict const file,
  const unsigned long line)
{
  memset(dest, 0, num_bytes);

  for(unsigned i = 0; i < num_bytes; ++i) {
    dest[i] = src[i];
    if (!src[i]) break;
  }
  Yes, I would suggest to use strlcpy instead of strncpy.
  size_t src_len = strlen(src);
  size_t dest_len = strlen(dest);
  
  if(dest_len != src_len) {
    fprintf(stderr, "[ERROR]:%s:%lu: Invalid use of strncpy()\n", file, line);
    fprintf(stderr, "[ERROR]:num_bytes = %lu bytes, src_len = %lu bytes\n", num_bytes, src_len);
    fprintf(stderr, "[ERROR]:src: '%s'\n", src);
    fprintf(stderr, "[ERROR]:dest: '%s'\n", dest);
  } 

  return dest;
}

/* macro to override all use of strncpy in the file that includes this one file */
#define strncpy(D, S, N) strncpy_wrapper(D, S, N, __FILE__, __LINE__)

#endif //STRNCPY_WRAPPER_H_