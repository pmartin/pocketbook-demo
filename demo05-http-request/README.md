
Our goals for this demo:

 * Doing an HTTP request
 * Being able to manipulate the result
 * Using APIs provided by `libinkview` for HTTP requests
 * Using `curl` for more control over the requests

# Functions related to HTTP requests

There are a few functions that seem interesting in [`inkview.h`](https://github.com/pocketbook-free/SDK_481/blob/5.12/arm-obreey-linux-gnueabi/sysroot/usr/local/include/inkview.h):

 * [`QuickDownloadExt()`](https://github.com/pocketbook-free/SDK_481/blob/5.12/arm-obreey-linux-gnueabi/sysroot/usr/local/include/inkview.h#L2318)
 * [`DownloadTo()`](https://github.com/pocketbook-free/SDK_481/blob/5.12/arm-obreey-linux-gnueabi/sysroot/usr/local/include/inkview.h#L2325)


# Interesting notes

When using ereader functions from `libinkview`, if wifi is down when we try to send a request, the ereader will
automatically display the system dialog, allowing the user to get online \o/

Questions / to check:

 * Do we get the same system dialog when using curl, or do we have to connect *by hand*?
 * What happens if user cancels?


# Eclipse configuration

If you are working with Eclipse, to get auto-completion for `curl` functions, you'll have to add the
following directory to includes folders:

```
${PBSDK}/arm-obreey-linux-gnueabi/sysroot/usr/include
```
