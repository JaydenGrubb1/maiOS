# maiOS
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
- [x] Boot from GRUB
- [x] Switch to 64-bit long mode
- [x] IO
- [x] Basic stdlib
- [x] Interrupts
- [x] Dynamic paging
- [ ] Memory allocation
- [ ] FPU/SSE
- [ ] Basic terminal
- [ ] Timers
- [ ] Virtual file system
- [ ] Basic file system driver
- [ ] Process scheduler

## Building
See [Building](docs/building.md) for more information.

## License
[BSD-3-Clause License](LICENSE)