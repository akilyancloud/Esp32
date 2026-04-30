📡 TP ESP32 – Communication avec un serveur Web
🎯 **Objectif**

Ce TP permet de comprendre comment un *ESP32* peut communiquer avec un serveur web.
On apprend à envoyer des données, mais aussi à recevoir des commandes depuis une page web pour contrôler des composants comme une LED ou un buzzer.

🧩 Partie 1 – Contrôler l’ESP32 depuis une page Web

On commence par modifier une page web hébergée sur un serveur Ubuntu.
Dans cette page, on ajoute des boutons qui permettent d’envoyer des requêtes vers l’ESP32.

Chaque bouton correspond à une action. Par exemple :

un bouton pour allumer une LED
un bouton pour activer un buzzer

Quand on clique sur un bouton, une requête est envoyée à l’adresse IP de l’ESP32.
⚠️ Il faut remplacer cette adresse IP par celle de votre propre ESP32.
