
This is a small PC + ereader tool that allows one to send an application from a computer to an
ereader over wifi, not having to connect the ereader to the computer using USB

# Two components

 * An application to run on the ereader to receive other applications
 * A shell-script to run on the computer to send an application to the ereader

All this: without having to use an USB cable \o/


# Usage

On the ereader, run `app-receiver.app` from the applications drawer.

On the computer, run the transfert script:

```
./app-sender.sh "$HOME/pocketbook/applications/plop.app" "plop.app" "192.168.0.12"
```

Parameters are as follow:

 * Path to the application on the computer
 * Name of the application file that will be created in the `applications/` folder on the ereader
 * IP address of the ereader
