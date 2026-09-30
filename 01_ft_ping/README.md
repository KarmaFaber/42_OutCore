# ft_ping

A systems-level re-implementation of the standard network utility based on `inetutils-2.0` specifications, written in C. This tool operates at the transport/network boundary via raw POSIX sockets (`SOCK_RAW`) to build, transmit, filter, and parse ICMP ECHO frames.

---

## 📌 Technical Scope & Subject Requirements

* **Reference standard**: Built against the behavior and output formatting of GNU `inetutils-2.0`.
* **Network & Protocols**: Raw IPv4 packet transmission (`IPPROTO_ICMP`), supporting both IP addresses and FQDN hosts (`getaddrinfo` resolution).
* **CLI flags**:

  * `-v` (*verbose*): Detailed logging of ICMP error payloads, header parsing, and malformed/dropped frames.
  * `-?` (*help*): Standard command usage and flag descriptions.
* **Integrity & Concurrency**:

  * Deterministic 1-second transmission loops controlled via POSIX signals (`SIGALRM`).
  * Graceful termination, resource deallocation, and statistical roll-up on interrupt (`SIGINT`).
  * Non-blocking or strictly timed socket reads (`SO_RCVTIMEO` / `select`) to handle timeouts.
* **Safety**: Zero memory leaks, zero crash tolerance (segfault/bus error protection on malformed incoming packets).
* No bonus features were implemented.

---

## 📐 Architecture & Core Concepts

### 1. Network Layer Interception

`ft_ping` bypasses transport protocols (TCP/UDP) by interfacing with Layer 3/Layer 4 raw sockets.

![OSI Model Layers](img/image_3.png)

* Requires administrative privileges (`CAP_NET_RAW` / `sudo`) to open `SOCK_RAW`.
* Assembles IP payloads directly and extracts network headers from incoming frames.

### 2. ICMP Frame Assembly & RTT Computation

The program constructs an `ICMP_ECHO` (Type 8, Code 0) request, calculates the RFC 1071 internet checksum, and embeds a 64-bit microsecond timestamp (`struct timeval`) within the payload.

![IP and ICMP Packet Structure](img/image.png)

* **RTT Calculation**: On receiving `ICMP_ECHOREPLY` (Type 0), transit time is computed:
  \(\Delta t = t_{\text{reception}} - t_{\text{embedded}}\)
* **Filtering & Isolation**: Validates `ICMP identifier` against `getpid() & 0xFFFF` and verifies the payload checksum to discard loopback or unassociated host traffic.
* **Statistics**: Tracks aggregate metrics across packets to compute minimum, average, maximum, and standard deviation (`mdev`).

---

## 🛠️ Build & Usage

### Prerequisites

* Linux environment (Debian-based recommended, Kernel > 3.14).
* Standard C toolchain (`gcc` or `clang`, `make`).
* sudo permissions

### Compilation

```bash
make
```

### Clean

```bash
make fclean
```

### Execution

Raw sockets require elevated network capabilities:

```bash
# Standard target ping
sudo ./ft_ping google.com

# Verbose mode
sudo ./ft_ping -v 1.1.1.1

# Help menu
./ft_ping -?
```

### Expected result

```bash
$ make
gcc -Wall -Wextra -Werror -c ft_ping.c -o ft_ping.o
gcc -Wall -Wextra -Werror -c utils.c -o utils.o
gcc -Wall -Wextra -Werror -c parser.c -o parser.o
gcc -Wall -Wextra -Werror -c send.c -o send.o
gcc -Wall -Wextra -Werror -c receive.c -o receive.o
gcc -Wall -Wextra -Werror -c printers.c -o printers.o
gcc -Wall -Wextra -Werror -o ft_ping ft_ping.o utils.o parser.o send.o receive.o printers.o -lm
ft_ping created.

$ sudo ./ft_ping google.com
FT_PING google.com (142.251.168.113) 56(84) bytes of data.
64 bytes from 142.251.168.113: icmp_seq=0 ttl=105 time=27.07 ms
64 bytes from 142.251.168.113: icmp_seq=1 ttl=105 time=27.42 ms
64 bytes from 142.251.168.113: icmp_seq=2 ttl=105 time=27.46 ms
64 bytes from 142.251.168.113: icmp_seq=3 ttl=105 time=27.39 ms
^C
--- google.com ft_ping statistics ---
13 packets transmitted, 13 received, 0% packet loss, time 12748ms
rtt min/avg/max/mdev = 26.52/27.47/29.68/0.77 ms

$ sudo ./ft_ping -v 1.1.1.1
FT_PING 1.1.1.1 (1.1.1.1) 56(84) bytes of data.
64 bytes from 1.1.1.1: icmp_seq=0 ttl=56 time=9.18 ms
64 bytes from 1.1.1.1: icmp_seq=1 ttl=56 time=8.82 ms
64 bytes from 1.1.1.1: icmp_seq=2 ttl=56 time=9.89 ms
64 bytes from 1.1.1.1: icmp_seq=3 ttl=56 time=8.74 ms
^C
--- 1.1.1.1 ft_ping statistics ---
4 packets transmitted, 4 received, 0% packet loss, time 3828ms
rtt min/avg/max/mdev = 8.74/9.16/9.89/0.45 ms

$ ./ft_ping -?
Usage:
  sudo ./ft_ping [options] <destination>

Options:
  <destination>      dns name or ip address
  -v                 verbose output
  -?                 show this help
  -h                 show this help
```

## 🧪 Verification & Defensive Engineering

**Error Injection**: Validated against hosts dropping packets and TTL expiry (Time-to-Live exceeded ICMP Type 11).

**Memory & Stability**: Verified using valgrind --leak-check=full to ensure complete cleanup on abrupt SIGINT interruption.
