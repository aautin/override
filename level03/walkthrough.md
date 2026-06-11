# XOR PASSWORD CHECK

## Analyse du programme

### decrypt

```shell
(gdb) p (char*) 0x80489d4
$1 = 0x80489d4 "/bin/sh"
(gdb) p (char*) 0x80489c3
$2 = 0x80489c3 "Congratulations!"
```
```asm
 8048673:	c7 45 e3 51 7d 7c 75 	mov    DWORD PTR [ebp-0x1d],0x757c7d51
 804867a:	c7 45 e7 60 73 66 67 	mov    DWORD PTR [ebp-0x19],0x67667360
 8048681:	c7 45 eb 7e 73 66 7b 	mov    DWORD PTR [ebp-0x15],0x7b66737e
 8048688:	c7 45 ef 7d 7c 61 33 	mov    DWORD PTR [ebp-0x11],0x33617c7d
 804868f:	c6 45 f3 00          	mov    BYTE PTR [ebp-0xd],0x0
```
```asm
 80486c7:	8d 45 e3             	lea    eax,[ebp-0x1d]
 80486ca:	03 45 d8             	add    eax,DWORD PTR [ebp-0x28]
 80486cd:	0f b6 00             	movzx  eax,BYTE PTR [eax]
 80486d0:	89 c2                	mov    edx,eax
 80486d2:	8b 45 08             	mov    eax,DWORD PTR [ebp+0x8]
 80486d5:	31 d0                	xor    eax,edx
 80486d7:	89 c2                	mov    edx,eax
 80486d9:	8d 45 e3             	lea    eax,[ebp-0x1d]
 80486dc:	03 45 d8             	add    eax,DWORD PTR [ebp-0x28]
 80486df:	88 10                	mov    BYTE PTR [eax],dl
 80486e1:	83 45 d8 01          	add    DWORD PTR [ebp-0x28],0x1
 80486e5:	8b 45 d8             	mov    eax,DWORD PTR [ebp-0x28]
 80486e8:	3b 45 dc             	cmp    eax,DWORD PTR [ebp-0x24]
 80486eb:	72 da                	jb     80486c7 <decrypt+0x67>
```
```asm
 80486ed:	8d 45 e3             	lea    eax,[ebp-0x1d]
 80486f0:	89 c2                	mov    edx,eax
 80486f2:	b8 c3 89 04 08       	mov    eax,0x80489c3
 80486f7:	b9 11 00 00 00       	mov    ecx,0x11
 80486fc:	89 d6                	mov    esi,edx
 80486fe:	89 c7                	mov    edi,eax
 8048700:	f3 a6                	repz cmps BYTE PTR ds:[esi],BYTE PTR es:[edi]
 8048702:	0f 97 c2             	seta   dl
 8048705:	0f 92 c0             	setb   al
 8048708:	89 d1                	mov    ecx,edx
 804870a:	28 c1                	sub    cl,al
 804870c:	89 c8                	mov    eax,ecx
 804870e:	0f be c0             	movsx  eax,al
 8048711:	85 c0                	test   eax,eax
 8048713:	75 0e                	jne    8048723 <decrypt+0xc3>
 8048715:	c7 04 24 d4 89 04 08 	mov    DWORD PTR [esp],0x80489d4
 804871c:	e8 bf fd ff ff       	call   80484e0 <system@plt>
```
- At some point, in the decrypt() function, the program compares something with the string "Congratulations!" using `cmps` instruction. And if it's equal it opens the shell
- It compares it with thoses values at [ebp-0x1d], [ebp-0x19], [ebp-0x15], [ebp-0x11] and [ebp-0xd]
- Those values are xor-ed with the parameter passed to decrypt(), and stored back at the same addresses

### test

```asm
08048747 <test>:
 8048747:	55                   	push   ebp
 8048748:	89 e5                	mov    ebp,esp
 804874a:	83 ec 28             	sub    esp,0x28
 804874d:	8b 45 08             	mov    eax,DWORD PTR [ebp+0x8]
 8048750:	8b 55 0c             	mov    edx,DWORD PTR [ebp+0xc]
 8048753:	89 d1                	mov    ecx,edx
 8048755:	29 c1                	sub    ecx,eax
 8048757:	89 c8                	mov    eax,ecx
 8048759:	89 45 f4             	mov    DWORD PTR [ebp-0xc],eax
 804875c:	83 7d f4 15          	cmp    DWORD PTR [ebp-0xc],0x15
 8048760:	0f 87 e4 00 00 00    	ja     804884a <test+0x103>
 ...
 8048775:	8b 45 f4             	mov    eax,DWORD PTR [ebp-0xc]
 8048778:	89 04 24             	mov    DWORD PTR [esp],eax
 804877b:	e8 e0 fe ff ff       	call   8048660 <decrypt>
 ...
 804884a:	e8 d1 fc ff ff       	call   8048520 <rand@plt>
 804884f:	89 04 24             	mov    DWORD PTR [esp],eax
 8048852:	e8 09 fe ff ff       	call   8048660 <decrypt>
```
- It's taking the difference between the two parameters passed to test() and if param2 - param1 > 0x15 (21) bigger it jumps to a decrypt() call with a random value, otherwise it calls it with the difference as parameter

### main

```shell
(gdb) p (char*) 0x8048a7b
$1 = 0x8048a7b "Password:"
(gdb) p (char*) 0x8048a85
$2 = 0x8048a85 "%d"
```
```asm
0804885a <main>:
 804885a:	55                   	push   ebp
 804885b:	89 e5                	mov    ebp,esp
 804885d:	83 e4 f0             	and    esp,0xfffffff0
 8048860:	83 ec 20             	sub    esp,0x20
 8048863:	50                   	push   eax
 8048864:	31 c0                	xor    eax,eax
 8048866:	74 03                	je     804886b <main+0x11>
 8048868:	83 c4 04             	add    esp,0x4
 804886b:	58                   	pop    eax
 ...
 80488a4:	b8 7b 8a 04 08       	mov    eax,0x8048a7b
 80488a9:	89 04 24             	mov    DWORD PTR [esp],eax
 80488ac:	e8 cf fb ff ff       	call   8048480 <printf@plt>
 80488b1:	b8 85 8a 04 08       	mov    eax,0x8048a85
 80488b6:	8d 54 24 1c          	lea    edx,[esp+0x1c]
 80488ba:	89 54 24 04          	mov    DWORD PTR [esp+0x4],edx
 80488be:	89 04 24             	mov    DWORD PTR [esp],eax
 80488c1:	e8 6a fc ff ff       	call   8048530 <__isoc99_scanf@plt>
 80488c6:	8b 44 24 1c          	mov    eax,DWORD PTR [esp+0x1c]
 80488ca:	c7 44 24 04 0d d0 37 	mov    DWORD PTR [esp+0x4],0x1337d00d
 80488d1:	13 
 80488d2:	89 04 24             	mov    DWORD PTR [esp],eax
 80488d5:	e8 6d fe ff ff       	call   8048747 <test>
 80488da:	b8 00 00 00 00       	mov    eax,0x0
 80488df:	c9                   	leave  
 80488e0:	c3                   	ret    
```
- Main function basically prints context strings and takes an input from the user, then it calls test() with the number input and the constant 0x1337d00d

## Exploit

To sum up, to arrives at the system("/bin/sh") call our input needs to be such that:
- 0x1337d00d - input <= 0x15 (21), so input >= 0x1337cff8
- 'C' XOR (0x1337d00d - input) = 0x51 (of [ebp-0x1d]) ('C' from "Congratulations!")
- 0x1337d00d - input = 0x51 XOR 'C'
- 0x1337d00d - input = 0x12
- input = 0x1337d00d - 0x12 = 0x1337cffb = 322424827

```shell
~$ ./level03 
***********************************
*		level03		**
***********************************
Password:322424827
$ cat /home/users/level04/.pass
kgv3tkEb9h2mLkRsPkXRfc2mHbjMxQzvb2FrgKkf
```