# Dependencies

## Arch

```bash
sudo pacman -S base-devel cmake gdb grub libisoburn mtools nasm qemu
```

OS-dev wiki says we also need `gmp`, `libmpc` and `mpfr`, but these are already included in `base-devel` package.

## Ubuntu

```bash
sudo apt install bison build-essential cmake flex gdb grub-common grub-pc-bin libgmp3-dev libmpc-dev libmpfr-dev nasm qemu qemu-kvm texinfo xorriso
```