#!/usr/bin/env python3 

import sys
import os

def main():
    try:
        file_ = open(sys.argv[1], "r")
    except FileNotFoundError:
        print("\_0_0_/")
        exit(2)
    dump = len(file_.read())
    print(f"Your key is: {dump}")
    file_.close()

if len(sys.argv) != 2:
    print("\_0_0_/")
    exit(1)

if __name__ == "__main__":
    main()
