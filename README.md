# IosifBin
A new encoding format called IosifBin (not to be used as a practical encoding scheme)

### Scheme
Data and files can be encoded into strings of type "iosifbin"

Bytes are represented as the following:

|128  |64   |32   |16   |8    |4    |2    |1    |
|:---:|:---:|:---:|:---:|:---:|:---:|:---:|:---:|
|I/i  |O/o  |S/s  |I/i  |F/f  |B/b  |I/i  |N/n  |

Capital letters represent a 1 and lower-case represent a 0; For e.g.:

```
0x48 = 01001000 = iOsiFbin
0xb7 = 10110111 = IoSIfBIN

"Hello" = iOsiFbin iOSifBiN iOSiFBin iOSiFBin iOSiFBIN
```
\
For those who ask why create such an impratical encoding scheme:
Why not?

### Installation
```
git clone https://github.com/jojoCode123/IosifBin
cd IosifBin
make
sudo make install
```

### Usage
#### Encoding
`./iosifbin encode /path/to/input.txt /path/to/output.txt`

#### Decoding
`./iosifbin decode /path/to/input.txt path/to/output.txt`