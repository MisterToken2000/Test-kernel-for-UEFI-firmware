OUTPUT_FORMAT("elf64-littleaarch64")
SECTIONS
 {
   . = 0x1000;
   .text : { *(.text) }
   .data : { *(.data) }
   .bss  : { *(.bss)  }
 }
