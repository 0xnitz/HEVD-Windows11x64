# HEVD on Windows 11 x64

## Remarks

* Currently with VBS off - I might do with HVCI in the future
	* KCET is disabled so bof can use a ROP -> see full chain in markdown
	* Also the arbitrary write is super simple, just patching the eprocess->Token
* I'm using a ROP chaing to disable SMEP and jump to usermode, when VBS is on, it protects `cr4`, making this impossible
	* I could've used a write primitive to patch the `PTE_BASE` and make SMEP irrelevant, but for that I need no PTE_BASE randomization and a write primitive, which doesn't exist in some IOCTLs

## Current Exploits

BoF Cookie = very hard/impossible. No VBS on my machine but can't overwrite PTE_BASE (no write primitive), can't scan the stack from usermode (no read primitive) to find the cookie and can't bruteforce (64 bit).  Also, can't trigger an exception because the handler is not on the stack.

Bof = duable (have solution up). Can't overwrite PTE_BASE, but I can ROP to disable SMEP and jump to usermode.

WWW = easy. I have a dummy solution up, that just patches EPROCESS->Token. I can make this cleaner by reading the ntos offset of `PsActiveProcessHead` from disk, the ntos address from the API and using the
	WWW as a read primitive to walk the list and patch the addresses. I believe my dummy solution is enough for now (but I might change it to the cleaner version in the future).

Double Fetch (TOCTOU) = easy, my most recent solution, just BoF with extra steps, I'll do the full exploit with this one.

Integer Overflow = actually impossible in 64 bit, DWORD user_buffer_size transferred to a 64 bit register, in 32 bit: just use size=max_int-4 and overflow the return address with my rop chain.
						A bugfix was issued and fixed, but the latest release (HEVD 3.00) does not include the updated merged branch. My exploit should work on the updated version.

UAF = Use the allocate and free, then shape heap with a ton of fake objects, and insert shellcode inside the fake object.
	No need for SMEP bypass because the shellcode is in kernel and executable (non paged), the only hurdle now is to get the shellcode (that will start in a `jmp rax`) to actually increment `rax` to get to the actual shellcode.
			fake_object = [address_of_jmp_rax][shellcode]; (*fake_object)() -> will result in a `jmp rax`= start of the fake object and not start of the fake object + 0x8

UAF NX = Super duper hard, currently in work, need to manipulate named pipes to get arbitrary read/write.

Future Exploits:

Pool IOCTLS = seems super fun, will try later. I need to spam identical pool allocations and corrupt a `POOL_HEADER` of one of them using the bof

Write Null = Looks fun and super hard. I believe leaking some handles and patching `SECURITY_DESCRIPTOR`s is in the only way here. I don't know how to leak that information, so I'll think about it later.