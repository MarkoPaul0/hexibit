# hexibit

![Author](https://img.shields.io/badge/author-MarkoPaul0-red.svg?style=flat-square)
[![License: GPL v3](https://img.shields.io/badge/License-GPL%20v3-blue.svg?style=flat-square)](https://www.gnu.org/licenses/gpl-3.0.en.html)
![GitHub last commit](https://img.shields.io/github/last-commit/MarkoPaul0/hexibit.svg?style=flat-square&maxAge=300)
![Stars](https://img.shields.io/github/stars/MarkoPaul0/hexibit.svg?style=social)


Command line tool to facilitate the analysis of hexadecimal data.
Here are some example:

##### Reading from an hexadecimal string

```bash
markopaulo@test_server$: hexibit -s  "68656c6c6f20776f726c6401FF" -i char_array_11,uint16

Data                             Interpretation       Value
-----------------------------------------------------------
68656C6C6F20776F726C64           CHAR_ARRAY           hello world
01FF                             UINT16               511
```

##### Reading a from a file

```
marko@tserver$ hexibit -t example/file.bin -i ipv4,uint8,int16,bool

Data                             Interpretation       Value
-----------------------------------------------------------
FFFF0001                         IPV4                 192.168.0.1
05                               UINT8                5
FFFF                             INT16                -14
01                               BOOL                 true
```

## Installation

* A computer running a linux distribution
