#!/usr/bin/env bash

set -eu

prefix="${1:-logprobe}"
interval="${2:-1}"
counter=0

while true; do
    printf '%s tick=%d pid=%d time=%(%Y-%m-%dT%H:%M:%S%z)T\n' \
        "$prefix" "$counter" "$$" -1
    counter=$((counter + 1))
    sleep "$interval"
done
