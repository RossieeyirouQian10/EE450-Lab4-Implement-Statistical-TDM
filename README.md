# EE450-Lab4-Implement-Statistical-TDM
Implement Statistical TDM in C++.

Your compiled C/C++ program should be able to read the
sources described in an input file. The file name should be
an input parameter of your complied program. Suppose your
compiled C/C++ program is STDM. A command of running
your program can be: “./STDM input.txt” where “input.txt” is
the name of the input file.

Input:

SourceA:0 1 A1,1 2 A2,2 3 A3,5 6 A4
SourceB:0 1 B1,1 2 B2
SourceC:1 2 C1,2 3 C2,3 4 C3,5 6 C4,6 7 C5
SourceD:4 5 D1,5 6 D2,8 9 D3


Name: Yirou Qian
USC ID: 4354716429

Compilation Steps: 
             
                   a.  g++ STDM.cpp -o STDM

                   b.  ./STDM input.txt



Results: 

SF
0,1 1.5 A1
1,1.5 2 B1
EF
SF
0,2 2.5 A2
1,2.5 3 B2
EF
SF
2,3 3.5 C1
0,3.5 4 A3
EF
SF
2,4 4.5 C2
2,4.5 5 C3
EF
SF
3,5 5.5 D1
EF
SF
0,6 6.5 A4
2,6.5 7 C4
EF
SF
3,7 7.5 D2
2,7.5 8 C5
EF
SF
EF
SF
3,9 9.5 D3
EF
