
Comme documentation, j'ai trouvé ceci, dont je me suis **très fortement** inspiré :
[PocketBookSDK.pdf](https://github.com/pocketbook-free/InkViewDoc)

***

Utilisé pour la suite :

```
export PBSDK="$HOME/developpement/PBSDK"
```

Récupération du SDK

```
git clone https://github.com/pocketbook-free/SDK_481.git "$PBSDK"
cd "$PBSDK"
git checkout 5.12
```

Ensuite, depuis le répertoire où on a les sources du projet :

```
"$PBSDK/bin/arm-obreey-linux-gnueabi-g++" demo01.cpp -o demo01.app -linkview
```

=> Ca construit `demo01.app`

Pour vérification :

```
$ file demo01.app
demo01.app: ELF 32-bit LSB executable, ARM, EABI5 version 1 (SYSV), dynamically linked, interpreter /lib/ld-linux.so.3, for GNU/Linux 2.6.35, not stripped
```

Essayons de mettre ça dans le répertoire `applications/` d'une liseuse, puis de le lancer, et on verra ^^

=> Success \o/
