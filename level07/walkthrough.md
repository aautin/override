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

n -> pour aller au second breakpoint

p/x $esp -> pour voir a quelle adresse est l'esp qui pointe sur l'eip sauvegardé
$1 = 0xffffd548 -> on retire 4 pour l'adresse de base car l'index 0 est bloqué

## Reverse engineering

## Exploitation
