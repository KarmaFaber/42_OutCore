
# parser tests:
## aceptas

### 🔹 IP + ports básicos
```bash
./ft_nmap --ip 8.8.8.8 --ports 80
./ft_nmap --ip 1.1.1.1 --ports 443
```

### 🔹 Lista de puertos
```bash
./ft_nmap --ip 8.8.8.8 --ports 80,443,8080
./ft_nmap --ip 192.168.1.1 --ports 22,80,443,53
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
./ft_nmap --ip 8.8.8.8 --scan SYN,NULL,FIN
./ft_nmap --ip 8.8.8.8 --scan SYN,ACK,FIN,XMAS,UDP
```

### 🔹 Full combo (stress parser)
```bash
./ft_nmap --ip 8.8.8.8 --ports 80,443 --scan SYN,ACK --speedup 10
./ft_nmap --ip 1.1.1.1 --ports 22,80-85,443 --scan SYN --speedup 50
```

### 🔹 Defaults esperados
```bash
./ft_nmap --ip 8.8.8.8
```

### 🔹 File 
```bash
./ft_nmap --file test/test_commands/file.txt 
./ft_nmap --file test/test_commands/file.txt --ports 80,443 --scan SYN,ACK --speedup 10
./ft_nmap --file test/test_commands/file.txt --ports 1,3-5,8,10-12 --speedup 20
./ft_nmap --file test/test_commands/file.txt --speedup 50 --scan SYN,ACK 
./ft_nmap --file test/test_commands/file.txt --scan SYN,ACK,FIN,XMAS,UDP --ports 22,80-90,443
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



./ft_nmap --ip 8.8.8.8 --ports 22,22,80-90,443
./ft_nmap --ip 8.8.8.8 --ports 22,22,80-90,443,81,85


./ft_nmap --ip 8.8.8.8 --ports 2147483648-2147483649
```

### 🔴 Ports inválidos
```bash
./ft_nmap --ip 8.8.8.8 --ports abc
./ft_nmap --ip 8.8.8.8 --ports 80,abc
```

### fuera de rango
```bash
./ft_nmap --ip 8.8.8.8 --ports 99999
./ft_nmap --ip 8.8.8.8 --ports -1

#min/max int y long:


```

### rangos incorrectos
```bash
./ft_nmap --ip 8.8.8.8 --ports 80 --flagInexistente
./ft_nmap --ip 8.8.8.8 --ports 100-10     # start > end
./ft_nmap --ip 8.8.8.8 --ports -100-10
./ft_nmap --ip 8.8.8.8 --ports 65530-65540
./ft_nmap --ip 8.8.8.8 --ports 1-999999   # overflow
./ft_nmap --ip 8.8.8.8 --ports -5-10      # inválido
./ft_nmap --ip 8.8.8.8 --ports 10-abc
```

### 🔴 speedup inválido
```bash
./ft_nmap --ip 8.8.8.8 --speedup hola
./ft_nmap --ip 8.8.8.8 --speedup -10
./ft_nmap --ip 8.8.8.8 --speedup 999
```

### 🔴 scan inválido
```bash
./ft_nmap --ip 8.8.8.8 --scan TCP
./ft_nmap --ip 8.8.8.8 --scan SYN,INVALID
./ft_nmap --ip 8.8.8.8 --scan ""
```

### 🔴 errores estructurales
```bash
./ft_nmap --ports 80
./ft_nmap --scan SYN
./ft_nmap --speedup 10
```

### flags incompletas
```bash
./ft_nmap --ip
./ft_nmap --ports
./ft_nmap --scan
./ft_nmap --speedup
```

### duplicados o conflictos (si los controlas)
```bash
./ft_nmap --ip 8.8.8.8 --file targets.txt
```

### 🧪 TESTS MÁS IMPORTANTES
```bash
--ports 1,2,3-5,8-10,100-90,abc
--scan SYN,ACK,INVALID
--ip 999.999.999.999
--speedup 999999999
--ports 80,443,abc,22-10
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


./ft_nmap --ip 8.8.8.8 --os-detect --spoof-ip 198.120.0.31 --badsum --packet-trace --ttl 26

```

