section text      ;lol lol aff aff
if label1
copy n1,n2
if 0
rot: add n2
if 1
add n3
copy n2 n3
mul n2
load n3
output n3
jmp rot2
rot2: stop
section data
n1: space
n2: space 4
n3: const 3
label1: equ 1
label2: equ 0
