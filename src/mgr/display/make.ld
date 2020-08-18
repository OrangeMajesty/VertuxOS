
ENTRY(starting)
SECTIONS
{
  .header_d :
  {
    *header.o(.data)
    *header.o(.rodata)
    *header.o(.bss)

  }
  .data :
  {   data = .;
     *(.data)
     *(.rodata)
     *(.bss)
     . = ALIGN(4096);
  }
  .text :
  {
    code = .;
    *(.text)
  }
  .comment :
  {
    end = .;
  }
} 