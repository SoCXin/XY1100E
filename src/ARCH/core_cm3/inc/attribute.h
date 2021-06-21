#ifndef ATTRIBUTE_H
#define ATTRIBUTE_H

#ifdef __cplusplus
 extern "C" {
#endif

/* force function on RAM */
#define RAM_TEXT_FUNCTION(func)      __attribute__((section(".ramtext."#func)))
/* force function on FLASH */
#define FLASH_TEXT_FUNCTION(func)    __attribute__((section(".flashtext."#func)))
/* force read only data on RAM */
#define RAM_RODATA_SECTION(rodata)   __attribute__((section(".ramrodata."#rodata)))


#ifdef __cplusplus
 }
#endif

#endif  /* ATTRIBUTE_H */
