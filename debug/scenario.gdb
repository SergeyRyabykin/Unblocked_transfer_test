target extended-remote localhost:3333
file uart_dma_test.elf
tui new-layout temp_layout {-horizontal src 1 asm 1} 2 regs 1 cmd 1
layout temp_layout
load uart_dma_test.elf