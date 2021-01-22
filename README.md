# Cache simulation
## Description
- Language: C++
- Test cases:
    - 1st input: cache#.org
    - 2nd input: reference#.lst
    - output: record.rpt
## Introduction
- Indexed policy: LSB
- Replacement policy: NRU

This program is to simulate cache behavior. The first input files gives a cache with M-bit address bits, B-byte block size, E cache sets, and using
A-way set associativity. The second input file assigns a series of block address. This program will count how many times it misses and write the record into a new file.


