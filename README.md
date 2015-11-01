# Science du Vivant - Alignement
 
Pierre Falez

## Choix d'implémentation

Je pré-traite toute les chaines afin de remplacer les bases non-conventionnelle vers les 4 bases ADN (remplacement également pour uniformiser les minuscules/majuscules). 

### Méthode d'indexation

J'ai choisi d'implémenter la méthode d'indexationavec la Transformée Burrows-Wheeler.

Pour la BWT, j'utilise un rank échantillionné pour gagner en place (echantillionnage de 32).

De plus, je calcule la BWT et le rank sur l'inverse de la séquence dans le but de borner les erreurs dans la phase de recherche (Algorithme BWA).

### Méthode de recherche

La méthode de recherche se fait grâce à l'algorithme du BWA, afin de permettre la recherche avec erreurs. J'ai choisi d'autoriser 2 erreurs sur les seed, ce qui est un bon compromis entre le temps et la flexibilité de recherche.
De plus, j'utilise des seeds de 25 nucléotides de long.
Si la recherche ne donne pas de résultats dans un sens, j'inverse la chaine et je refais la recherche dessus.

### Méthodes d'extension

J'utilise la programmation dynamique sur le prefix puis le suffixe de la seed de façon séparé. Puis je réuni les deux parties.


## Benchmark

Le programme utilise un seule coeur logique en même temps.
L'ensemble des benchmarks est réalisé sur un i5 4690K @ 3.5 Ghz et une ram @ 2133 MHz

J'utilise 10% d'erreurs maximum sur l'alignement

Pour l'alignement de génome d'une souche Ebola du Zaïre avec les 3389 read, j'ai tenter de lancer le programme avec différents parametres :
En prennant des seeds de 25 de long, 2 erreurs lors de la recherche et en déplacant la seed de 25 sur le read j'obtient le programme met 60.83 secondes et utilise au maximum 3 232 Kb de mémoire. De plus il réussi à aligner 82% des reads.

En faisant un alignement sans erreurs sur chaque position possible de la seed sur le read, le programme met 10.44 secondes et 3 492 Kb de mémoire et aligne 81% des reads.

Pour l'alignement du chromosome 21 Humain, avec les 20000 premiers read, le programme met 1409.25 secondes et utilse 686 876 Kb pour des read de 25 avec 2 erreurs et des décalage des seeds sur le read de 25 et aligne 37.72% des seeds

Pour une recherche sans erreurs avec toute les position de seed sur le read, le programme met 356.93 secondes et utilise au maximum 686 904 Kb de mémoire et se termine avec 34.74 % d'alignement

## Utilisation

Pour compiler le projet, if faut utiliser CMake :
Sur un systeme linux, il faut utiliser les commandes dans le répertoire racine du projet
cmake . -G"Unix Makefiles"
make

Pour lancer le programme :
./Alignment <fichier_fasta> <fichier_fastq>







