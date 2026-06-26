#!/bin/bash

# ==========================================
# Configuration des variables du jeu
# ==========================================
PORT=4242
WIDTH=25
HEIGHT=10
TEAM1="TeamA"
TEAM2="TeamB"
TEAM3="TeamC"
CLIENTS_NB=3
FREQ=100
MACHINE="127.0.0.1"
MODE="./libzappy_graphic_3d.so"

# ==========================================
# Fonction de nettoyage (Clean exit)
# ==========================================
# Se déclenche quand on fait un Ctrl+C
cleanup() {
    echo -e "\n[+] Arrêt de tous les processus Zappy..."
    pkill -f "./zappy_server"
    pkill -f "./zappy_gui"
    pkill -f "./zappy_ai"
    pkill -f "python3.*zappy"
    pkill -f "python.*zappy"
    echo "[+] Terminé."
    exit 0
}

# Intercepte le signal d'interruption (Ctrl+C)
trap cleanup SIGINT

# ==========================================
# 1. Lancement du Serveur
# ==========================================
echo "[+] Lancement du zappy_server..."
# Options du serveur basées sur le sujet
./zappy_server -p $PORT -x $WIDTH -y $HEIGHT -n $TEAM1 $TEAM2 $TEAM3 -c $CLIENTS_NB -f $FREQ &
SERVER_PID=$!

# Petite pause pour laisser le temps au serveur d'ouvrir ses sockets réseau
sleep 1

# ==========================================
# 3. Lancement des IA
# ==========================================
echo "[+] Lancement des clients IA..."
# Lancement de 2 drones pour la TeamA et 1 pour la TeamB.
# TeamA sera bleue et TeamB sera rouge dans le GUI.
# On redirige la sortie standard vers /dev/null pour ne pas polluer ton terminal
./zappy_ai -p $PORT -n $TEAM1 -h $MACHINE > /dev/null 2>&1 &

echo "[+] Les IA sont lancées : $TEAM1"
echo "[!] Appuie sur Ctrl+C dans ce terminal pour tout fermer proprement."

sleep 3

# ==========================================
# 2. Lancement du GUI
# ==========================================
echo "[+] Lancement du zappy_gui..."
# Le GUI a besoin du port et de l'hôte
./zappy_gui -p $PORT -h $MACHINE -g $MODE

# On attend que le processus du serveur se termine (bloque le script ici)
wait $SERVER_PID
