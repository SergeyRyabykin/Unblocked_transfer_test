target extended-remote localhost:3333
file elvees_test.elf
tui new-layout temp_layout {-horizontal src 1 asm 1} 2 regs 1 cmd 1
layout temp_layout
load elvees_test.elf