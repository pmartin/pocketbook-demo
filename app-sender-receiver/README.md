
# Objectif

 * Une application à lancer sur la liseuse pour en recevoir d'autres
 * Un shell-script à lancer sur PC pour envoyer une application à la liseuse

Le tout : sans brancher de cable USB \o/


# Usage

Sur liseuse, lancer `app-receiver.app` depuis le drawer d'applications.

Sur PC, lancer la commande transfert :

```
./app-sender.sh "$HOME/pocketbook/applications/plop.app" "plop.app" "192.168.0.12"
```
