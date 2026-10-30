#!/usr/bin/env bash

BIN="./ft_nmap"
TMP_FILE="/tmp/ft_nmap_test_hosts.txt"
TMP_FILE_EMPTY="/tmp/ft_nmap_test_empty.txt"
PASSED=0
FAILED=0

# ANSI color codes
GREEN="\033[1;32m"
RED="\033[1;31m"
YELLOW="\033[1;33m"
RESET="\033[0m"

# Setup temporary test target files
echo "127.0.0.1" > "$TMP_FILE"
echo "google.com" >> "$TMP_FILE"
touch "$TMP_FILE_EMPTY"

cleanup() {
    rm -f "$TMP_FILE" "$TMP_FILE_EMPTY"
}
trap cleanup EXIT

# Test runner function
# Usage: run_test "Test Description" <expected_exit_code: 0 or 1> <args...>
run_test() {
    local desc="$1"
    local expected_code="$2"
    shift 2

    # Run suppressing stdout/stderr
    "$BIN" "$@" > /dev/null 2>&1
    local actual_code=$?

    local success=0
    if [ "$expected_code" -eq 0 ] && [ "$actual_code" -eq 0 ]; then
        success=1
    elif [ "$expected_code" -ne 0 ] && [ "$actual_code" -ne 0 ]; then
        success=1
    fi

    if [ "$success" -eq 1 ]; then
        printf "  [%bPASS%b] %s\n" "$GREEN" "$RESET" "$desc"
        printf "         Cmd: %s %s\n" "$BIN" "$*"
        ((PASSED++))
    else
        printf "  [%bFAIL%b] %s (Expected exit %d, got %d)\n" "$RED" "$RESET" "$desc" "$expected_code" "$actual_code"
        printf "         Cmd: %s %s\n" "$BIN" "$*"
        ((FAILED++))
    fi
}

section() {
    printf "\n%b=== %s ===%b\n" "$YELLOW" "$1" "$RESET"
}

if [ ! -x "$BIN" ]; then
    printf "%bError: Binary '%s' not found or not executable.%b\n" "$RED" "$BIN" "$RESET"
    exit 1
fi

# ==============================================================================
# 1. Structural & Mandatory Arguments (--ip / --file / --help)
# ==============================================================================
section "1. Structural & Mandatory Arguments"
run_test "No arguments provided (must fail)" 1
run_test "Unknown standalone flag" 1 --foo
run_test "Help option alone" 0 --help
run_test "Help option with additional flags" 0 --help --ip 8.8.8.8
run_test "Valid single IPv4 target" 0 --ip 8.8.8.8
run_test "Valid domain name target" 1 --ip google.com
run_test "Valid target file" 0 --file "$TMP_FILE"
run_test "Mutual exclusion: --ip and --file simultaneously" 1 --ip 8.8.8.8 --file "$TMP_FILE"
run_test "Missing argument for --ip" 1 --ip
run_test "Missing argument for --file" 1 --file
run_test "File target does not exist" 1 --file "/path/to/nonexistent_file_98765.txt"

# ==============================================================================
# 2. Valid Port Configurations
# ==============================================================================
section "2. Valid Port Configurations"
run_test "Single port (HTTP)" 0 --ip 8.8.8.8 --ports 80
run_test "Single port (HTTPS)" 0 --ip 1.1.1.1 --ports 443
run_test "Comma-separated port list" 0 --ip 8.8.8.8 --ports 80,443,8080
run_test "Standard port range (1-1024)" 0 --ip 8.8.8.8 --ports 1-1024
run_test "Restricted port range (80-90)" 0 --ip 10.0.0.1 --ports 80-90
run_test "Mixed list and range (22,80-90,443)" 0 --ip 8.8.8.8 --ports 22,80-90,443
run_test "Multiple discrete ranges (1,3-5,8,10-12)" 0 --ip 127.0.0.1 --ports 1,3-5,8,10-12
run_test "Boundary port (65535)" 0 --ip 127.0.0.1 --ports 65535
run_test "Tolerate trailing comma in ports list" 0 --ip 127.0.0.1 --ports 80,

# ==============================================================================
# 3. Invalid Port Configurations (Syntax & Limits)
# ==============================================================================
section "3. Invalid Port Configurations"
run_test "Alphabetic port token" 1 --ip 8.8.8.8 --ports abc
run_test "Mixed valid and alphabetic tokens" 1 --ip 8.8.8.8 --ports 80,abc
run_test "Out of range port (>65535)" 1 --ip 8.8.8.8 --ports 99999
run_test "Negative port number" 1 --ip 8.8.8.8 --ports -1
run_test "Port 0 (reserved/invalid)" 1 --ip 8.8.8.8 --ports 0
run_test "Inverted port range (start > end)" 1 --ip 8.8.8.8 --ports 100-10
run_test "Inverted port range (80-20)" 1 --ip 8.8.8.8 --ports 80-20
run_test "Negative range boundaries (-100-10)" 1 --ip 8.8.8.8 --ports -100-10
run_test "Negative start range (-5-10)" 1 --ip 8.8.8.8 --ports -5-10
run_test "Range exceeding port maximum (65530-65540)" 1 --ip 8.8.8.8 --ports 65530-65540
run_test "Range integer overflow (1-999999)" 1 --ip 8.8.8.8 --ports 1-999999
run_test "Alphanumeric range boundary (10-abc)" 1 --ip 8.8.8.8 --ports 10-abc
run_test "Trailing hyphen range syntax (80-)" 1 --ip 8.8.8.8 --ports 80-
run_test "Total ports exceed buffer limit (>1024 ports)" 1 --ip 8.8.8.8 --ports 1-1025
run_test "32-bit unsigned overflow values" 1 --ip 8.8.8.8 --ports 2147483648-2147483649
run_test "Compound invalid port syntax (1,2,3-5,8-10,100-90,abc)" 1 --ip 8.8.8.8 --ports 1,2,3-5,8-10,100-90,abc
run_test "Missing argument for --ports" 1 --ip 8.8.8.8 --ports

# ==============================================================================
# 4. Target IP Validation
# ==============================================================================
section "4. Target IP Validation"
run_test "Octet out of bounds (999.999.999.999)" 1 --ip 999.999.999.999
run_test "Invalid host string (abc.def.ghi.jkl)" 1 --ip abc.def.ghi.jkl
run_test "Incomplete IPv4 address (8.8.8)" 1 --ip 8.8.8
run_test "Trailing dot in IPv4 (127.0.0.)" 1 --ip 127.0.0.
run_test "Truncated IPv4 octets (127.0.0)" 1 --ip 127.0.0
run_test "Short notation IPv4 (127.1)" 1 --ip 127.1
run_test "Empty string as IP target" 1 --ip ""

# ==============================================================================
# 5. Scan Types (--scan)
# ==============================================================================
section "5. Scan Types"
run_test "Single SYN scan" 0 --ip 8.8.8.8 --scan SYN
run_test "Single ACK scan" 0 --ip 8.8.8.8 --scan ACK
run_test "Single NULL scan" 0 --ip 8.8.8.8 --scan NULL
run_test "Single FIN scan" 0 --ip 8.8.8.8 --scan FIN
run_test "Single XMAS scan" 0 --ip 8.8.8.8 --scan XMAS
run_test "Single UDP scan" 0 --ip 8.8.8.8 --scan UDP
run_test "Multiple scans (SYN,ACK)" 0 --ip 8.8.8.8 --scan SYN,ACK
run_test "Multiple scans (SYN,NULL,FIN)" 0 --ip 8.8.8.8 --scan SYN,NULL,FIN
run_test "Full 6-scan combo" 0 --ip 8.8.8.8 --scan SYN,ACK,FIN,XMAS,NULL,UDP
run_test "Tolerate trailing comma in scan types" 0 --ip 8.8.8.8 --scan SYN,
run_test "Unsupported scan type (TCP)" 1 --ip 8.8.8.8 --scan TCP
run_test "Unsupported scan type (CONNECT)" 1 --ip 8.8.8.8 --scan CONNECT
run_test "Lowercase scan type (syn)" 1 --ip 8.8.8.8 --scan syn
run_test "Mixed valid and invalid scans (SYN,INVALID)" 1 --ip 8.8.8.8 --scan SYN,INVALID
run_test "Empty string as scan type" 0 --ip 8.8.8.8 --scan ""
run_test "Missing argument for --scan" 1 --ip 8.8.8.8 --scan

# ==============================================================================
# 6. Concurrency & Speedup (--speedup)
# ==============================================================================
section "6. Concurrency & Speedup"
run_test "Speedup 0 (sequential execution)" 0 --ip 8.8.8.8 --speedup 0
run_test "Speedup nominal value (10)" 0 --ip 8.8.8.8 --speedup 10
run_test "Speedup nominal value (50)" 0 --ip 8.8.8.8 --speedup 50
run_test "Speedup upper boundary (250)" 0 --ip 8.8.8.8 --speedup 250
run_test "Speedup exceeds boundary (>250)" 1 --ip 8.8.8.8 --speedup 251
run_test "Speedup large overflow value (999)" 1 --ip 8.8.8.8 --speedup 999
run_test "Speedup extreme overflow (999999999)" 1 --ip 8.8.8.8 --speedup 999999999
run_test "Speedup negative value (-10)" 1 --ip 8.8.8.8 --speedup -10
run_test "Speedup non-numeric token (hola)" 1 --ip 8.8.8.8 --speedup hola
run_test "Missing argument for --speedup" 1 --ip 8.8.8.8 --speedup

# ==============================================================================
# 7. Bonus Options (--os-detect, --spoof-ip, --badsum, --packet-trace, --ttl)
# ==============================================================================
section "7. Bonus Options"
run_test "OS detection flag alone" 0 --ip 8.8.8.8 --os-detect
run_test "Bad checksum flag alone" 0 --ip 8.8.8.8 --badsum
run_test "Packet trace flag alone" 0 --ip 8.8.8.8 --packet-trace
run_test "Valid custom TTL minimum boundary (0)" 0 --ip 8.8.8.8 --ttl 0
run_test "Valid custom TTL nominal (64)" 0 --ip 8.8.8.8 --ttl 64
run_test "Valid custom TTL maximum boundary (255)" 0 --ip 8.8.8.8 --ttl 255
run_test "TTL exceeds upper limit (>255)" 1 --ip 8.8.8.8 --ttl 256
run_test "TTL negative value" 1 --ip 8.8.8.8 --ttl -1
run_test "Missing argument for --ttl" 1 --ip 8.8.8.8 --ttl
run_test "Valid spoofed IPv4 address" 0 --ip 8.8.8.8 --spoof-ip 198.120.0.31
run_test "Invalid spoofed IPv4 address" 1 --ip 8.8.8.8 --spoof-ip 999.999.999.999
run_test "Missing argument for --spoof-ip" 1 --ip 8.8.8.8 --spoof-ip
run_test "All bonus flags combined" 0 --ip 8.8.8.8 --os-detect --spoof-ip 198.120.0.31 --badsum --packet-trace --ttl 64

# ==============================================================================
# 8. Integrated Combinations & Complex Invocations
# ==============================================================================
section "8. Integrated Combinations & Complex Invocations"
run_test "Full standard combo: IP + Ports + Scan + Speedup" 0 \
    --ip 8.8.8.8 --ports 80,443 --scan SYN,ACK --speedup 10
run_test "Complex port list and scan combination" 0 \
    --ip 1.1.1.1 --ports 22,80-85,443 --scan SYN --speedup 50
run_test "Target file combined with complex options" 0 \
    --file "$TMP_FILE" --ports 200,80-190,443 --scan SYN,ACK,FIN,XMAS,UDP --speedup 50
run_test "Missing target completely with optional flags" 1 \
    --ports 80 --scan SYN --speedup 10
run_test "File use" 0 \
    --file test/test_commands/file_2.txt --ports 200,80-190,443 --scan SYN,ACK,FIN,XMAS,UDP --speedup 50

# ==============================================================================
# Final Summary
# ==============================================================================
TOTAL=$((PASSED + FAILED))
printf "\n%b================ TEST SUMMARY ===============%b\n" "$YELLOW" "$RESET"
printf "Total tests executed : %d\n" "$TOTAL"
printf "Passed               : %b%d%b\n" "$GREEN" "$PASSED" "$RESET"
printf "Failed               : %b%d%b\n" "$RED" "$FAILED" "$RESET"
printf "%b==============================================%b\n\n" "$YELLOW" "$RESET"

if [ "$FAILED" -ne 0 ]; then
    exit 1
fi
exit 0