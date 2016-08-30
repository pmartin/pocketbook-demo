
For this first demo, let's start from this little (17 pages) and old (5+ years) piece of documentation,
written by Pocketbook, found next to the SDK in another `pocketbook-free` repository:

‣ [PocketBookSDK.pdf](https://github.com/pocketbook-free/InkViewDoc)

*Note: this first demo is pretty much copy-pasted from this doc.*

***

For everything that follow, in this demo and in the other ones, as well as pretty much every pocketbook-related
development and documentation written here, we'll depend on the following environment variable being properly set:

```
export PBSDK="$HOME/developpement/PBSDK"
```

Getting the SDK, distributed by Pocketbook on one of their `pocketbook-free` repositories:

```
git clone https://github.com/pocketbook-free/SDK_481.git "$PBSDK"
cd "$PBSDK"
git checkout 5.12
```

*Note: this is the SDK for firmware 5.x devices, like the Touch Lux 3 ereader.*

Then, from the directory that contains our project's source-code, to compile the application:

```
"$PBSDK/bin/arm-obreey-linux-gnueabi-g++" demo01.cpp -o demo01.app -linkview
```

⇒ This should build `demo01.app` from `demo01.cpp`, linking to `libinkview` (the shared library
containing a lot of pocketbook-relected functions)

Possible verification, ensure we have an ARM binary executable:

```
$ file demo01.app
demo01.app: ELF 32-bit LSB executable, ARM, EABI5 version 1 (SYSV), dynamically linked, interpreter /lib/ld-linux.so.3, for GNU/Linux 2.6.35, not stripped
```

Finally, to run the application, we need to transfert it to the ereader (using an USB cable) and put it
in the `applications/` folder. It will then appear in the applications drawer, allowing us to run it.

![Success: we have something displayed on the screen!](http://extern.pascal-martin.fr/pocketbook-demo/demo01-ibAbNojyij9.jpg)
