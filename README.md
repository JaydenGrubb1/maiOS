# maiOS
Project is on pause for now until completion of embedded systems course and other less ambitous projects.
## Why
This project exists for the sole purpose of learning about low-level programming and the fundamentals of operating systems. This project was ultimately inspired by the [SerenityOS](https://github.com/SerenityOS/serenity) project but will be much less grand in scale.
## Goals
These are some of the long term goals that I'd like the project to eventually achieve.
- Unix like terminal
- Ability to switch between kernel and user level privelage modes
- Basic file system (probably FAT)
- Driver modules for `qemu` "hardware"
- Scheduler
	- Processes and threads
	- Maybe multi-core utilization
- Basic suite of built in software like `cat`, `grep`, `top`, etc
- Basic networking capabilites
	- Basic `ping` program at minimum
	- Maybe http/s capabilities, i.e. `curl`
- Maybe get `gcc` working within OS

## Tasks
- [x] Basic boot
- [x] Switch to 32-bit protected mode
- [x] Switch to 64-bit long mode
- [x] Working from C/C++ entry point (i.e. initial assembly code complete)
- [x] Basic IO library
- [ ] Slab memory allocator
- [ ] Kernel debug e.g. `kprintf()`
- [ ] Dynamic paging (not just 1st 1GB)
- [ ] RTC and timers
- [ ] ...

## Immediate Tasks
- [ ] Create Docker build environment
	- remove dependency on host machine libraries
- [ ] Fix global constructors issue

## License
[BSD-3-Clause License](LICENSE)