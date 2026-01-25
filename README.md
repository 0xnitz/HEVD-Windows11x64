* Currently with VBS off - I might do with HVCI in the future
* Becuase `NtQuerySystemInformation` KASLR leak is patched, I'm triggering a crashdump
	* I can implement this into the code, and might do so in the future, also racing `NtQuerySystemInformation` is possible