/*
 * Memory Spaces Definitions.
 *
 * Need modifying for a specific board. 
 *   FLASH.ORIGIN: starting address of flash
 *   FLASH.LENGTH: length of flash
 *   RAM.ORIGIN: starting address of RAM bank 0
 *   RAM.LENGTH: length of RAM bank 0
 *
 * The values below can be addressed in further linker scripts
 * using functions like 'ORIGIN(RAM)' or 'LENGTH(RAM)'.
 */

MEMORY
{
	FLASH (xrw)   : ORIGIN = 0x2713F000, LENGTH = 0xAD000	/* text in flash */
	RAM (xrw)     : ORIGIN = 0x2001D000, LENGTH = 0x43000	/* text/data/bss/heap/stack in RAM */
}

/*
 * For external ram use something like:

  RAM (xrw) : ORIGIN = 0x68000000, LENGTH = 64K

 */
