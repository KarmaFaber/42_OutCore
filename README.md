# 42_OutCore

A personal collection of low-level systems programming, networking, and protocol engineering projects in C, built around project specifications from the 42 Madrid curriculum and adapted for independent portfolio and systems experimentation.

Implementations focus on POSIX interfaces, modular C architecture, explicit error and resource handling, and verification with system diagnostics (`valgrind`, `strace`, and project test suites).

---

## Repository Index

| Project | Domain | Core Concepts & Technologies | Estimated work time |
| :--- | :--- | :--- | :--- |
| **[ft_ping](./01_ft_ping)** | Low-Level Networking | Raw Sockets (`SOCK_RAW`), ICMP protocol, RFC 1071 checksum calculation, POSIX signals (`SIGALRM`), RTT metric computation (min/avg/max/mdev). GNU `inetutils-2.0` architecture. | ~49 hours |
| **[ft_traceroute](./02_ft_traceroute)** | Network Diagnostics | Hop-by-hop route tracing, IP header TTL manipulation (`IP_TTL`), ICMP Time Exceeded parsing, socket receive timeouts (`SO_RCVTIMEO`), DNS resolution (`getnameinfo`). | ~49 hours |
| **[ft_nmap](./03_ft_nmap)** | Network Reconnaissance & Concurrency | Multi-threaded port scanner, packet injection and frame capture with `libpcap`, worker pool concurrency (`pthreads`, up to 250 threads), advanced scan techniques (SYN, NULL, FIN, XMAS, ACK, UDP), port-to-service mapping. | ~120 hours *(In progress)* |
| **[malloc](./04_malloc)** | Memory Management & Systems | Custom dynamic allocator (`malloc`, `free`) built as a shared library (`libft_malloc.so` / `LD_PRELOAD`), `TINY`/`SMALL`/`LARGE` zones via `mmap`/`munmap`, 16-byte alignment, POSIX mutex synchronization, `show_alloc_mem` / `show_alloc_mem_ex`. | ~49 hours |

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

### 3. `ft_nmap` — High-Throughput Concurrent Port Scanner
- **Core Standard**: RFC 793 (Transmission Control Protocol), RFC 768 (User Datagram Protocol).
- **Architecture**:
  - **Packet Crafting & Sniffing**: Manual TCP/UDP header construction and low-level packet capture via `libpcap` with optimized Berkeley Packet Filters (BPF).
  - **Scan Vector Support**: Full implementation of RFC-compliant scan modes: SYN (half-open stealth), NULL (no flags), FIN, XMAS (FIN/PSH/URG), ACK (firewall rule mapping), and raw UDP.
  - **Concurrency Engine**: Multi-threaded architecture scaling up to 250 parallel workers (`pthreads`) synchronized via mutexes to minimize total scan elapsed time across target port ranges (1–1024 default).
  - **Service Resolution**: Internal lookup table resolving standard port assignments (`/etc/services` mapping) to identify host daemons without payload inspection.

### 4. `malloc` — Custom Dynamic Memory Allocator & Shared Library
- **Core Interfaces**: `malloc`, `free`, `show_alloc_mem`, and `show_alloc_mem_ex` exported in `libft_malloc_$HOSTTYPE.so` (symlinked as `libft_malloc.so` for `LD_PRELOAD` and test execution).
- **Architecture**:
  - **Zone Classes & Virtual Memory**: `TINY` and `SMALL` pre-allocated zones sized to multiples of the system page size (`sysconf(_SC_PAGESIZE)`) for at least 64 allocations each, alongside direct `mmap()`/`munmap()` mappings for `LARGE` blocks.
  - **Block & Zone Metadata**: Zone and block structures with relative offsets, block splitting, free-space tracking (`free_size`), and 16-byte payload alignment.
  - **Thread Synchronization**: POSIX mutex (`pthread_mutex_t`) protecting allocator metadata and heap snapshot capture for `show_alloc_mem` and `show_alloc_mem_ex`.
  - **Verification & Scope**: Tested with custom unit, boundary, splitting, and concurrency checks alongside `valgrind` and `strace` (`mmap`/`munmap`); kept here as an independent baseline while stress-testing with `philo` and cross-platform portability experiments live in a dedicated repository.

---

## Engineering Standards & Verification

Projects in this repository follow practical systems-engineering and C verification habits:

- **Language & Toolchain**: Written in C and compiled with strict warning flags (`-Wall -Wextra -Werror`).
- **Memory & Syscall Sanitation**: Audited with `valgrind` and `strace` (`mmap`/`munmap`), plus AddressSanitizer (`-fsanitize=address`) where applicable.
- **POSIX & Kernel Interfaces**: Direct use of POSIX and kernel interfaces (`socket`, `setsockopt`, `select`, `poll`, `signal`, `getaddrinfo`, `mmap`, `munmap`, `pthread`) rather than high-level application frameworks.
- **Build System**: Modular `Makefile` per project providing standard targets (`all`, `clean`, `fclean`, `re`) alongside project-specific test targets.

---

## Code Quality & Style Guidelines

Source code in this repository is structured around 42-style modularity and defensive C practices:

- **Function Size & Modularity**: Functions are kept short and focused (targeting up to 25 lines per function under 42 Norminette conventions) with clear helper decomposition.
- **Controlled Global State**: Unnecessary globals are avoided; where global state is required (such as the allocator zone list and mutex in `malloc`), access is centralized and synchronized.
- **Resource & Error Discipline**: Null checks, size/overflow guards, and explicit resource cleanup (`free`, `munmap`, descriptor/socket cleanup) are handled consistently across paths.
- **Style & Compiler Checks**: Checked against 42 style rules (`norminette`) and built with `-Wall -Wextra -Werror`.

---

## AI Usage Disclosure

To keep authorship and workflow transparency clear across the repository, here is how AI tools were used:

- **Gemini:** Used as a conceptual and systems mentor to guide design decisions, architecture, and debugging workflows without providing implementation code.
- **ChatGPT:** Used strictly for drafting and formatting function documentation.
- **GitHub Copilot:** Used for test structure, formatting, and test harness layout.
- **Kimi AI/Claude AI:** Not used.