#!/usr/bin/env python3
"""Compatibility entrypoint for the canonical fresh build; see build.py --help.
The historical manifest-composition code (historical/original-target-tools) was removed on
2026-11-17; it is at tag historical-final.
"""
from build import main

if __name__ == '__main__':
    main()
