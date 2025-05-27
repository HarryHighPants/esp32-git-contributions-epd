#!/bin/bash

find src lib \( -name "*.cpp" -o -name "*.h" \) -exec clang-format -style=file -i -n --Werror {} +

if [[ $? -ne 0 ]]; then
  echo "Code style issues found. Please run './bin/clang-format-fix.sh' to fix them."
  exit 1
fi

echo "Code style check passed."
