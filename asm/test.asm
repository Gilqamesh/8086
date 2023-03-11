; ../asm/own disassembly:
bits 16

test_label0:
jnz test_label1
jnz test_label0
test_label1:
jnz test_label0
jnz test_label1
