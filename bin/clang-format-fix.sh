#!/bin/bash

find src \( -name "*.cpp" -o -name "*.h" \) -exec clang-format -style=file -i {} +
