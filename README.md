# Cache simulation
## Description
- Language: C++
- Test cases:
    - 1st input: cache#.org
    - 2nd input: reference#.lst
## Introduction
- Indexed policy: LSB
- Replacement policy: NRU

This program is to simulate cache behavior. The first input files gives a cache with M-bit address bits, B-byte block size, E cache sets, and using
A-way set associativity. The second input file assigns a series of referenced block address. Finally output the record of hit/miss into another file.

## Compile & Execute
- (on Windows) <br>
    g++ main.cpp -o main.exe <br>
    main.exe cache#.org reference.lst outputFile.rpt
    
- (on Linux) <br>
    g++ main.cpp -o main <br>
    main.exe cache#.org reference.lst outputFile
