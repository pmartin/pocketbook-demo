
Our goals for this demo:

 * Using a (simple) `Makefile` to compile our (simple) application
 * Developping using Eclipse (re-using this `Makefile` to compile the app), with auto-complete + remote-debugging with a GUI


# A first Makefile

The principles of makefiles is not explained here; if needed, you'll easily find a tutorial on the matter on the Internet.

Here, we have:

 * A target `all`, building `demo03.app`
 * `demo03.app` depends on `demo03.cpp`
 * A typical `clean` target.

Using a `Makefile`, here, has one goal: we want to be able to compile from CLI or from pretty much
any editor or IDE we might want to use, depending on each one's preferences.


# An IDE: Eclipse

There are many C++ IDEs we could use; here, I chose Eclipse (as I've already used it for other C/C++ projects these last couple of years).

## Installing Eclipse

To developp with Eclipse and have auto-complete and all:

 * Download the installer from [its website](https://eclipse.org/downloads/)
 * Install "Eclipse IDE for C/C++ Developers"


## A project with Eclipse

With an existing source directory called `demo03-makefile-eclipse` and using a `Makefile` to build the app,
create the corresponding Eclipse project:

 * `File` > `New` > `Makefile Project with existing code`
 * Fill-in the following fields in the dialog box:

    * `Languages` : `C++`
    * `Toolchain` : `Cross GCC`

Once the project has been created, Eclipse should be able to delete and re-create the binary, using
`make`, with our `Makefile` that was already working in CLI:

 * `Project` > `Clean`
 * `Project` > `Build All`

## Auto-complete and functions form `inkview.h`

Then, next step is to configure Eclipse so it finds the `.h` header files we are working with, so:

 * We don't have 3/4 of our code underlined in red because of missing functions / constants / types
 * We get auto-complete and Ctrl+click!

We have to do the following configuration steps:

 * Right click on the project > `Properties`
 * `C/C++ General` > `Paths and Symbols` > `Includes` > `GNU C++` > `Add...`
 * Add: `/home/squale/developpement/PBSDK/arm-obreey-linux-gnueabi/sysroot/usr/local/include` or `${PBSDK}/arm-obreey-linux-gnueabi/sysroot/usr/local/include` if you have set the *right* environment variable ;-)
 * When you have confirmed everything in the dialog box, all *unknown function* and similar errors should disappear \o/ and you'll get auto-complete \o/

Note: you will probably have to repeat the same steps for other include directories in the SDK, when you include some other file.


## Remote debug (with graphical interface with Eclipse + running the code on an ereader)

### Debug configuration

We have to create a debug configuration:

 * `Run` > `Debug Configurations...`
 * Right click on `C/C++ Remote Application` > `New`
 * At the bottom of the dialog box, set: `Using GDB (DSF) Manual Remote Debugging Launcher`
 * `Main` tab:

    * `Project`: `demo03-makefile-eclipse`
    * `C/C++ Application`: `demo03.app`

 * `Debugger` tab:

    * `Main`:

       * `GDB debugger`: `/home/pmartin/developpement/PBSDK/bin/arm-obreey-linux-gnueabi-gdb` ← adapt this path
       * `GDB commande file`: `.gdbinit`

    * `Shared Libraries`:

       * Ajouter: `/home/pmartin/developpement/PBSDK/arm-obreey-linux-gnueabi/sysroot/usr/local/lib` ← adapt this path

    * `Connection`:

       * `Type`: `TCP`
       * `Host name or IP address`: `192.168.0.12` ← IP address of your ereader
       * `Port number`: `10002` ← the same you used to run `gdbserver` on the ereader


### Starting a debug session

On the ereader, run `gdbserver` on your application, with the same port as specified in Eclipse -- like we did before (see `demo02-gdb`).

In Eclipse: `Debug`


### Profit \o/

![Screenshot of Eclipse while remote-debugging an ereader application](http://extern.pascal-martin.fr/pocketbook-demo/demo03-debug-eclipse-tafOveyfsiv2.png)


### Troubleshooting

Hint: if you get the following error, make sure the ereader is still connected to your wiki!

```
Error in final launch sequence
Failed to execute MI command:
-target-select remote 192.168.0.12:10002
Error message from debugger back end:
192.168.0.12:10002: Aucun chemin d'accès pour atteindre l'hôte cible.
192.168.0.12:10002: Aucun chemin d'accès pour atteindre l'hôte cible.
```

*To preserve battery, the ereader disconnects quite quickly ;-)*
