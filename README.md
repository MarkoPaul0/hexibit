# hexibit

![Author](https://img.shields.io/badge/author-MarkoPaul0-red.svg?style=flat-square)
[![License: GPL v3](https://img.shields.io/badge/License-GPL%20v3-blue.svg?style=flat-square)](https://www.gnu.org/licenses/gpl-3.0.en.html)
![GitHub last commit](https://img.shields.io/github/last-commit/MarkoPaul0/hexibit.svg?style=flat-square&maxAge=300)
[![Build Status](https://travis-ci.com/MarkoPaul0/hexibit.svg?token=1Gt7pCYg2Usf4VX2axE1&branch=master)](https://travis-ci.com/MarkoPaul0/hexibit)
![Stars](https://img.shields.io/github/stars/MarkoPaul0/hexibit.svg?style=social)


Command line tool to facilitate the analysis of hexadecimal data.
Here are some examples:

### Reading from an hexadecimal string

```bash
markopaulo@server$: hexibit -s  "68656c6c6f20776f726c6401FF" -i char_array_11,uint16

Data                             Interpretation       Value
-----------------------------------------------------------
68656C6C6F20776F726C64           CHAR_ARRAY           hello world
01FF                             UINT16               511
```

### Reading a from a file

```
marko@tserver$ hexibit -f example/file.bin -i ipv4,uint8,int16,bool

Data                             Interpretation       Value
-----------------------------------------------------------
C0A80001                         IPV4                 192.168.0.1
05                               UINT8                5
FFF2                             INT16                -14
01                               BOOL                 true
```

## Installation

* Clone or download the repo

* Make the repo

* Use the binary produced in the `bin/` directory

## Synopsis

### Hexadecimal Reader Mode
This mode allows you to interpret data from a hexadecimal string.
```bash
hexibit -s  <hex_string> -i <interpretation,...> [-b <byte_order>]
```
Where:
- `hex_string` is a hexadecimal string (With or without whitespaces, not case sensitive).
- `interpretation` is one of uint[8|16|32|64], int[8|16|32|64], double, ipv4, char_array[_\<length\>], bool, skipped_<length> (Not case sensitive).
- `byte_order` is one of LITTLE_ENDIAN, BIG_ENDIAN, LE, or BE. (Not case sensitive, defaulted to BE).

### File Reader Mode
Thie mode allows you to interpret data from a binary file.
```bash
hexibit -f  <filepath> -i <interpretation,...> [-b <byte_order> -o <offset>]
```
Where:
- `filepath` path of the file which data is to be interpreted.
- `interpretation` is one of uint[8|16|32|64], int[8|16|32|64], double, ipv4, char_array[_\<length\>], bool, skipped_<length> (Not case sensitive).
- `byte_order` is one of LITTLE_ENDIAN, BIG_ENDIAN, LE, or BE. (Not case sensitive, defaulted to BE).
- `offset` is the offset at which the data interpretation starts in the input file.

### Valid interpretations
Types of interpretations belong to 2 categories: static-length interpretations and dynamic-length interpretations.
#### Static-length interpretations:
The length of the interpretation is embedded in the interpretation itself (no length need to passed).
* **UINT8**
* **UINT16**
* **UINT32**
* **UINT64**
* **INT8**
* **INT16**
* **INT32**
* **INT64**
* **DOUBLE**
* **BOOL**
* **IPV4**
#### Dynamic-length interpretations:
The length of the interpretation can be an input passed to the interpretation
* **SKIPPED_\<length\>**: for this interpretation, the length parameter is mandatory and specifies the number of bytes to be skipped. For example:
  ```bash
  -i uint8,skipped_42,bool
  ```
  In the example above, the first byte is interpreted as a uint8, the following 42 bytes are skipped, and the 43rd one is interpreted as a bool.
  
* **CHAR_ARRAY[_\<length\>]**: for this interpretation, the length parameter is optional. 
  * If `<length>` is not specified, the sequence of bytes is treated as a null terminated array. 
  * If `<length>` is specified, then <length> bytes are interpreted as characters.

## Extending this project
*Hexibit* has been designed so that 2 core aspects can easily be extended:
1) Extending what type of data can be read by implementing the `IDataReader` interface. As of now, *hexibit* can read data from a hexadecimal string, or a file. One could imagine reading data from some other type of resources such as a network resource.
2) Extending how the interpretation is outputed by implementing the `IConsolePrinter` interface. As of now, there is only one type of printer which simply prints interpretations in the form of a list. You can create any other implementation you like to print the interpretations any way you want.

## Licensing
This project is licensed under [GPLv3](LICENSE).
