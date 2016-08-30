
A few **demos and documents and tools** that could help you write a **native C/C++ application** for your **Pocketbook Touch Lux ereader**.


# Demos

In each `demo` directory, you will find a small application highlighting or presenting one specific point.

You should take a look at the first three demos in the following order:

 * `demo01`: how to create your first application
 * `demo02-gdb`: how to remote-debug an application, using `gdb` in CLI on the computer
 * `demo03-makefile-eclipse`: using a `Makefile` to build the application, and developping with an IDE

Once you have completed these first steps, the order of the next demos is far less important, as they
each focus on a specific point and should be relatively independant from each other.


# Documents

A `README.md` file in present in each demo directory, presenting the goals of the demo and sometimes
giving some additionnal informations.

For example, see `demo02-gdb/README.md` to learn how to remote-debug an application, or `demo03-makefile-eclipse/README.md`
to see how to configure Eclipse for both development and remote-debugging.


# Tools

To make the development process a bit easier (there is no shell on the device -- and even if we can
install one, the virtual keyboard makes it rather hard to use), a few tools are provided in this repo.

## App Sender / Receiver

A small shell-script that listens on the ereader side while the computer sends an application over wifi.

⇒ No need to plug the device with an USB cable each time you need to test a new buid!


## Dev Utils

A collection of commands you can run from a shell-script. They expose features that are normally only
accessible from native-apps.

This is used by several other scripts in this repository.


# Disclaimer

```
#include <std/disclaimer.h>
```

Like *they* say:

> The content in these demos can be *deadly* to your ereader device!  
> In short, "If it breaks, you get to keep both pieces."

These demos and documents were written by people just like you: not-quite-perfect.
Which means there might be mistakes, errors, things we did not quite understand…  
Our hope is they can help some other developpers (maybe you?) write applications for their Pocketbook e-reader devices.
In any case, especially if you want to fix something: PRs are welcome!

Also please note that, even if some commiters on this project work at [TEA, The Ebook Alternative](https://www.tea-ebook.com/),
which sells Pocketbook ereader devices, the demos and docs in this repository have been written using a **public SDK**
(exploring it, doing a lot of trial-and-error), with **absolutely no *insider-knownledge* at all**.
