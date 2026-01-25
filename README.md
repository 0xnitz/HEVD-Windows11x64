* Currently with VBS off - I might do with HVCI in the future
	* KCET is disabled so bof can use a ROP -> see full chain in markdown
	* Also the arbitrary write is super simple, just patching the eprocess->Token
* Becuase `NtQuerySystemInformation` KASLR leak is patched, I'm triggering a crashdump
	* I can implement this into the code, and might do so in the future, also using/racing `NtQuerySystemInformation` is possible (medium integrity)