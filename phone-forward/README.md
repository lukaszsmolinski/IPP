# phone-forward

This is my solution to an assignment for Individual Programming Project course at the University of Warsaw (MIMUW).

This assignment was divided into three segments, and each segment introduced new requirements or modified existing ones. The code was fully documented using Doxygen. Various data structures, such as trie and list, were implemented and utilized.

### Description

It is a library that allows performing various operations on phone numbers, which are non-empty strings (of arbitrary length) composed of digits '0' to '9' and characters '*', '#' representing the tenth and eleventh digits.
The library's key features include:
- Redirection of all numbers starting with a given prefix.
- Deletion of redirections.
- Finding the redirection of a given phone number.
- Finding all numbers redirected to a given phone number.

### Usage
```
mkdir build
cd build
cmake ..
make
```
