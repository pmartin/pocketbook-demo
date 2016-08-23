
# Etapes préparatoires

On a besoin des outils de développement / déboggage sur la liseuse. Ils sont fournis avec le SDK, il faut
les copier vers la liseuse :

```
cp -R $PBSDK/arm-obreey-linux-gnueabi/debug-root/* /media/squale/TL\ TEA/system/
```

Il faudra également pouvoir lancer `gdbserver` sur la liseuse. Par exemple :

 * en ayant un serveur telnet ou ssh sur la liseuse
 * ou via un terminal comme `pbterm` (ce que j'ai fais ci-dessous)
 * ou via un rapide script qui lance la bonne commande, cf `gdbserver-demo02.app`

Attention : dans le dernier cas (le plus pratique à lancer), lors de son lancement, l'application reste colorée dans
le tiroir d'applications, donnant l'impression qu'il ne se passe rien au départ. Une fois le deboggage terminé, utiliser
la commande `kill` de `dbg` fermera le tout.


# Compilation

```
"$PBSDK/bin/arm-obreey-linux-gnueabi-g++" demo02.cpp -o demo02.app -linkview -g -gdwarf-3
```

Options importantes :

 * `-g` pour avoir les informations de déboggage
 * `-gdwarf-3` pour que la table de symboles soit dans un ancien format compris par la version de GDB utilisée

Le binaire produit est *plus gros* que celui sans informations de débug ; il doit être déposé dans `applications/` de la liseuse.


# Débogage

Dans le principe :

 * On lance `gdbserver` sur la liseuse
 * On s'y connecte depuis `gdb` sur le PC
 * Le code est exécuté sur la liseuse, avec contrôle de l'exécution et du débogage depuis le PC.


## Vérification sur le PC

Déjà, vérifions qu'on charge bien les libs :

```
$PBSDK/bin/arm-obreey-linux-gnueabi-ldd --root $PBSDK/arm-obreey-linux-gnueabi/sysroot/usr/local/ demo02.app
        libinkview.so => /lib/libinkview.so (0xdeadbeef)
        libdl.so.2 => /lib/libdl.so.2 (0x8badf00d)
        libc.so.6 => /lib/libc.so.6 (0x8badf00d)
        ld-linux.so.3 => /lib/ld-linux.so.3 (0x8badf00d)
        libpthread.so.0 => /lib/libpthread.so.0 (0x8badf00d)
        libfreetype.so.6 => /usr/lib/libfreetype.so.6 (0x8badf00d)
        libbz2.so.1.0 => /usr/lib/libbz2.so.1.0 (0x8badf00d)
        libpng12.so.0 => /usr/lib/libpng12.so.0 (0x8badf00d)
        libz.so.1 => /usr/lib/libz.so.1 (0x8badf00d)
        libm.so.6 => /lib/libm.so.6 (0x8badf00d)
        libjpeg.so.8 => /usr/lib/libjpeg.so.8 (0x8badf00d)
        libtiff.so.5 => /usr/lib/libtiff.so.5 (0x8badf00d)
        librt.so.1 => /lib/librt.so.1 (0x8badf00d)
        libcrypto.so.1.0.0 => /usr/lib/libcrypto.so.1.0.0 (0x8badf00d)
        libicuuc.so.51 => /usr/lib/libicuuc.so.51 (0x8badf00d)
        libicudata.so.51 => /usr/lib/libicudata.so.51 (0x8badf00d)
        libstdc++.so.6 => /lib/libstdc++.so.6 (0x8badf00d)
        libgcc_s.so.1 => /lib/libgcc_s.so.1 (0x8badf00d)
```

=> ça devrait trouver les libs \o/


## Lancement gdbserveur sur la liseuse

Lancer `gdbserver` sur la liseuse, une fois qu'elle est connectée au même réseau wifi que le PC :

```
/mnt/ext1/system/usr/bin/gdbserver 0.0.0.0:10002 /mnt/ext1/applications/demo02.app
```

On devrait obtenir un affichage de ce type :

```
Process /mnt/ext1/applications/demo02.app created; pid = 731
Listening on port 10002
```


## Session de debug depuis le PC

Lancer gdb sur l'application (la même que celle sur la liseuse) :

```
$PBSDK/bin/arm-obreey-linux-gnueabi-gdb ./demo02.app

GNU gdb (crosstool-NG 1.19.0) 7.4.1
Copyright (C) 2012 Free Software Foundation, Inc.
License GPLv3+: GNU GPL version 3 or later <http://gnu.org/licenses/gpl.html>
This is free software: you are free to change and redistribute it.
There is NO WARRANTY, to the extent permitted by law.  Type "show copying"
and "show warranty" for details.
This GDB was configured as "--host=i686-build_pc-linux-gnu --target=arm-obreey-linux-gnueabi".
For bug reporting instructions, please see:
<http://www.gnu.org/software/gdb/bugs/>...
Reading symbols from /home/pmartin/developpement/pocketbook-demo/demo02-gdb/demo02.app...done.
```

Se connecter au serveur qui tourne sur la liseuse :

```
(gdb) target remote 192.168.100.132:10002
Remote debugging using 192.168.100.132:10002
Reading symbols from /home/pmartin/developpement/PBSDK/arm-obreey-linux-gnueabi/sysroot/lib/ld-linux.so.3...done.
Loaded symbols for /home/pmartin/developpement/PBSDK/arm-obreey-linux-gnueabi/sysroot/lib/ld-linux.so.3
0x40000bb0 in _start () from /home/pmartin/developpement/PBSDK/arm-obreey-linux-gnueabi/sysroot/lib/ld-linux.so.3
```

Pour l'instant, on n'a pas trop de bibliothèques partagées accédées (vu que l'appli est tout juste lancée) :

```
(gdb) info sharedlibrary
From        To          Syms Read   Shared Object Library
0x400007a0  0x400160d4  Yes         /home/pmartin/developpement/PBSDK/arm-obreey-linux-gnueabi/sysroot/lib/ld-linux.so.3
```

Positionnons un breakpoint au début de la fonction `test_debug()` qu'on veut débugguer :

```
(gdb) break test_debug
Breakpoint 1 at 0x88d0
```

Et avançons (on commence à avoir des sorties qui se plaignent) :

```
(gdb) continue
Continuing.
warning: .dynamic section for "/home/pmartin/developpement/PBSDK/arm-obreey-linux-gnueabi/sysroot/usr/lib/libssl.so.1.0.0" is not at the expected address (wrong library or version mismatch?)
warning: .dynamic section for "/home/pmartin/developpement/PBSDK/arm-obreey-linux-gnueabi/sysroot/usr/lib/libcrypto.so.1.0.0" is not at the expected address (wrong library or version mismatch?)
warning: .dynamic section for "/home/pmartin/developpement/PBSDK/arm-obreey-linux-gnueabi/sysroot/usr/lib/libjpeg.so.8" is not at the expected address (wrong library or version mismatch?)
warning: .dynamic section for "/home/pmartin/developpement/PBSDK/arm-obreey-linux-gnueabi/sysroot/usr/lib/libcurl.so.4" is not at the expected address (wrong library or version mismatch?)
warning: Could not load shared library symbols for 2 libraries, e.g. /usr/lib/libinkview.so.
Use the "info sharedlibrary" command to see the complete listing.
Do you need "set solib-search-path" or "set sysroot"?

Program received signal SIGILL, Illegal instruction.
0x404282e8 in ?? () from /home/pmartin/developpement/PBSDK/arm-obreey-linux-gnueabi/sysroot/usr/lib/libcrypto.so.1.0.0
```

En effet, maintenant, il nous manque quelques infos de bibliothèques partagées :

```
(gdb) info sharedlibrary
From        To          Syms Read   Shared Object Library
0x400007a0  0x400160d4  Yes         /home/pmartin/developpement/PBSDK/arm-obreey-linux-gnueabi/sysroot/lib/ld-linux.so.3
                        No          /usr/lib/libinkview.so
                        No          /usr/lib/libstdc++.so.6
0x401dc210  0x402087c0  Yes         /home/pmartin/developpement/PBSDK/arm-obreey-linux-gnueabi/sysroot/lib/libm.so.6
0x40254d98  0x402643b4  Yes         /home/pmartin/developpement/PBSDK/arm-obreey-linux-gnueabi/sysroot/lib/libgcc_s.so.1
0x40281f30  0x4034e784  Yes         /home/pmartin/developpement/PBSDK/arm-obreey-linux-gnueabi/sysroot/lib/libc.so.6
0x4037bd90  0x4038897c  Yes         /home/pmartin/developpement/PBSDK/arm-obreey-linux-gnueabi/sysroot/lib/libpthread.so.0
0x403a62bc  0x403d779c  Yes         /home/pmartin/developpement/PBSDK/arm-obreey-linux-gnueabi/sysroot/usr/lib/libssl.so.1.0.0
0x40428e80  0x404e83c0  Yes         /home/pmartin/developpement/PBSDK/arm-obreey-linux-gnueabi/sysroot/usr/lib/libcrypto.so.1.0.0
0x40537800  0x40585134  Yes         /home/pmartin/developpement/PBSDK/arm-obreey-linux-gnueabi/sysroot/usr/lib/libfreetype.so.6
0x405d281c  0x405f6df4  Yes         /home/pmartin/developpement/PBSDK/arm-obreey-linux-gnueabi/sysroot/usr/lib/libjpeg.so.8
0x40607080  0x4063aafc  Yes         /home/pmartin/developpement/PBSDK/arm-obreey-linux-gnueabi/sysroot/usr/lib/libtiff.so.5
0x40669410  0x4067dd20  Yes         /home/pmartin/developpement/PBSDK/arm-obreey-linux-gnueabi/sysroot/usr/lib/libpng12.so.0
0x4068a7a0  0x40695bb0  Yes         /home/pmartin/developpement/PBSDK/arm-obreey-linux-gnueabi/sysroot/usr/lib/libz.so.1
0x406acd68  0x406e495c  Yes         /home/pmartin/developpement/PBSDK/arm-obreey-linux-gnueabi/sysroot/usr/lib/libcurl.so.4
0x4072dc58  0x407be4f0  Yes (*)     /home/pmartin/developpement/PBSDK/arm-obreey-linux-gnueabi/sysroot/usr/lib/libicuuc.so.51
0x4088c3c8  0x4095b138  Yes (*)     /home/pmartin/developpement/PBSDK/arm-obreey-linux-gnueabi/sysroot/usr/lib/libicui18n.so.51
0x40981320  0x409862f4  Yes (*)     /home/pmartin/developpement/PBSDK/arm-obreey-linux-gnueabi/sysroot/usr/lib/libicuio.so.51
0x4098f968  0x4099068c  Yes (*)     /home/pmartin/developpement/PBSDK/arm-obreey-linux-gnueabi/sysroot/lib/libdl.so.2
0x4099b700  0x4099ec48  Yes (*)     /home/pmartin/developpement/PBSDK/arm-obreey-linux-gnueabi/sysroot/lib/librt.so.1
0x409aa038  0x409b38c8  Yes         /home/pmartin/developpement/PBSDK/arm-obreey-linux-gnueabi/sysroot/usr/lib/libbz2.so.1.0
0x409bffe4  0x409c9450  Yes         /home/pmartin/developpement/PBSDK/arm-obreey-linux-gnueabi/sysroot/usr/lib/libcares.so.2
                        Yes (*)     /home/pmartin/developpement/PBSDK/arm-obreey-linux-gnueabi/sysroot/usr/lib/libicudata.so.51
(*): Shared library is missing debugging information.
```

Pour les charger :

```
(gdb) set solib-search-path /home/pmartin/developpement/PBSDK/arm-obreey-linux-gnueabi/sysroot/usr/local/lib
warning: .dynamic section for "/home/pmartin/developpement/PBSDK/arm-obreey-linux-gnueabi/sysroot/usr/local/lib/libinkview.so" is not at the expected address (wrong library or version mismatch?)
Reading symbols from /home/pmartin/developpement/PBSDK/arm-obreey-linux-gnueabi/sysroot/usr/local/lib/libinkview.so...(no debugging symbols found)...done.
Loaded symbols for /home/pmartin/developpement/PBSDK/arm-obreey-linux-gnueabi/sysroot/usr/local/lib/libinkview.so
Cannot access memory at address 0x0

(gdb) info sharedlibrary
From        To          Syms Read   Shared Object Library
0x400007a0  0x400160d4  Yes         /home/pmartin/developpement/PBSDK/arm-obreey-linux-gnueabi/sysroot/lib/ld-linux.so.3
0x40047e24  0x400bfd48  Yes (*)     /home/pmartin/developpement/PBSDK/arm-obreey-linux-gnueabi/sysroot/usr/local/lib/libinkview.so
                        No          /usr/lib/libstdc++.so.6
0x401dc210  0x402087c0  Yes         /home/pmartin/developpement/PBSDK/arm-obreey-linux-gnueabi/sysroot/lib/libm.so.6
0x40254d98  0x402643b4  Yes         /home/pmartin/developpement/PBSDK/arm-obreey-linux-gnueabi/sysroot/lib/libgcc_s.so.1
0x40281f30  0x4034e784  Yes         /home/pmartin/developpement/PBSDK/arm-obreey-linux-gnueabi/sysroot/lib/libc.so.6
0x4037bd90  0x4038897c  Yes         /home/pmartin/developpement/PBSDK/arm-obreey-linux-gnueabi/sysroot/lib/libpthread.so.0
0x403a62bc  0x403d779c  Yes         /home/pmartin/developpement/PBSDK/arm-obreey-linux-gnueabi/sysroot/usr/lib/libssl.so.1.0.0
0x40428e80  0x404e83c0  Yes         /home/pmartin/developpement/PBSDK/arm-obreey-linux-gnueabi/sysroot/usr/lib/libcrypto.so.1.0.0
0x40537800  0x40585134  Yes         /home/pmartin/developpement/PBSDK/arm-obreey-linux-gnueabi/sysroot/usr/lib/libfreetype.so.6
0x405d281c  0x405f6df4  Yes         /home/pmartin/developpement/PBSDK/arm-obreey-linux-gnueabi/sysroot/usr/lib/libjpeg.so.8
0x40607080  0x4063aafc  Yes         /home/pmartin/developpement/PBSDK/arm-obreey-linux-gnueabi/sysroot/usr/lib/libtiff.so.5
0x40669410  0x4067dd20  Yes         /home/pmartin/developpement/PBSDK/arm-obreey-linux-gnueabi/sysroot/usr/lib/libpng12.so.0
0x4068a7a0  0x40695bb0  Yes         /home/pmartin/developpement/PBSDK/arm-obreey-linux-gnueabi/sysroot/usr/lib/libz.so.1
0x406acd68  0x406e495c  Yes         /home/pmartin/developpement/PBSDK/arm-obreey-linux-gnueabi/sysroot/usr/lib/libcurl.so.4
0x4072dc58  0x407be4f0  Yes (*)     /home/pmartin/developpement/PBSDK/arm-obreey-linux-gnueabi/sysroot/usr/lib/libicuuc.so.51
0x4088c3c8  0x4095b138  Yes (*)     /home/pmartin/developpement/PBSDK/arm-obreey-linux-gnueabi/sysroot/usr/lib/libicui18n.so.51
0x40981320  0x409862f4  Yes (*)     /home/pmartin/developpement/PBSDK/arm-obreey-linux-gnueabi/sysroot/usr/lib/libicuio.so.51
0x4098f968  0x4099068c  Yes (*)     /home/pmartin/developpement/PBSDK/arm-obreey-linux-gnueabi/sysroot/lib/libdl.so.2
0x4099b700  0x4099ec48  Yes (*)     /home/pmartin/developpement/PBSDK/arm-obreey-linux-gnueabi/sysroot/lib/librt.so.1
0x409aa038  0x409b38c8  Yes         /home/pmartin/developpement/PBSDK/arm-obreey-linux-gnueabi/sysroot/usr/lib/libbz2.so.1.0
0x409bffe4  0x409c9450  Yes         /home/pmartin/developpement/PBSDK/arm-obreey-linux-gnueabi/sysroot/usr/lib/libcares.so.2
                        Yes (*)     /home/pmartin/developpement/PBSDK/arm-obreey-linux-gnueabi/sysroot/usr/lib/libicudata.so.51
(*): Shared library is missing debugging information.
```

Avançons un peu plus dans notre applications, pour arriver jusqu'à notre breakpoint :

```
(gdb) continue
Continuing.
Cannot access memory at address 0x0

Breakpoint 1, 0x000088d0 in test_debug(ifont_s*, int, int) ()
```

On en est où de la backtrace ?

```
(gdb) backtrace
#0  0x000088d0 in test_debug(ifont_s*, int, int) ()
#1  0x00008b0c in main_handler(int, int, int) ()
#2  0x400554a4 in ?? () from /home/pmartin/developpement/PBSDK/arm-obreey-linux-gnueabi/sysroot/usr/local/lib/libinkview.so
Cannot access memory at address 0x15
#3  0x400554a4 in ?? () from /home/pmartin/developpement/PBSDK/arm-obreey-linux-gnueabi/sysroot/usr/local/lib/libinkview.so
Cannot access memory at address 0x15
Backtrace stopped: previous frame identical to this frame (corrupt stack?)
```

Et les arguments et variables ?

```
(gdb) info args
font = 0x155128
a = 1000
b = 3

(gdb) info locals
buffer = '\000' <repeats 44 times>"\220, \370\377\276\000\000\000\000\060\371\377\276\001\000\000\000\030\000\000\000\204\212\000@\001\000\000\000\220\370\377\276\240[\002@4\346\001@\276\067@\002\340\023\002\000\250'\f@`\034\005@\001\000\000\000,\000\000\000\000\000\000\000X\246\001@\005\365\002@@\306\002@`^\002@\001\000\000\000@\274\002@\000\000\000\000X\246\001@\321\367\006H\f\250\001@\020\250\001@0\371\377\276\005\365\002@|\371\377\276\244\214\000@0\371\377\276\f(\002@\000\000\000\000\000\000\000\000\001\000\000\000\000@7@\000\000\000\000\000\000\000\000\276\337 B\000\000\000\000hP7@\220\001\025\000\002\000\000\200\002\000\000\000\020\060\005@\304\373\230@\344\371\377\276\324\360,@\001\000\000\000H\371\377\276\220\001\025\000g\371\377\276\002\000\000\200\070\006\001@\000\000\000\000 \372\377\276cannot open shared ob"...
result = 0
```

Mettons un autre breakpoint quelques lignes plus loin et sautons-y :

```
(gdb) break demo02.cpp:17
Breakpoint 3 at 0x895c: file demo02.cpp, line 17.

(gdb) continue
Continuing.

Breakpoint 3, test_debug (font=0x155128, a=1010, b=3) at demo02.cpp:17
17          sprintf(buffer, "a=%d b=%d result=%d", a, b, result);
```

Et les variables ont changé :

```
(gdb) print a
$3 = 1010

(gdb) print b
$4 = 3
```

On peut même avancer ligne par ligne (et voir que l'écran se MAJ au fur et à mesure) :

```
(gdb) s
18          DrawTextRect(0, 20, ScreenWidth(), kFontSize, buffer, ALIGN_LEFT);

(gdb) s
19          FullUpdate();

(gdb) s
21          b *= 2;
```

Donc : on peut débogguer \o/


# Liens utiles

C'est du `dbg`, donc il peut être intéressant de connaitre les commandes de l'outil.

Quelques liens en vrac :

 * [GDB Command Reference](http://visualgdb.com/gdbreference/commands/)
 * [Continuing and Stepping](https://sourceware.org/gdb/onlinedocs/gdb/Continuing-and-Stepping.html)
