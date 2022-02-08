# Xilinx ISE WebPack crash hack-o-fix

This repository is a result of my struggling with Xilinx ISE WebPack 14.7 crashing with `SIGSEGV` on my Kubuntu 21.10 (64 bit) laptop.

A bit of `gdb` entertainment pointed me to this very font file igniting the crash:

```
$ ls -l /usr/share/fonts/truetype/noto/NotoColorEmoji.ttf
-rw-r--r-- 1 root root 10082968 lip 17  2021 /usr/share/fonts/truetype/noto/NotoColorEmoji.ttf
```

It's enough to remove this file, or `chmod 000` it, but where's the fun?

Let's try a simple `LD_PRELOAD` trick to hide this file from Xilinx ISE only (hey, I wanna use my emoji font, right?). Intercepting `open()` call solved the issue, but the process was `SIGSEGV`-ing a bit later. Hijacking `FT_New_Face()` (from `libfreetype`) worked, though.

## How to use?

You need `libfreetype-dev` package installed on your system, and compiler, and you know it all.

```
$ make
gcc -Wall -fPIC -shared -I /usr/include/freetype2/ -o fixtype.so fixtype.c -ldl
$ LD_PRELOAD=`pwd`/fixtype.so  /opt/Xilinx/14.7/ISE_DS/ISE/bin/lin64/ise
```

Enjoy!

