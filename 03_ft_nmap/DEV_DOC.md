
# parser tests:
## aceptas

### 🔹 IP + ports básicos
```bash
./ft_nmap --ip 8.8.8.8 --ports 80
./ft_nmap --ip 1.1.1.1 --ports 443
./ft_nmap --ip 127.0.0.1 --ports 65535
./ft_nmap --ip 127.0.0.1 --ports 80,
```

### 🔹 Lista de puertos
```bash
./ft_nmap --ip 8.8.8.8 --ports 80,443,8080
./ft_nmap --ip 192.168.1.1 --ports 22,80,443,53,
```

### 🔹 Rangos
```bash
./ft_nmap --ip 8.8.8.8 --ports 1-1024
./ft_nmap --ip 10.0.0.1 --ports 80-90
```

### 🔹 Mixto (IMPORTANTE)
```bash
./ft_nmap --ip 8.8.8.8 --ports 22,80-90,443
./ft_nmap --ip 127.0.0.1 --ports 1,3-5,8,10-12
```

### 🔹 Scans simples
```bash
./ft_nmap --ip 8.8.8.8 --scan SYN
./ft_nmap --ip 8.8.8.8 --scan ACK
./ft_nmap --ip 8.8.8.8 --scan NULL
./ft_nmap --ip 8.8.8.8 --scan FIN
./ft_nmap --ip 8.8.8.8 --scan XMAS
./ft_nmap --ip 8.8.8.8 --scan UDP
```

### 🔹 Scans múltiples
```bash
./ft_nmap --ip 8.8.8.8 --scan SYN,ACK
./ft_nmap --ip 192.168.1.1 --scan SYN,NULL,FIN
./ft_nmap --ip 8.8.8.8 --scan SYN,ACK,FIN,XMAS,UDP
./ft_nmap --ip 125.112.111.10 --scan SYN,ACK,FIN,XMAS,NULL,UDP
./ft_nmap --ip 8.8.8.8 --scan SYN,
```

### 🔹 Full combo (stress parser)
```bash
./ft_nmap --ip 8.8.8.8 --ports 80,443 --scan SYN,ACK --speedup 10
./ft_nmap --ip 1.1.1.1 --ports 22,80-85,443 --scan SYN --speedup 50
./ft_nmap --ip 127.0.0.1 --ports 1-1024 --scan SYN,ACK,FIN,XMAS,NULL,UDP --speedup 250
```

### 🔹 Defaults esperados
```bash
./ft_nmap --ip 8.8.8.8
./ft_nmap --ip 127.0.0.1 --speedup 0
./ft_nmap --help
./ft_nmap --help --ip 8.8.8.8
```

### 🔹 File 
```bash
./ft_nmap --file test/test_commands/file.txt 
./ft_nmap --file test/test_commands/file.txt --ports 80,443 --scan SYN,ACK --speedup 10
./ft_nmap --file test/test_commands/file.txt --ports 1,3-5,8,10-12 --speedup 20
./ft_nmap --file test/test_commands/file.txt --speedup 50 --scan SYN,ACK 
./ft_nmap --file test/test_commands/file.txt --scan SYN,ACK,FIN,XMAS,UDP --ports 22,80-190,443

./ft_nmap --file test/test_commands/file_2.txt --ports 22,443 --scan SYN,ACK --speedup 10
./ft_nmap --file test/test_commands/file_2.txt --ports 22,80-190,443 --scan SYN,ACK --speedup 10
./ft_nmap --file test/test_commands/file_2.txt --ports 200,80-190,443 --scan SYN,ACK,FIN,XMAS,UDP --speedup 50
```

---

## ❌ CASOS QUE DEBEN FALLAR en el **PARSEO** (invalidos)
### 🔴 IP inválida
```bash
./ft_nmap --ip 999.999.999.999 --ports 80
./ft_nmap --ip abc.def.ghi.jkl --ports 80
./ft_nmap --ip 8.8.8 --ports 80

./ft_nmap --ip 127.0.0.
./ft_nmap --ip 127.0.0
./ft_nmap --ip 127.1
./ft_nmap --ip 
./ft_nmap --ip ""

./ft_nmap --ip 8.8.8.8 --ports 22,22,80-90,443
./ft_nmap --ip 8.8.8.8 --ports 22,22,80-90,443,81,85

./ft_nmap --ip 8.8.8.8 --ports 2147483648-2147483649
```

### 🔴 Ports inválidos
```bash
./ft_nmap --ip 999.999.999.999 --ports 80
./ft_nmap --ip abc.def.ghi.jkl --ports 80
./ft_nmap --ip 8.8.8 --ports 80

./ft_nmap --ip 127.0.0.
./ft_nmap --ip 127.0.0
./ft_nmap --ip 127.1
./ft_nmap --ip 
./ft_nmap --ip ""

./ft_nmap --ip 8.8.8.8 --ports 22,22,80-90,443
./ft_nmap --ip 8.8.8.8 --ports 22,22,80-90,443,81,85

./ft_nmap --ip 8.8.8.8 --ports 2147483648-2147483649
```

### fuera de rango
```bash
./ft_nmap --ip 8.8.8.8 --ports abc
./ft_nmap --ip 8.8.8.8 --ports 80,abc
./ft_nmap --ip 8.8.8.8 --ports 80-
./ft_nmap --ip 8.8.8.8 --ports 80,http,443
./ft_nmap --ip 8.8.8.8 --ports ""
```

### rangos incorrectos
```bash
./ft_nmap --ip 8.8.8.8 --ports 80 --flagInexistente
./ft_nmap --ip 8.8.8.8 --ports 100-10     # start > end
./ft_nmap --ip 8.8.8.8 --ports 80-20
./ft_nmap --ip 8.8.8.8 --ports -100-10
./ft_nmap --ip 8.8.8.8 --ports 65530-65540
./ft_nmap --ip 8.8.8.8 --ports 1-999999   # overflow
./ft_nmap --ip 8.8.8.8 --ports -5-10      # inválido
./ft_nmap --ip 8.8.8.8 --ports 10-abc
```

### 🔴 speedup inválido
```bash
./ft_nmap --ip 8.8.8.8 --ports 80 --flagInexistente
./ft_nmap --ip 8.8.8.8 --ports 100-10     # start > end
./ft_nmap --ip 8.8.8.8 --ports 80-20
./ft_nmap --ip 8.8.8.8 --ports -100-10
./ft_nmap --ip 8.8.8.8 --ports 65530-65540
./ft_nmap --ip 8.8.8.8 --ports 1-999999   # overflow
./ft_nmap --ip 8.8.8.8 --ports -5-10      # inválido
./ft_nmap --ip 8.8.8.8 --ports 10-abc
```

### 🔴 scan inválido
```bash
./ft_nmap --ip 8.8.8.8 --speedup hola
./ft_nmap --ip 8.8.8.8 --speedup -10
./ft_nmap --ip 8.8.8.8 --speedup -1
./ft_nmap --ip 8.8.8.8 --speedup 251
./ft_nmap --ip 8.8.8.8 --speedup 999
./ft_nmap --ip 8.8.8.8 --speedup 999999999
```

### 🔴 errores estructurales
```bash
./ft_nmap --ip 8.8.8.8 --scan TCP
./ft_nmap --ip 8.8.8.8 --scan CONNECT
./ft_nmap --ip 8.8.8.8 --scan syn
./ft_nmap --ip 8.8.8.8 --scan SYN,INVALID
./ft_nmap --ip 8.8.8.8 --scan ""
```

### flags incompletas
```bash
./ft_nmap
./ft_nmap --foo
./ft_nmap --ports 80
./ft_nmap --scan SYN
./ft_nmap --speedup 10
./ft_nmap --ports 80 --scan SYN --speedup 10
./ft_nmap --file /path/to/nonexistent_file_98765.txt
```

### duplicados o conflictos (si los controlas)
```bash
./ft_nmap --ip 8.8.8.8 --file targets.txt
./ft_nmap --ip 8.8.8.8 --file test/test_commands/file.txt
```

### 🧪 TESTS MÁS IMPORTANTES
```bash
./ft_nmap --ip 8.8.8.8 --ports 1,2,3-5,8-10,100-90,abc
./ft_nmap --ip 8.8.8.8 --scan SYN,ACK,INVALID
./ft_nmap --ip 999.999.999.999
./ft_nmap --ip 8.8.8.8 --speedup 999999999
./ft_nmap --ip 8.8.8.8 --ports 80,443,abc,22-10
```

---
# Motor ecaneo
```bash
./ft_nmap --ip 8.8.8.8 --ports 80-85 --scan FIN,XMAS,NULL 				#// solo puede dar open/filtered. sin SYN no sabremos mas. 
./ft_nmap --ip 8.8.8.8 --ports 80-85 --scan FIN,XMAS,NULL,SYN			#// aqui si q deberiamos saber si es opn o filtered. 

```


---
# Bonus examples:

```bash
./ft_nmap --ip 8.8.8.8 --ports 80-85 --scan FIN,XMAS,NULL --os-detect
./ft_nmap --ip 8.8.8.8 --ports 80-85 --scan FIN,XMAS,NULL,SYN --spoof-ip 198.120.0.31
./ft_nmap --ip 8.8.8.8 --ports 80-85 --scan FIN,XMAS,NULL,SYN --badsum 
./ft_nmap --ip 8.8.8.8 --ports 80-85 --scan FIN,XMAS,NULL,SYN --packet-trace
./ft_nmap --ip 8.8.8.8 --ports 80-85 --scan FIN,XMAS,NULL,SYN --ttl 26

./ft_nmap --ip 8.8.8.8 --os-detect
./ft_nmap --ip 8.8.8.8 --badsum
./ft_nmap --ip 8.8.8.8 --packet-trace
./ft_nmap --ip 8.8.8.8 --ttl 0
./ft_nmap --ip 8.8.8.8 --ttl 64
./ft_nmap --ip 8.8.8.8 --ttl 255
./ft_nmap --ip 8.8.8.8 --spoof-ip 10.0.0.1
./ft_nmap --ip 8.8.8.8 --os-detect --spoof-ip 198.120.0.31 --badsum --packet-trace --ttl 26

# Fallos en bonus:
./ft_nmap --ip 8.8.8.8 --ttl 256
./ft_nmap --ip 8.8.8.8 --ttl -1
./ft_nmap --ip 8.8.8.8 --spoof-ip 999.999.999.999
```

