
Our goals for this demo:

 * Being able to parse some JSON string and extract the data it contains


# Resources

The JSON library used in this demo is `json-c` (one of the JSON libs included in FW 5.x).

Couple of useful links:

 * [json-c on github](https://github.com/json-c/json-c/)
 * [json-c / libjson Tutorial with Examples](https://linuxprograms.wordpress.com/2010/05/20/json-c-libjson-tutorial/)


# Eclipse configuration

If you are working with Eclipse, to get auto-completion for `json` functions, you'll have to add the
following directory to includes folders, like we did for `curl` in `demo05`:

```
${PBSDK}/arm-obreey-linux-gnueabi/sysroot/usr/include
```
