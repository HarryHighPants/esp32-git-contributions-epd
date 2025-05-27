#!/bin/bash

find src lib \( -name "*.cpp" -o -name "*.h" \) -exec clang-format -style=file -i {} +
