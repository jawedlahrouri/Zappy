# Zappy-1

## Documentation (Doxygen)

La documentation du serveur est générée à partir des commentaires Doxygen du code
(elle n'est **pas** versionnée : voir `.gitignore`).

### Prérequis

- `doxygen`
- une toolchain LaTeX (`pdflatex`, `make`) — uniquement pour le PDF

### Générer la doc

Depuis la racine du dépôt :

#### Documentation Server
```sh
# HTML : sortie dans docs/doxygen/html/index.html
doxygen server/Doxyfile

# PDF (optionnel) : sortie dans docs/doxygen/latex/refman.pdf
make -C docs/doxygen/latex
```

#### Documentation GameEngine
```sh
doxygen GameEngine/Doxyfile 
xdg-open GameEngine/docs/html/index.html
```

La configuration se trouve dans [server/Doxyfile](server/Doxyfile).