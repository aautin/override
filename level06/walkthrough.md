# ALGORITHM CRACKING

## Reverse engineering

### Main function

```shell
(gdb) p (char*) 0x8048ad4
$1 = 0x8048ad4 '*' <repeats 35 times>
(gdb) p (char*) 0x8048af8
$2 = 0x8048af8 "*\t\tlevel06\t\t  *"
(gdb) p (char*) 0x8048b08
$3 = 0x8048b08 "-> Enter Login: "
```
```asm
08048879 <main>:
 8048879:	55                   	push   ebp
 804887a:	89 e5                	mov    ebp,esp
 804887c:	83 e4 f0             	and    esp,0xfffffff0
 804887f:	83 ec 50             	sub    esp,0x50
 8048882:	8b 45 0c             	mov    eax,DWORD PTR [ebp+0xc]
 8048885:	89 44 24 1c          	mov    DWORD PTR [esp+0x1c],eax
 8048889:	65 a1 14 00 00 00    	mov    eax,gs:0x14
 804888f:	89 44 24 4c          	mov    DWORD PTR [esp+0x4c],eax
 8048893:	31 c0                	xor    eax,eax
 8048895:	50                   	push   eax
 8048896:	31 c0                	xor    eax,eax
 8048898:	74 03                	je     804889d <main+0x24>
 804889a:	83 c4 04             	add    esp,0x4
 804889d:	58                   	pop    eax
 804889e:	c7 04 24 d4 8a 04 08 	mov    DWORD PTR [esp],0x8048ad4
 80488a5:	e8 e6 fc ff ff       	call   8048590 <puts@plt>
 80488aa:	c7 04 24 f8 8a 04 08 	mov    DWORD PTR [esp],0x8048af8
 80488b1:	e8 da fc ff ff       	call   8048590 <puts@plt>
 80488b6:	c7 04 24 d4 8a 04 08 	mov    DWORD PTR [esp],0x8048ad4
 80488bd:	e8 ce fc ff ff       	call   8048590 <puts@plt>
 80488c2:	b8 08 8b 04 08       	mov    eax,0x8048b08
 80488c7:	89 04 24             	mov    DWORD PTR [esp],eax
 80488ca:	e8 41 fc ff ff       	call   8048510 <printf@plt>
```
- Prints context strins and the prompt for the login.

```shell
(gdb) p (char*) 0x804a060
$4 = 0x804a060 ""
```
```asm
 80488cf:	a1 60 a0 04 08       	mov    eax,ds:0x804a060
 80488d4:	89 44 24 08          	mov    DWORD PTR [esp+0x8],eax
 80488d8:	c7 44 24 04 20 00 00 	mov    DWORD PTR [esp+0x4],0x20
 80488df:	00 
 80488e0:	8d 44 24 2c          	lea    eax,[esp+0x2c]
 80488e4:	89 04 24             	mov    DWORD PTR [esp],eax
 80488e7:	e8 64 fc ff ff       	call   8048550 <fgets@plt>
```
- Fgets on stdin, with a buffer of 32 bytes, and storing the result in [esp+0x2c].

```shell
(gdb) p (char*) 0x8048ad4
$5 = 0x8048ad4 '*' <repeats 35 times>
(gdb) p (char*) 0x8048b1c
$6 = 0x8048b1c "***** NEW ACCOUNT DETECTED ********"
(gdb) p (char*) 0x8048b40
$7 = 0x8048b40 "-> Enter Serial: "
```
```asm
 80488ec:	c7 04 24 d4 8a 04 08 	mov    DWORD PTR [esp],0x8048ad4
 80488f3:	e8 98 fc ff ff       	call   8048590 <puts@plt>
 80488f8:	c7 04 24 1c 8b 04 08 	mov    DWORD PTR [esp],0x8048b1c
 80488ff:	e8 8c fc ff ff       	call   8048590 <puts@plt>
 8048904:	c7 04 24 d4 8a 04 08 	mov    DWORD PTR [esp],0x8048ad4
 804890b:	e8 80 fc ff ff       	call   8048590 <puts@plt>
 8048910:	b8 40 8b 04 08       	mov    eax,0x8048b40
 8048915:	89 04 24             	mov    DWORD PTR [esp],eax
 8048918:	e8 f3 fb ff ff       	call   8048510 <printf@plt>
```
- Prints context strings and the prompt for the serial.

```shell
(gdb) p (char*) 0x8048a60
$8 = 0x8048a60 "%u"
```
```asm
 804891d:	b8 60 8a 04 08       	mov    eax,0x8048a60
 8048922:	8d 54 24 28          	lea    edx,[esp+0x28]
 8048926:	89 54 24 04          	mov    DWORD PTR [esp+0x4],edx
 804892a:	89 04 24             	mov    DWORD PTR [esp],eax
 804892d:	e8 ae fc ff ff       	call   80485e0 <__isoc99_scanf@plt>
```
- Scanf an unsingned integer from stdin, and storing the result in [esp+0x28].

```asm
 8048932:	8b 44 24 28          	mov    eax,DWORD PTR [esp+0x28]
 8048936:	89 44 24 04          	mov    DWORD PTR [esp+0x4],eax
 804893a:	8d 44 24 2c          	lea    eax,[esp+0x2c]
 804893e:	89 04 24             	mov    DWORD PTR [esp],eax
 8048941:	e8 02 fe ff ff       	call   8048748 <auth>
```
- Calls auth with the login and serial as arguments.

```shell
(gdb) p (char*) 0x8048b52
$9 = 0x8048b52 "Authenticated!"
(gdb) p (char*) 0x8048b61
$10 = 0x8048b61 "/bin/sh"
```
```asm
 8048946:	85 c0                	test   eax,eax
 8048948:	75 1f                	jne    8048969 <main+0xf0>
 804894a:	c7 04 24 52 8b 04 08 	mov    DWORD PTR [esp],0x8048b52
 8048951:	e8 3a fc ff ff       	call   8048590 <puts@plt>
 8048956:	c7 04 24 61 8b 04 08 	mov    DWORD PTR [esp],0x8048b61
 804895d:	e8 3e fc ff ff       	call   80485a0 <system@plt>
```
- If auth returned 0 : prints "Authenticated!" and calls system("/bin/sh")

### Auth function

Auth parameters are initially stored in the stack as follows :
- [ebp+0x8] : login
- [ebp+0xc] : serial

```shell
(gdb) p (char*) 0x8048a63
$11 = 0x8048a63 "\n"
```
```asm
08048748 <auth>:
 8048748:	55                   	push   ebp
 8048749:	89 e5                	mov    ebp,esp
 804874b:	83 ec 28             	sub    esp,0x28
 804874e:	c7 44 24 04 63 8a 04 	mov    DWORD PTR [esp+0x4],0x8048a63
 8048755:	08 
 8048756:	8b 45 08             	mov    eax,DWORD PTR [ebp+0x8]
 8048759:	89 04 24             	mov    DWORD PTR [esp],eax
 804875c:	e8 bf fd ff ff       	call   8048520 <strcspn@plt>
 8048761:	03 45 08             	add    eax,DWORD PTR [ebp+0x8]
 8048764:	c6 00 00             	mov    BYTE PTR [eax],0x0
```
- Replaces the first '\n' character in the login with '\0'. Equivalent to login[strcspn(login, "\n")] = 0;

```asm
 8048767:	c7 44 24 04 20 00 00 	mov    DWORD PTR [esp+0x4],0x20
 804876e:	00 
 804876f:	8b 45 08             	mov    eax,DWORD PTR [ebp+0x8]
 8048772:	89 04 24             	mov    DWORD PTR [esp],eax
 8048775:	e8 56 fe ff ff       	call   80485d0 <strnlen@plt>
```
- Computes the length of the login string, up to a maximum of 32 characters

```asm
 804877a:	89 45 f4             	mov    DWORD PTR [ebp-0xc],eax
 804877d:	50                   	push   eax
 804877e:	31 c0                	xor    eax,eax
 8048780:	74 03                	je     8048785 <auth+0x3d>
 8048782:	83 c4 04             	add    esp,0x4
 8048785:	58                   	pop    eax
 8048786:	83 7d f4 05          	cmp    DWORD PTR [ebp-0xc],0x5
 804878a:	7f 0a                	jg     8048796 <auth+0x4e>
 804878c:	b8 01 00 00 00       	mov    eax,0x1
 8048791:	e9 e1 00 00 00       	jmp    8048877 <auth+0x12f>
 ...
 8048877:	c9                   	leave
```
- Store the length of the login string in [ebp-0xc]
- If its more than 5 characters, continue, else return 1 (failure)

```shell
(gdb) p (char*) 0x8048a68
$1 = 0x8048a68 "\033[32m.", '-' <repeats 27 times>, "."
(gdb) p (char*) 0x8048a8c
$2 = 0x8048a8c "\033[31m| !! TAMPERING DETECTED !!  |"
(gdb) p (char*) 0x8048ab0
$3 = 0x8048ab0 "\033[32m'", '-' <repeats 27 times>, "'"
```
```asm
 8048796:	c7 44 24 0c 00 00 00 	mov    DWORD PTR [esp+0xc],0x0
 804879d:	00 
 804879e:	c7 44 24 08 01 00 00 	mov    DWORD PTR [esp+0x8],0x1
 80487a5:	00 
 80487a6:	c7 44 24 04 00 00 00 	mov    DWORD PTR [esp+0x4],0x0
 80487ad:	00 
 80487ae:	c7 04 24 00 00 00 00 	mov    DWORD PTR [esp],0x0
 80487b5:	e8 36 fe ff ff       	call   80485f0 <ptrace@plt>
 80487ba:	83 f8 ff             	cmp    eax,0xffffffff
 80487bd:	75 2e                	jne    80487ed <auth+0xa5>
 80487bf:	c7 04 24 68 8a 04 08 	mov    DWORD PTR [esp],0x8048a68
 80487c6:	e8 c5 fd ff ff       	call   8048590 <puts@plt>
 80487cb:	c7 04 24 8c 8a 04 08 	mov    DWORD PTR [esp],0x8048a8c
 80487d2:	e8 b9 fd ff ff       	call   8048590 <puts@plt>
 80487d7:	c7 04 24 b0 8a 04 08 	mov    DWORD PTR [esp],0x8048ab0
 80487de:	e8 ad fd ff ff       	call   8048590 <puts@plt>
 80487e3:	b8 01 00 00 00       	mov    eax,0x1
 80487e8:	e9 8a 00 00 00       	jmp    8048877 <auth+0x12f>
```
- Calls ptrace with PTRACE_TRACEME, 0, 1, 0 : if the program is being debugged with gdb, it prints context strings and returns
- Tried it with gdb, and indeed, it did that.

```asm
 80487ed:	8b 45 08             	mov    eax,DWORD PTR [ebp+0x8]
 80487f0:	83 c0 03             	add    eax,0x3
 80487f3:	0f b6 00             	movzx  eax,BYTE PTR [eax]
 80487f6:	0f be c0             	movsx  eax,al
 80487f9:	35 37 13 00 00       	xor    eax,0x1337
 80487fe:	05 ed ed 5e 00       	add    eax,0x5eeded
 8048803:	89 45 f0             	mov    DWORD PTR [ebp-0x10],eax
```
- Dereferencing on the 4th character of the login, xoring on it with 0x1337, and adding 0x5eeded to it.
- The result is stored in [ebp-0x10], we will call it the hash

```asm
 8048806:	c7 45 ec 00 00 00 00 	mov    DWORD PTR [ebp-0x14],0x0
 804880d:	eb 4c                	jmp    804885b <auth+0x113>
 804880f:	8b 45 ec             	mov    eax,DWORD PTR [ebp-0x14]
 8048812:	03 45 08             	add    eax,DWORD PTR [ebp+0x8]
 8048815:	0f b6 00             	movzx  eax,BYTE PTR [eax]
 8048818:	3c 1f                	cmp    al,0x1f
 804881a:	7f 07                	jg     8048823 <auth+0xdb>
 804881c:	b8 01 00 00 00       	mov    eax,0x1
 8048821:	eb 54                	jmp    8048877 <auth+0x12f>
```
- Iterating in every character of the login, and checking if it is greater than 0x1f (31 in decimal). If it is not, return 1 (failure).

```asm
 8048823:	8b 45 ec             	mov    eax,DWORD PTR [ebp-0x14]
 8048826:	03 45 08             	add    eax,DWORD PTR [ebp+0x8]
 8048829:	0f b6 00             	movzx  eax,BYTE PTR [eax]
 804882c:	0f be c0             	movsx  eax,al
 804882f:	89 c1                	mov    ecx,eax
 8048831:	33 4d f0             	xor    ecx,DWORD PTR [ebp-0x10]
 8048834:	ba 2b 3b 23 88       	mov    edx,0x88233b2b
 8048839:	89 c8                	mov    eax,ecx
 804883b:	f7 e2                	mul    edx
 804883d:	89 c8                	mov    eax,ecx
 804883f:	29 d0                	sub    eax,edx
 8048841:	d1 e8                	shr    eax,1
 8048843:	01 d0                	add    eax,edx
 8048845:	c1 e8 0a             	shr    eax,0xa
 8048848:	69 c0 39 05 00 00    	imul   eax,eax,0x539
 804884e:	89 ca                	mov    edx,ecx
 8048850:	29 c2                	sub    edx,eax
 8048852:	89 d0                	mov    eax,edx
 8048854:	01 45 f0             	add    DWORD PTR [ebp-0x10],eax
```
- Taking the hash at [ebp-0x10], and adding to it the result of a complex operation : (current_char XOR hash) MODULO 0x539. The result is stored back in [ebp-0x10].

```asm
 8048863:	8b 45 0c             	mov    eax,DWORD PTR [ebp+0xc]
 8048866:	3b 45 f0             	cmp    eax,DWORD PTR [ebp-0x10]
 8048869:	74 07                	je     8048872 <auth+0x12a>
 804886b:	b8 01 00 00 00       	mov    eax,0x1
 8048870:	eb 05                	jmp    8048877 <auth+0x12f>
 8048872:	b8 00 00 00 00       	mov    eax,0x0
 8048877:	c9                   	leave  
```
- At the end of the loop, compares the hash with the serial, if they are equal it return 0, otherwise it returns 1.

## Exploitation

Let's sum up the exploitation possibilities of this program :
- The program use a complex algorithm to compute a hash from the login, then it compares it with the serial
- We cant put a breakpoint at the end of the auth function to get the hash of a login we put, because of the ptrace
- We can reimplement the auth function in python and use it to compute the hash of a given login

### Exploit

```shell
python3 level06/Resources/serialCracker.py aautin
aautin hash is 6232794
```
- In host machine

```shell
~$ ./level06
***********************************
*		level06		  *
***********************************
-> Enter Login: aautin
***********************************
***** NEW ACCOUNT DETECTED ********
***********************************
-> Enter Serial: 6232794
Authenticated!
$ cat /home/users/level07/.pass
GbcPDRgsFK77LNnnuh7QyFYA2942Gp8yKj9KrWD8
```
- In override machine