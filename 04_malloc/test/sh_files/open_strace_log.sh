#!/bin/bash

# Get the most recent Strace log file based on name sorting (timestamp)
latest_log=$(find test/strace_outputs/strace_output_*.log 2>/dev/null | sort | tail -n 1)

# Check if the file exists
if [ -f "$latest_log" ]; then
    # Open the file with VSCode
    code "$latest_log"
else
    echo "No Strace log file found."
fi