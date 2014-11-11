#ifndef RAMDISK_H
#define RAMDISK_H

/* ramdisk provides a fake filesystem.
 * It is read-only, and its contents
 * are assembled at compile time.
 */

typedef void (*Process)(void);

typedef struct
{
  const char *  filename;
  Process       process;
} File;

Process ramdisk_find(const char* filename);


#endif

