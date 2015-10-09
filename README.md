# SV
Science du Vivant - Alignement
 
Pierre Falez

## Choix d'implémentation

### Méthode d'indexation

J'ai choisi d'implémenter la méthode avec la Transformée Burrows-Wheeler.

Pour la construction de la table des suffixe, j'utilise en complément de la séquence une représentation compréssée (1 nucléotide sur 2 bits) afin d'accélérer le tri :
Lorsque deux index sont à comparer, si ceux-ci sont aligné de la même façon (Modulo 4), alors j'utilise la représentation compréssé afin de comparer les nucléotides 4 par 4, sinon la représentation de base.

Pour la BWT, j'utilise un rank échantillionné pour gagner en place.

### Méthode de recherche

La méthode de recherche se fait grâce à l'algorithme du BWA, afin de permettre la recherche avec erreurs. J'ai choisi d'autoriser 2 erreurs sur les seed, ce qui est un bon compromis entre le temps et la flexibilité de recherche.

### Méthodes d'extension

J'utilise la programmation dynamique sur le prefix puis le suffixe de la seed de façon séparé. Puis je réuni les deux parties.







