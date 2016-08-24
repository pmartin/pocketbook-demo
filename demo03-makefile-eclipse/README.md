Objectifs :

 * Compiler une application simple via un `Makefile`
 * Développer sous Eclipse (en réutilisant le `Makefile` pour compiler), avec auto-complétion + remote-debug en interface graphique


# Un Makefile

Pour le principe des makefiles -> cf des docs / tutoriels.

Ici, je fais au plus simple :

 * Une cible `all` qui builde `demo03.app`
 * `demo03.app` dépend de `demo03.cpp`
 * Une cible `clean` classique.

L'intérêt du `Makefile`, ici : pouvoir compiler en CLI ou sous à peu près n'importe quel éditeur ou IDE, en fonction des préférences de chacun.


# Un IDE : Eclipse

Il existe un paquet d'IDE qui feraient probablement un peu tous l'affaire ; je pars sur Eclipse en premier, que j'ai déjà utilisé plusieurs fois ces dernières années pour développer en C.

## Installation Eclipse

Pour développer sous Eclipse et avoir l'auto-complétion et tout ça :

 * Télécharger l'installeur Eclipse depuis [son site](https://eclipse.org/downloads/)
 * Choisir d'installer "Eclipse IDE for C/C++ Developers"


## Projet sous Eclipse

Ensuite, en ayant un répertoire source `demo03-makefile-eclipse` déjà existant, basé sur un `Makefile`, créer le projet Eclipse correspondant :

 * `File` > `New` > `Makefile Project with existing code`
 * Renseigner les champs de la boite de dialogue :

    * `Languages` : `C++`
    * `Toolchain` : `Cross GCC`

Une fois le projet créé, Eclipse doit réussir à supprimer le binaire créé et à le re-compiler (via `make`, avec un `Makefile` qui fonctionnait déjà en ligne de commandes !) :

 * `Project` > `Clean`
 * `Project` > `Build All`

## Auto-complétion, connaissance des fonctions de la bibliothèque `ink`

L'étape suivante est de configurer Eclipse pour qu'il trouve les fichiers d'en-têtes `.h` qu'on utilise, afin :

 * De ne pas avoir les 3/4 du code souligné en rouge à cause de fonctions / constantes / types non trouvés
 * Avoir de l'auto-complétion, du Ctrl+clic !

Pour ça :

 * Clic droit sur le projet > `Properties`
 * `C/C++ General` > `Paths and Symbols` > `Includes` > `GNU C++` > `Add...`
 * Ajouter : `/home/squale/developpement/PBSDK/arm-obreey-linux-gnueabi/sysroot/usr/local/include` ou `${PBSDK}/arm-obreey-linux-gnueabi/sysroot/usr/local/include` si vous avez défini la variable d'environnement ;-)
 * Dès que vous aurez validé la boite de dialogue, toutes les erreurs de *fonctions inconnues* et similaires soulignées en rouge dans l'IDE devraient disparaitre \o/ Et vous aurez de l'auto-complétion \o/

**A VOIR** : il faudra possiblement faire pareil avec quelques autres répertoires d'includes ailleurs dans le SDK ; liste à compléter, donc, en fonction des découvertes futures !


## Remote debug (en interface graphique sous Eclipse + exécution du code sur liseuse)

### Configuration de debug

Création de la configuration de debug, sous Eclipse :

 * `Run` > `Debug Configurations...`
 * Clic droit sur `C/C++ Remote Application` > `New`
 * Configurer (en bas) : `Using GDB (DSF) Manual Remote Debugging Launcher`
 * Onglet `Main` :

    * `Project` : `demo03-makefile-eclipse`
    * `C/C++ Application` : `demo03.app`

 * Onglet `Debugger` :

    * `Main` :

       * `GDB debugger` : `/home/pmartin/developpement/PBSDK/bin/arm-obreey-linux-gnueabi-gdb`
       * `GDB commande file` : `.gdbinit`

    * `Shared Libraries` :

       * Ajouter : `/home/pmartin/developpement/PBSDK/arm-obreey-linux-gnueabi/sysroot/usr/local/lib`

    * `Connection` :

       * `Type : `TCP`
       * `Host name or IP address` : `192.168.0.12`     <- l'IP de la liseuse
       * `Port number` : `10002`    <- le même que celui utilisé pour lancer `gdbserver` sur la liseuse


### Lancement du déboggage

Sur la liseuse, lancer `gdbserver`, sur votre application et le port spécifié dans Eclipse -- comme fait précédemment (cf `demo02-gdb`).

Depuis Eclipse : `Debug`


### Profit \o/

![Screenshot d'Eclipse en session de deboggage](http://extern.pascal-martin.fr/pocketbook-demo/demo03-debug-eclipse-tafOveyfsiv2.png)


### Troubleshooting

Astuce, si vous avez l'erreur suivante, vérifiez que la liseuse soit encore connectée au wifi !

```
Error in final launch sequence
Failed to execute MI command:
-target-select remote 192.168.0.12:10002
Error message from debugger back end:
192.168.0.12:10002: Aucun chemin d'accès pour atteindre l'hôte cible.
192.168.0.12:10002: Aucun chemin d'accès pour atteindre l'hôte cible.
```

Elle se déconnecte généralement assez vite ;-)
