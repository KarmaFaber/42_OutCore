# 42_OutCore

A structured collection of advanced systems programming, networking, and low-level algorithms in C, developed as part of the advanced curriculum (Out Core) at 42 Madrid.

All implementations strictly adhere to POSIX standards, zero-leak memory policies (`valgrind`), robust error handling, and modular architectural design without reliance on external frameworks.

---

## 📂 Repository Index

| Project | Domain | Core Concepts & Technologies |
| :--- | :--- | :--- |
| **[ft_ping](./01_ft_ping)** | Low-Level Networking | Raw Sockets (`SOCK_RAW`), ICMP protocol, RFC 1071 checksums, POSIX signals (`SIGALRM`), RTT statistics ($min/avg/max/mdev$). Based on GNU `inetutils-2.0`. |
| **[ft_traceroute](./02_ft_traceroute)** | Network Diagnostics | Hop-by-hop route discovery, TTL manipulation (`IP_TTL`), ICMP error handling (TTL Exceeded), blocking I/O timeouts (`SO_RCVTIMEO`). |

*(Additional advanced systems and infrastructure projects will be indexed here as they are integrated).*

---

## 🛠️ Engineering Standards & Verification

Every subproject in this repository meets strict systems-engineering constraints:

* **Language & Toolchain**: Written in C (compiled with `gcc`/`clang` using `-Wall -Wextra -Werror` flags).
* **Memory & Resource Sanitation**: Fully verified against memory leaks and invalid accesses via `valgrind` and AddressSanitizer (`-fsanitize=address`).
* **POSIX & Kernel Primitives**: Direct system call interfaces (`socket`, `setsockopt`, `select`, `signal`, `getaddrinfo`) bypassing high-level wrappers.
* **Build System**: Clean, deterministic `Makefile` builds per project with standard rules (`all`, `clean`, `fclean`, `re`).

