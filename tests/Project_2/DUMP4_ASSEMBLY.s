# Evaluationof pipelined processor 

#independant instructions
addi x1, x0, 0xff
addi x2, x0, 31
addi x3, x0, 0xab
addi x4, x0, -35

sub x11, x1, x2
xor x12, x2, x1
addi x0, x0, 0
srli x13, x3, 3
slti x14, x4, 4

addi x5, x0, 0x400         # x5 = address
addi x1, x0, 0x412
addi x2, x0, 0x414
lui x3, 0x416
auipc x4, 0x418

sh x11, 4(x5)
lhu x15, 4(x5)
sltiu x16, x2, 8
sw x3, 8(x5)
lw x17, 8(x5)
addi x0, x0, 0
addi x0, x0, 0
addi x0, x0, 0
add x18, x16, x17

# Answer in X11,12,13,14,15,16,17,18

#Forwarded instructions RAW 
addi x1, x0, 0x4
addi x2, x0, 31
addi x3, x0, 0xab
addi x4, x0, -35

sub x19, x1, x2
xor x19, x2, x19
srli x2, x19, 3
slti x20, x2, 4

lui x21, 0xfed
addi x21, x21, 0xfe
sll x21, x21, x1
xori x21, x21, 2
add x20, x21, x20

#Answer in x19, X20

#load instrctions set 1
addi x1, x0, 0xa0
addi x2, x0, 0xb0
addi x3, x0, 0xc0
#addi x4, x0, 0xd0
addi x4, x0, 0x400

lui x24, 0xff
srai x24, x24, 4
addi x25, x0, 0x20
add x24, x24, x25

sb x4, 40(x4)
lb x21, 40(x4)
sub x21, x21, x2
sw x21, 48(x4)
lhu x8, 48(x4)
sw x8, 8(x8)
lw x9, 8(x8)

#Answer in X8 X9

# dependent branch instructions // with data hazards
addi x1, x0, 1
addi x2, x0, 0xb1
addi x3, x0, 0xc1
addi x4, x0, 0x4
addi x5, x0, 0x1

loop1:
add x2, x3, x4
slli x5, x5, 2 
slti x3, x2, -8
sub x4, x4, x1
blt x1, x4, loop1

srli x28, x5, 3
add x29, x2, x28

#Answer in X28,X29

# dependent branch instructions // with load stalls
addi x1, x0, 0x1ff
addi x2, x0, 0xb1
addi x3, x0, 0xc1
addi x4, x0, 0x4
addi x22, x0, 0x480

sh x4, 4(x22)
loop2:
slli x1, x1, 1
lh x5, 4(x22)
bge x5, x1, loop2

#lhu x30, 0(x22)
slli x30, x1, 10
add x31, x2, x30

#Answer in X30,X31