# Zappy

> Jeu réseau multijoueur où des équipes d'IA autonomes explorent une planète
> (Trantor), collectent des ressources et s'élèvent de niveau par incantation.
> La première équipe à amener 6 joueurs au niveau 8 gagne.

Le projet est composé de **trois binaires indépendants** qui communiquent via un
protocole réseau TCP :

| Composant        | Langage                     | Rôle                                               |
| ---------------- | --------------------------- | --------------------------------------------------- |
| `zappy_server` | C++20                       | Cœur du jeu : monde, ressources, règles, sockets. |
| `zappy_gui`    | C++20 (SFML 2D / raylib 3D) | Visualisation temps réel de la partie.             |
| `zappy_ai`     | Python 3                    | Client autonome qui joue une équipe.               |

---

## Prérequis

- **CMake ≥ 3.20** et un compilateur **C++20** (g++/clang++)
- **SFML 2.5** (graphics, window, system, network, audio)
- **raylib 5.5** — récupéré automatiquement par CMake s'il est absent
- **Python 3** (pour l'IA)
- `pip install pytest gcovr` (pour les tests et la couverture)

---

## Compilation

```sh
cmake -B build
cmake --build build -j
```

Les binaires sont générés **à la racine du dépôt** : `zappy_server`, `zappy_gui`,
`zappy_ai`, ainsi que les bibliothèques graphiques `libzappy_graphic_2d.so` /
`libzappy_graphic_3d.so` et le backend réseau `libzappy_network_linux.so`.

Pour ne (re)compiler qu'une cible :

```sh
cmake --build build --target zappy_server -j
```

---

## Lancement

### Rapide (script tout-en-un)

```sh
./launch-self.sh        # serveur local + IA + GUI 2D
```

Le script lance le serveur, une IA, le GUI, puis ferme tout proprement au
`Ctrl+C`. Les paramètres (port, taille, équipes, fréquence) se règlent en tête du
script.

### Manuel (chaque composant séparément)

**Serveur**

```sh
./zappy_server -p 4242 -x 15 -y 15 -n TeamA TeamB TeamC -c 3 -f 100
```

| Option        | Signification                             |
| ------------- | ----------------------------------------- |
| `-p`        | port                                      |
| `-x` `-y` | largeur / hauteur de la map               |
| `-n`        | noms des équipes                         |
| `-c`        | nombre de slots (clients) par équipe     |
| `-f`        | fréquence (unités de temps par seconde) |

Le serveur s'arrête proprement sur `Ctrl+C` (`SIGINT`) ou `SIGTERM`.

**GUI**

```sh
./zappy_gui -p 4242 -h 127.0.0.1 -g ./libzappy_graphic_2d.so
```

(`-g ./libzappy_graphic_3d.so` pour la vue 3D.)

**IA**

```sh
./zappy_ai -p 4242 -n TeamA -h 127.0.0.1
```

---

## Protocole

Le serveur expose deux protocoles sur le même port :

- **IA** — 12 commandes : `Forward`, `Right`, `Left`, `Look`, `Inventory`,
  `Broadcast`, `Connect_nbr`, `Fork`, `Eject`, `Take`, `Set`, `Incantation`.
  Chaque action a un coût en temps de `n/f` unités.
- **GUI** — `msz`, `bct`, `mct`, `tna`, `sgt`, `ppo`, `plv`, `pin`, … (état du
  monde et événements).

---

## Tests

Suite d'intégration **pytest** : elle démarre un vrai `zappy_server` et valide le
protocole en boîte noire (connexion TCP, handshake, réponses des commandes).

```sh
cmake --build build --target zappy_server -j   # binaire requis
python -m pytest tests/test_zappy_com.py -v
```

### Couverture du serveur

Le script recompile le serveur instrumenté (gcov) dans un build isolé
`build-coverage/`, lance la suite pytest, affiche le rapport, puis restaure le
binaire normal :

```sh
tests/coverage_server.sh          # rapport dans le terminal
tests/coverage_server.sh --html   # + coverage_server.html détaillé
```

---

## Documentation (Doxygen)

La documentation est générée à partir des commentaires du code (non versionnée).

**Prérequis** : `doxygen`, et une toolchain LaTeX (`pdflatex`, `make`) pour le PDF.

```sh
# Serveur — HTML : docs/doxygen/html/index.html
doxygen server/Doxyfile
make -C docs/doxygen/latex          # PDF optionnel : docs/doxygen/latex/refman.pdf

# GameEngine — HTML : GameEngine/docs/html/index.html
doxygen GameEngine/Doxyfile
```

Configuration : [server/Doxyfile](server/Doxyfile).

---

## Structure du dépôt

```
server/            # serveur de jeu (C++)
GameEngine/        # GUI + plugins graphiques 2D/3D (C++)
IA/                # client IA (Python)
network_backends/  # backend réseau (sockets/poll) par OS
tests/             # tests d'intégration pytest + coverage
```
