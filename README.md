# cocos
Color Computer OS

# Synopsis

I'm setting off to answer the question:
What if the RSDOS operating system on the Color Computer 3 had been written targeting an 8086 instead of a 6809?

# Upcoming Tasks
* Update the keyboard lock LEDs as needed.
* Implement malloc.
* Get the terminal scrolling while respecting cursor color.

# Done (recently)
* Get a simple REPL running.
* Implement the libc keyboard reading functions.

# Log

## 2021-08-30

Found some great resources for figuring out the IDT:
* [Developer](http://www.osdever.net/tutorials/)
* [OS Dev Wiki](https://wiki.osdev.org/)

I had wasted a bit of time trying to figure out how to use interrupts to control the cursor and keyboard.  I originally learned assembly in 16-bit real mode back in the '90s, so this made sense to me.  It's not how things are done anymore though.  I made a lot of progress when I starting using IO ports.  The IDT is necessary for the timer and keyboard interrupt handlers.  I copied a bunch of code from the Tetris OS project to make this happen, but it didn't work right away.  I was able to make sense of the rest of what I needed to do using [Bran's Kernel Development Tutorial](http://www.osdever.net/tutorials/view/brans-kernel-development-tutorial).  Turns out those useless-looking segment register loads and long jumps over short distance were actually pivotal to making the whole thing work!

I have a colorful prompt now, and have implemented the getc/getchar/gets/getline libc functions.  I'm really missing the malloc function right now, so that one's probably coming up soon.  You can type at the prompt, and the kernel will process your text and do something with it.  It's a start.
