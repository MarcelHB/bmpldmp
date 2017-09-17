# bmpldump - inspect a bitmap color palette

A **b**it**m**ap **p**a**l**ette **dump**er. When you sit in front of a
1/4/8bpp bitmap image file, with the indices of the colors encoding what
you really want to know.

Setup: `$ make`

Do `$ bmpldmp somefile.bmp` to read something like this.

```
   #,   R,   G,   B
   0,   0,   0,   0
   1, 128,   0,   0
   2,   0, 128,   0
   3, 128, 128,   0
   4,   0,   0, 128
   5, 128,   0, 128
   6,   0, 128, 128
   7, 192, 192, 192
   8, 128, 128, 128
   9, 255,   0,   0
  10,   0, 255,   0
  11, 255, 255,   0
  12,   0,   0, 255
  13, 255,   0, 255
  14,   0, 255, 255
  15, 255, 255, 255
```

Guess the index by picking the closest RGB value of the color your
viewer makes you see.

Subject to the _Unlicense_.
