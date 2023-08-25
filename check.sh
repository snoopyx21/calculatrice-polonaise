#!/bin/bash

if ! test -x ./pc; then
    echo "Error: no executable \"./pc\"" 1>&2
    exit 1
fi

if diff results.txt <(./pc<tests.txt 2>/dev/null); then
    printf "\033[32mSUCCESS\033[0m\n"
    exit 1
else
    printf "\n\033[31mFAIL\033[0m\n"
    exit 1
fi
