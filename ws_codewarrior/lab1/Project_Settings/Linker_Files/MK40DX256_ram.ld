/*
*****************************************************************************
**
**  File        : K40DX256M10_ram.ld
**
** 	Default linker command file for RAM targets
**
*****************************************************************************
*/
/* Entry Point */
ENTRY(__thumb_startup)

/* Highest address of the user mode stack */
_estack = 0x20008000;    /* end of 64K RAM */
__SP_INIT = _estack;

/* Generate a link error if heap and stack don't fit into RAM */
__heap_size = 0x800;		/* required amount of heap  */
__stack_size = 0x800;		/* required amount of stack */

/* Specify the memory areas */
MEMORY
{
  m_interrupts	(rx) : ORIGIN = 0x1FFF8000, LENGTH = 0x1E0
  m_text 		(rx) : ORIGIN = 0x1FFF81E0, LENGTH = 32K-0x1E0		/* Lower SRAM */
  m_data 		(rw) : ORIGIN = 0x20000000, LENGTH = 32K			/* Upper SRAM */
}


/* Define output sections */
SECTIONS
{
  /* The startup code goes first into RAM */
  .interrupts :
  {
	__vector_table = .;
    . = ALIGN(4);
    KEEP(*(.vectortable)) /* Startup code */
    . = ALIGN(4);
  } > m_interrupts

  /* The program code and other data goes into RAM */
  .text :
  {
    . = ALIGN(4);
    *(.text)           /* .text sections (code) */
    *(.text*)          /* .text* sections (code) */
    *(.rodata)         /* .rodata sections (constants, strings, etc.) */
    *(.rodata*)        /* .rodata* sections (constants, strings, etc.) */
    *(.glue_7)         /* glue arm to thumb code */
    *(.glue_7t)        /* glue thumb to arm code */
    *(.eh_frame)

    KEEP (*(.init))
    KEEP (*(.fini))

    . = ALIGN(4);
    _etext = .;        /* define a global symbols at end of code */
  } > m_text

  .ARM.extab   : { *(.ARM.extab* .gnu.linkonce.armextab.*) } > m_text
  .ARM : {
	__exidx_start = .;
	  *(.ARM.exidx*)
	  __exidx_end = .;
  } > m_text

  .ctors :
  {
    __CTOR_LIST__ = .;
    /* gcc uses crtbegin.o to find the start of
       the constructors, so we make sure it is
       first.  Because this is a wildcard, it
       doesn't matter if the user does not
       actually link against crtbegin.o; the
       linker won't look for a file to match a
       wildcard.  The wildcard also means that it
       doesn't matter which directory crtbegin.o
       is in.  */
    KEEP (*crtbegin.o(.ctors))
    /* We don't want to include the .ctor section from
       from the crtend.o file until after the sorted ctors.
       The .ctor section from the crtend file contains the
       end of ctors marker and it must be last */
    KEEP (*(EXCLUDE_FILE (*crtend.o ) .ctors))
    KEEP (*(SORT(.ctors.*)))
    KEEP (*(.ctors))
    __CTOR_END__ = .;
  } > m_text  
  .dtors :
  {
    __DTOR_LIST__ = .;
    KEEP (*crtbegin.o(.dtors))
    KEEP (*(EXCLUDE_FILE (*crtend.o ) .dtors))
    KEEP (*(SORT(.dtors.*)))
    KEEP (*(.dtors))
    __DTOR_END__ = .;
  } > m_text

  .preinit_array :
  {
    PROVIDE_HIDDEN (__preinit_array_start = .);
    KEEP (*(.preinit_array*))
    PROVIDE_HIDDEN (__preinit_array_end = .);
  } > m_text
  .init_array :
  {
    PROVIDE_HIDDEN (__init_array_start = .);
    KEEP (*(SORT(.init_array.*)))
    KEEP (*(.init_array*))
    PROVIDE_HIDDEN (__init_array_end = .);
  } > m_text
  .fini_array :
  {
    PROVIDE_HIDDEN (__fini_array_start = .);
    KEEP (*(SORT(.fini_array.*)))
    KEEP (*(.fini_array*))
    PROVIDE_HIDDEN (__fini_array_end = .);
  } > m_text

  /* Initialized data sections goes into RAM, load LMA copy after code */
  .data :
  {
    . = ALIGN(4);
    _sdata = .;        /* create a global symbol at data start */
    *(.data)           /* .data sections */
    *(.data*)          /* .data* sections */

    . = ALIGN(4);
    _edata = .;        /* define a global symbol at data end */
  } > m_data

  /* Uninitialized data section */
  . = ALIGN(4);
  .bss :
  {
    /* This is used by the startup in order to initialize the .bss section */
    __START_BSS = .;
	PROVIDE ( __bss_start__ = __START_BSS );
    *(.bss)
	*(.bss*)
    *(COMMON)
	
	. = ALIGN(4);
    __END_BSS = .;
	PROVIDE ( __bss_end__ = __END_BSS );
  } > m_data

  /* User_heap_stack section, used to check that there is enough RAM left */
  ._user_heap_stack :
  {
    . = ALIGN(4);
    PROVIDE ( end = . );
    PROVIDE ( _end = . );
	__heap_addr = .;
    . = . + __heap_size;
    . = . + __stack_size;
    . = ALIGN(4);
  } > m_data
  
  __S_romp = 0;
  .ARM.attributes 0 : { *(.ARM.attributes) }
}
