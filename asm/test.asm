; ../asm/own disassembly:
bits 16

rep movsb
rep cmpsb
rep scasb
rep lodsb
rep movsw
rep cmpsw
rep scasw
rep lodsw

; NOTE(casey): Special thanks (as always!) to Mārtiņš Možeiko for figuring out why NASM 
; wouldn't compile "rep stds" instructions. It was because it was a misprint in the 8086
; manual! It was really just "rep stos", which of course is still in x64, and NASM
; assembles it just fine.
rep stosb
rep stosw
