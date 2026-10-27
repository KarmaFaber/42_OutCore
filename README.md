# 42_OutCore

A structured collection of advanced systems programming, low-level networking, and protocol engineering projects in C, developed as part of the advanced curriculum (Out Core) at 42 Madrid.

All implementations strictly adhere to POSIX standards, strict zero-leak memory policies (`valgrind`), robust error handling, and modular architectural design without reliance on high-level application frameworks.

---

## Repository Index

| Project | Domain | Core Concepts & Technologies | Estimated work time |
| :--- | :--- | :--- | :--- |
| **[ft_ping](./01_ft_ping)** | Low-Level Networking | Raw Sockets (`SOCK_RAW`), ICMP protocol, RFC 1071 checksum calculation, POSIX signals (`SIGALRM`), RTT metric computation (min/avg/max/mdev). GNU `inetutils-2.0` architecture. | ~49 hours |
| **[ft_traceroute](./02_ft_traceroute)** | Network Diagnostics | Hop-by-hop route tracing, IP header TTL manipulation (`IP_TTL`), ICMP Time Exceeded parsing, socket receive timeouts (`SO_RCVTIMEO`), DNS resolution (`getnameinfo`). | ~49 hours |
| **[ft_nmap](./03_ft_nmap)** | Network Reconnaissance & Concurrency | Multi-threaded port scanner[cite: 3, 5, 7], packet injection and frame capture with `libpcap`[cite: 5], worker pool concurrency (`pthreads`, up to 250 threads)[cite: 5, 7], advanced scan techniques (SYN, NULL, FIN, XMAS, ACK, UDP)[cite: 7], port-to-service mapping[cite: 7]. | ~120 hours *(In progress)* |

---

## Technical Deep-Dive

### 1. `ft_ping` — ICMP Network Latency Diagnostic
- **Core Standard**: RFC 792 (ICMP), RFC 1071 (Internet Checksum).
- **Architecture**:
  - Direct kernel interaction via `SOCK_RAW` sockets requiring elevated capabilities (`CAP_NET_RAW`).
  - Bit-level assembly of `icmphdr` structs and software-computed 16-bit one's complement checksums.
  - POSIX signal handling (`SIGALRM`, `SIGINT`) for sub-millisecond round-trip time calculations ($min$, $avg$, $max$, $mdev$).
  - Clean separation of transmission loops, listener poll routines, and summary reporting.

### 2. `ft_traceroute` — Layer 3 Hop Discovery
- **Core Standard**: RFC 792, IPv4 Specifications (RFC 791).
- **Architecture**:
  - Incremental `IP_TTL` modification per probe batch (3 probes per hop) to trigger intermediate router expiration.
  - Dual-layer socket handling: UDP datagram dispatch on incrementing destination ports paired with raw ICMP capture for `ICMP_TIME_EXCEEDED` and `ICMP_PORT_UNREACH`.
  - Non-blocking I/O timeouts using `SO_RCVTIMEO` to prevent deadlocks on unresponsive hops.
  - Reverse lookup caching using `getnameinfo` with fallback to dotted-decimal IPv4 formatting.

### 3. `ft_nmap` — High-Throughput Concurrent Port Scanner[cite: 3, 5, 7]
- **Core Standard**: RFC 793 (Transmission Control Protocol), RFC 768 (User Datagram Protocol).
- **Architecture**:
  - **Packet Crafting & Sniffing**: Manual TCP/UDP header construction and low-level packet capture via `libpcap` with optimized Berkeley Packet Filters (BPF)[cite: 5, 7].
  - **Scan Vector Support**: Full implementation of RFC-compliant scan modes: SYN (half-open stealth), NULL (no flags), FIN, XMAS (FIN/PSH/URG), ACK (firewall rule mapping), and raw UDP[cite: 7].
  - **Concurrency Engine**: Multi-threaded architecture scaling up to 250 parallel workers (`pthreads`) synchronized via mutexes to minimize total scan elapsed time across target port ranges (1–1024 default)[cite: 5, 7].
  - **Service Resolution**: Internal lookup table resolving standard port assignments (`/etc/services` mapping) to identify host daemons without payload inspection[cite: 7].

---

## Engineering Standards & Verification

Every project in this repository meets strict systems-engineering constraints:

- **Language & Toolchain**: Written in C (compiled with `gcc`/`clang` using `-Wall -Wextra -Werror`).
- **Memory & Resource Sanitation**: Fully verified against memory leaks, double-frees, and invalid memory access using `valgrind --leak-check=full --track-origins=yes` and AddressSanitizer (`-fsanitize=address`).
- **POSIX & Kernel Interfaces**: Direct system call interfaces (`socket`, `setsockopt`, `select`, `poll`, `signal`, `getaddrinfo`) bypassing unvetted third-party wrappers.
- **Build System**: Modular, deterministic `Makefile` per project with required targets (`all`, `clean`, `fclean`, `re`).

---

## Code Quality & Static Analysis Compliance

All C source code strictly adheres to an uncompromising, safety-oriented coding standard inspired by the **Linux Kernel Coding Style** and defensive systems programming guidelines:

- **Modularity & Complexity Limits**: Functions are capped at 25 lines with strict cyclomatic complexity constraints (maximum of 4 parameters and 5 local variables per function).
- **Predictable Control Flow**: Prohibition of global variables, goto statements, and uncontrolled side effects to ensure predictable memory states.
- **Deterministic Resource Management**: Every allocation is audited for immediate null-pointer validation and symmetric deallocation (zero dangling pointers / zero memory leaks).
- **Automated Style & Linter Enforcement**: Codebase validated against static style conformance linters prior to compilation with strict warning flags (`-Wall -Wextra -Werror`).