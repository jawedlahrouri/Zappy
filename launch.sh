#!/bin/bash

# ==========================================
# Configuration des variables du jeu
# ==========================================
PORT=4242
WIDTH=15
HEIGHT=15
TEAM1="TeamA"
TEAM2="TeamB"
CLIENTS_NB=3
FREQ=100
MACHINE="127.0.0.1"

DEBUG_MODE=0
while getopts "d" opt; do
  case ${opt} in
    d )
      DEBUG_MODE=1
      ;;
    \? )
      echo "Usage: ./lunch.sh [-d]"
      exit 1
      ;;
  esac
done
rm logs/TeamA_logs.txt
# ==========================================
# Fonction de nettoyage (Clean exit)
# ==========================================
# Se déclenche quand on fait un Ctrl+C
cleanup() {
    echo -e "\n[+] Arrêt de tous les processus Zappy..."
    pkill -f "./server/zappy_ref-v3.0.1/linux/zappy_server"
    pkill -f "./zappy_gui"
    pkill -f "./zappy_ai"
    pkill -f "auto_connect.py"
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
./server/zappy_ref-v3.0.1/linux/zappy_server -p $PORT -x $WIDTH -y $HEIGHT -n $TEAM1 -c $CLIENTS_NB -f $FREQ &
SERVER_PID=$!

# Petite pause pour laisser le temps au serveur d'ouvrir ses sockets réseau
sleep 1

# ==========================================
# 3. Lancement des IA
# ==========================================
echo "[+] Lancement de l'auto-connect IA..."
# Lancement d'auto_connect.py qui surveille les slots et spawne les agents dynamiquement
python3 IA/auto_connect.py -p $PORT -n $TEAM1 -h $MACHINE &
AUTOCONNECT_PID=$!
echo "  -> Auto-connect lancé (PID: $AUTOCONNECT_PID)"

echo "[+] Tout est lancé ! Regarde la fenêtre du GUI."
echo "[!] Appuie sur Ctrl+C dans ce terminal pour tout fermer proprement."

sleep 3

# ==========================================
# 2. Lancement du GUI
# ==========================================
echo "[+] Lancement du zappy_gui..."
# Le GUI a besoin du port et de l'hôte
if [ $DEBUG_MODE -eq 1 ]; then
    echo "[!] Mode DEBUG: Lancement de zappy_gui.AppImage"
    env WAYLAND_DISPLAY="" ./zappy_gui.AppImage -p $PORT -h $MACHINE &
else
    env WAYLAND_DISPLAY="" ./zappy_gui -p $PORT -h $MACHINE &
fi

# On attend que le processus du serveur se termine (bloque le script ici)
wait $SERVER_PID
