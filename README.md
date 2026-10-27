# ft_ssl

An introduction to cryptographic hashing algorithms: a small clone of
`openssl` with the `md5`, `sha224`, `sha256`, `sha384`, `sha512` and
`whirlpool` commands.

Every hash is implemented from its specification (RFC 1321, FIPS 180-4,
the Whirlpool paper); no crypto library is linked.

---

## Build

Requirements: `cc` and `make`. The test suite also needs the `openssl`
command line tool, used as the reference.

```sh
make            # builds libft.a then ./ft_ssl
make clean      # removes object files
make fclean     # removes object files, libft.a and ft_ssl
make re         # fclean + make
```

---

## Usage

```
./ft_ssl command [flags] [file/string]
```

| Command     | Digest size |
| ----------- | ----------- |
| `md5`       | 128 bits    |
| `sha224`    | 224 bits    |
| `sha256`    | 256 bits    |
| `sha384`    | 384 bits    |
| `sha512`    | 512 bits    |
| `whirlpool` | 512 bits    |

| Flag        | Meaning                                                    |
| ----------- | ---------------------------------------------------------- |
| `-p`        | echo STDIN to STDOUT and append the checksum               |
| `-q`        | quiet mode: print only the hash                            |
| `-r`        | reverse the output format: `hash name`                     |
| `-s string` | hash the given string                                      |

Rules:

- With no file and no `-s`, ft_ssl reads STDIN.
- Inputs are processed in this order: STDIN (`-p`), then each `-s`, then files.
- Flags are only read **before** the first file; after it everything is a file
  name (`./ft_ssl md5 file -s foo` hashes the files `file`, `-s` and `foo`).
- `-q` wins over `-r`.
- The exit status is `1` if anything failed (bad option, unreadable file).

### Examples

```console
$ echo "42 is nice" | ./ft_ssl md5
(stdin)= 35f1d6de0302e2086a4e472266efb3a9

$ echo "42 is nice" | ./ft_ssl md5 -p
("42 is nice")= 35f1d6de0302e2086a4e472266efb3a9

$ ./ft_ssl sha256 -s "42 is nice"
SHA256 ("42 is nice") = b7e44c7a40c5f80139f0a50f3650fb2bd8d00b0d24667c4c2ca32c88e13b758f

$ ./ft_ssl md5 -r Makefile
<hash> Makefile

$ ./ft_ssl md5 -q -s "foo"
acbd18db4cc2f85cedef654fccc4a4d8
```

> **`echo` vs `-s`** — `echo "abc" | ./ft_ssl md5` hashes `abc\n` (4 bytes,
> echo adds a newline) while `./ft_ssl md5 -s "abc"` hashes `abc` (3 bytes).
> Use `printf '%s' "abc"` or `echo -n` to pipe without the newline.

---

## Testing

### 1. Test one string or file (quick check)

`tests/compare.sh` hashes one input with **ft_ssl** and with **openssl**
and tells you if they match:

```sh
make compare S="hello world"                  # every algorithm
make compare S="hello world" A="md5 sha256"   # only some
make compare F=Makefile A=whirlpool           # a file
```

Or call the script directly (needed for strings containing `'` or for
an empty string):

```sh
sh tests/compare.sh -s "it's nice"
sh tests/compare.sh md5 -s ""
echo "42 is nice" | sh tests/compare.sh sha512    # stdin
```

Output:

```
input: string "hello world" (11 bytes)

md5        OK
  ft_ssl : 5eb63bbbe01eeed093cb22bb8f5acdc3
  openssl: 5eb63bbbe01eeed093cb22bb8f5acdc3
  ./ft_ssl md5 -s 'hello world'
```

### 2. Compare by hand with the original tools

The same input must give the same hash. Use `printf '%s'` so no newline is
added:

| ft_ssl                       | openssl                                                   | macOS             | Linux               |
| ---------------------------- | --------------------------------------------------------- | ----------------- | ------------------- |
| `./ft_ssl md5 -s "abc"`       | `printf '%s' abc \| openssl md5`                          | `md5 -s "abc"`    | `printf '%s' abc \| md5sum` |
| `./ft_ssl sha224 -s "abc"`    | `printf '%s' abc \| openssl sha224`                       | `printf '%s' abc \| shasum -a 224` | `printf '%s' abc \| sha224sum` |
| `./ft_ssl sha256 -s "abc"`    | `printf '%s' abc \| openssl sha256`                       | `printf '%s' abc \| shasum -a 256` | `printf '%s' abc \| sha256sum` |
| `./ft_ssl sha384 -s "abc"`    | `printf '%s' abc \| openssl sha384`                       | `printf '%s' abc \| shasum -a 384` | `printf '%s' abc \| sha384sum` |
| `./ft_ssl sha512 -s "abc"`    | `printf '%s' abc \| openssl sha512`                       | `printf '%s' abc \| shasum -a 512` | `printf '%s' abc \| sha512sum` |
| `./ft_ssl whirlpool -s "abc"` | `printf '%s' abc \| openssl dgst -provider legacy -whirlpool` | —             | —                   |

For a file, give the same file to both: `./ft_ssl sha256 file` and
`openssl sha256 file` (or `shasum -a 256 file`, `sha256sum file`).

Whirlpool lives in OpenSSL 3+'s *legacy* provider, hence the extra flag.

### 3. Full test suite

```sh
make test                   # every algorithm (304 tests)
make test md5               # one algorithm
make test sha256 whirlpool  # several
```

For every algorithm it checks, against `openssl dgst`:

- STDIN with and without `-p`, `-q`, `-r`, and an empty STDIN
- `-s` (including an empty string and repeated `-s`)
- files, several files, and that STDIN is ignored when a file is given
- the processing order (STDIN, strings, files) and that flags after a file
  are treated as files
- error messages and exit status (missing file, directory, bad option,
  `-s` without argument, invalid command)
- input lengths around the block boundaries (55, 56, 63, 64, 111, 112, 127,
  128 … bytes), a binary file and 5 MB through STDIN

`md5` and `sha256` are also checked against the exact outputs printed in the
subject. Results are printed as one table per suite plus a summary; on a
failure the expected and actual output are shown under the tables and
`make test` exits with `1`.

---

## Norm

```sh
make norm     # norminette on src/, include/ and libft/
```

The code follows [Norm v4.1](https://github.com/42school/norminette/blob/master/pdf/en.norm.pdf),
including the rules norminette does not check (*): explicit names, no
non-const globals, no unused includes, no `#pragma`, Makefile without
wildcards and without relinking.

Prints only the errors, or `norminette: all files OK`.
Install norminette with `python3 -m pip install --user norminette`.

---

## How the algorithms work

Every algorithm is written below in Python, step by step. The example message
is `"abc"`, and the comments show the value at each step. All the blocks
together form one runnable program (`python3`) that prints the real hashes.

```text
message ─► pad ─► split into blocks ─► mix each block into the state ─► state = digest
```

### Shared helpers

```python
import math

MASK32 = 0xFFFFFFFF
MASK64 = 0xFFFFFFFFFFFFFFFF

def rotl32(x, n): return ((x << n) | (x >> (32 - n))) & MASK32
def rotr32(x, n): return ((x >> n) | (x << (32 - n))) & MASK32
def rotr64(x, n): return ((x >> n) | (x << (64 - n))) & MASK64

def pad(message, block_size, length_size, byteorder):
    padded = message + b"\x80"                              # one 1 bit, then 0 bits
    while len(padded) % block_size != block_size - length_size:
        padded += b"\x00"
    bit_length = len(message) * 8                           # length in BITS
    return padded + bit_length.to_bytes(length_size, byteorder)

def words(block, size, byteorder):
    return [int.from_bytes(block[i:i + size], byteorder) for i in range(0, len(block), size)]

message = b"abc"            # 61 62 63  ->  3 bytes  ->  24 bits = 0x18
```

---

### MD5

**Step 1: pad** (64-byte blocks, 8-byte length, little-endian)

```python
padded = pad(message, 64, 8, "little")
# 61 62 63 80 00 00 00 00 00 00 00 00 00 00 00 00
# 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
# 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
# 00 00 00 00 00 00 00 00 18 00 00 00 00 00 00 00
```

**Step 2: constants and initial state**

```python
T = [int(abs(math.sin(i + 1)) * 2**32) & MASK32 for i in range(64)]
# T = [0xd76aa478, 0xe8c7b756, 0x242070db, ... , 0xeb86d391]

SHIFT = [[7, 12, 17, 22], [5, 9, 14, 20], [4, 11, 16, 23], [6, 10, 15, 21]]

MD5_INIT = [0x67452301, 0xefcdab89, 0x98badcfe, 0x10325476]
```

**Step 3: mix one block (4 rounds × 16 operations)**

```python
def md5_block(state, block):
    X = words(block, 4, "little")
    # "abc": X[0] = 0x80636261, X[14] = 0x00000018, the rest 0

    a, b, c, d = state
    for i in range(64):
        rnd, j = i // 16, i % 16
        if rnd == 0:
            f, k = (b & c) | (~b & d), j
        elif rnd == 1:
            f, k = (b & d) | (c & ~d), (1 + 5 * j) % 16
        elif rnd == 2:
            f, k = b ^ c ^ d, (5 + 3 * j) % 16
        else:
            f, k = c ^ (b | ~d), (7 * j) % 16
        f &= MASK32
        new = (b + rotl32((a + f + X[k] + T[i]) & MASK32, SHIFT[rnd][i % 4])) & MASK32
        a, b, c, d = d, new, b, c
        # "abc", i = 0:  new = 0xd6d117b4
    # "abc", after 64 operations: a b c d = 310ade8f c08226b3 e484b9d8 624d8cb2

    return [(s + v) & MASK32 for s, v in zip(state, (a, b, c, d))]
    # "abc": 98500190 b04fd23c 7d3f96d6 727fe128
```

**Step 4: all blocks, then output little-endian**

```python
def md5(message):
    padded = pad(message, 64, 8, "little")
    state = MD5_INIT
    for i in range(0, len(padded), 64):
        state = md5_block(state, padded[i:i + 64])
    return b"".join(w.to_bytes(4, "little") for w in state)

print(md5(b"abc").hex())    # 900150983cd24fb0d6963f7d28e17f72
#                             98500190 -> 90 01 50 98 ...
```

| Message | MD5 |
| ------- | --- |
| `""` | `d41d8cd98f00b204e9800998ecf8427e` |
| `"a"` | `0cc175b9c0f1b6a831c399e269772661` |
| `"abc"` | `900150983cd24fb0d6963f7d28e17f72` |
| `"message digest"` | `f96b697d7cb7938d525a2f31aaf161d0` |

---

### SHA-256 and SHA-224

**Step 1: pad** (64-byte blocks, 8-byte length, big-endian)

```python
padded = pad(message, 64, 8, "big")
# 61 62 63 80 00 00 ... 00 00 00 00 00 00 00 00 18    (64 bytes)
```

**Step 2: constants and initial states**

```python
def primes(n):
    found, x = [], 2
    while len(found) < n:
        if all(x % p for p in found):
            found.append(x)
        x += 1
    return found

def fraction_bits(p, root, bits):
    # the first `bits` bits after the point of p ** (1 / root), exactly
    x = round((p << (root * bits)) ** (1 / root))
    while x ** root > p << (root * bits): x -= 1
    while (x + 1) ** root <= p << (root * bits): x += 1
    return x & ((1 << bits) - 1)

K256 = [fraction_bits(p, 3, 32) for p in primes(64)]            # cube roots
# [0x428a2f98, 0x71374491, 0xb5c0fbcf, ... , 0xc67178f2]

SHA256_INIT = [fraction_bits(p, 2, 32) for p in primes(8)]      # square roots
# [0x6a09e667, 0xbb67ae85, 0x3c6ef372, 0xa54ff53a,
#  0x510e527f, 0x9b05688c, 0x1f83d9ab, 0x5be0cd19]

SHA224_INIT = [fraction_bits(p, 2, 64) & MASK32 for p in primes(16)[8:]]
# [0xc1059ed8, 0x367cd507, 0x3070dd17, 0xf70e5939,
#  0xffc00b31, 0x68581511, 0x64f98fa7, 0xbefa4fa4]
```

**Step 3: mix one block (64 steps)**

```python
def sha256_block(state, block):
    W = words(block, 4, "big")
    # "abc": W[0] = 0x61626380, W[15] = 0x00000018, the rest 0
    for t in range(16, 64):
        s0 = rotr32(W[t - 15], 7) ^ rotr32(W[t - 15], 18) ^ (W[t - 15] >> 3)
        s1 = rotr32(W[t - 2], 17) ^ rotr32(W[t - 2], 19) ^ (W[t - 2] >> 10)
        W.append((W[t - 16] + s0 + W[t - 7] + s1) & MASK32)
    # "abc": W[16] = 0x61626380, W[17] = 0x000f0000

    a, b, c, d, e, f, g, h = state
    for t in range(64):
        S1 = rotr32(e, 6) ^ rotr32(e, 11) ^ rotr32(e, 25)
        ch = (e & f) ^ (~e & g)
        T1 = (h + S1 + ch + K256[t] + W[t]) & MASK32
        S0 = rotr32(a, 2) ^ rotr32(a, 13) ^ rotr32(a, 22)
        maj = (a & b) ^ (a & c) ^ (b & c)
        T2 = (S0 + maj) & MASK32
        a, b, c, d, e, f, g, h = (T1 + T2) & MASK32, a, b, c, (d + T1) & MASK32, e, f, g
        # "abc", t = 0: 5d6aebcd 6a09e667 bb67ae85 3c6ef372 fa2a4622 510e527f 9b05688c 1f83d9ab
        # "abc", t = 1: 5a6ad9ad 5d6aebcd 6a09e667 bb67ae85 78ce7989 fa2a4622 510e527f 9b05688c

    return [(s + v) & MASK32 for s, v in zip(state, (a, b, c, d, e, f, g, h))]
```

**Step 4: all blocks, then output big-endian**

```python
def sha256_family(message, init, out_words):
    padded = pad(message, 64, 8, "big")
    state = init
    for i in range(0, len(padded), 64):
        state = sha256_block(state, padded[i:i + 64])
    return b"".join(w.to_bytes(4, "big") for w in state[:out_words])

def sha256(message): return sha256_family(message, SHA256_INIT, 8)
def sha224(message): return sha256_family(message, SHA224_INIT, 7)   # drop the last word

print(sha256(b"abc").hex())  # ba7816bf8f01cfea414140de5dae2223b00361a396177a9cb410ff61f20015ad
print(sha224(b"abc").hex())  # 23097d223405d8228642a477bda255b32aadbce4bda0b3f7e36c9da7
```

---

### SHA-512 and SHA-384

Same as SHA-256, with 64-bit words, 128-byte blocks, a 16-byte length, 80 steps
and other rotation amounts.

**Step 1: pad** (128-byte blocks, 16-byte length, big-endian)

```python
padded = pad(message, 128, 16, "big")
# 61 62 63 80 00 00 ... 00 00 00 00 00 00 00 00 18    (128 bytes)
```

**Step 2: constants and initial states**

```python
K512 = [fraction_bits(p, 3, 64) for p in primes(80)]
# [0x428a2f98d728ae22, 0x7137449123ef65cd, ... , 0x6c44198c4a475817]

SHA512_INIT = [fraction_bits(p, 2, 64) for p in primes(8)]
# [0x6a09e667f3bcc908, 0xbb67ae8584caa73b, ... , 0x5be0cd19137e2179]

SHA384_INIT = [fraction_bits(p, 2, 64) for p in primes(16)[8:]]
# [0xcbbb9d5dc1059ed8, 0x629a292a367cd507, ... , 0x47b5481dbefa4fa4]
```

**Step 3: mix one block (80 steps)**

```python
def sha512_block(state, block):
    W = words(block, 8, "big")
    # "abc": W[0] = 0x6162638000000000, W[15] = 0x18
    for t in range(16, 80):
        s0 = rotr64(W[t - 15], 1) ^ rotr64(W[t - 15], 8) ^ (W[t - 15] >> 7)
        s1 = rotr64(W[t - 2], 19) ^ rotr64(W[t - 2], 61) ^ (W[t - 2] >> 6)
        W.append((W[t - 16] + s0 + W[t - 7] + s1) & MASK64)

    a, b, c, d, e, f, g, h = state
    for t in range(80):
        S1 = rotr64(e, 14) ^ rotr64(e, 18) ^ rotr64(e, 41)
        ch = (e & f) ^ (~e & g)
        T1 = (h + S1 + ch + K512[t] + W[t]) & MASK64
        S0 = rotr64(a, 28) ^ rotr64(a, 34) ^ rotr64(a, 39)
        maj = (a & b) ^ (a & c) ^ (b & c)
        T2 = (S0 + maj) & MASK64
        a, b, c, d, e, f, g, h = (T1 + T2) & MASK64, a, b, c, (d + T1) & MASK64, e, f, g
        # "abc", t = 0: a = f6afceb8bcfcddf5, e = 58cb02347ab51f91

    return [(s + v) & MASK64 for s, v in zip(state, (a, b, c, d, e, f, g, h))]
```

**Step 4: all blocks, then output big-endian**

```python
def sha512_family(message, init, out_words):
    padded = pad(message, 128, 16, "big")
    state = init
    for i in range(0, len(padded), 128):
        state = sha512_block(state, padded[i:i + 128])
    return b"".join(w.to_bytes(8, "big") for w in state[:out_words])

def sha512(message): return sha512_family(message, SHA512_INIT, 8)
def sha384(message): return sha512_family(message, SHA384_INIT, 6)   # drop the last 2 words

print(sha512(b"abc").hex())
# ddaf35a193617abacc417349ae20413112e6fa4e89a97ea20a9eeee64b55d39a
# 2192992a274fc1a836ba3c23a3feebbd454d4423643ce80e2a9ac94fa54ca49f
print(sha384(b"abc").hex())
# cb00753f45a35e8bb5a03d699ac65007272c32ab0eded163
# 1a8b605a43ff5bed8086072ba1e7cc2358baeca134c825a7
```

---

### Whirlpool

The state is an 8 × 8 grid of bytes, filled row by row from a 64-byte block.

**Step 1: pad** (64-byte blocks, 32-byte length, big-endian)

```python
padded = pad(message, 64, 32, "big")
# 61 62 63 80 00 00 00 00
# 00 00 00 00 00 00 00 00
# 00 00 00 00 00 00 00 00
# 00 00 00 00 00 00 00 00
# 00 00 00 00 00 00 00 00
# 00 00 00 00 00 00 00 00
# 00 00 00 00 00 00 00 00
# 00 00 00 00 00 00 00 18

def to_grid(block):  return [list(block[8 * row:8 * row + 8]) for row in range(8)]
def to_bytes(grid):  return bytes(v for row in grid for v in row)
def xor(g1, g2):     return [[g1[i][j] ^ g2[i][j] for j in range(8)] for i in range(8)]
```

**Step 2: the S-box** (256 bytes, built from three 16-entry tables)

```python
E  = [0x1, 0xB, 0x9, 0xC, 0xD, 0x6, 0xF, 0x3, 0xE, 0x8, 0x7, 0x4, 0xA, 0x2, 0x5, 0x0]
R  = [0x7, 0xC, 0xB, 0xD, 0xE, 0x4, 0x9, 0xF, 0x6, 0x3, 0x8, 0xA, 0x2, 0x5, 0x1, 0x0]
Ei = [E.index(v) for v in range(16)]                     # inverse of E

SBOX = []
for x in range(256):
    u, v = x >> 4, x & 0xF                               # high half, low half
    r = R[E[u] ^ Ei[v]]
    SBOX.append((E[E[u] ^ r] << 4) | Ei[Ei[v] ^ r])
# SBOX = [0x18, 0x23, 0xc6, 0xe8, 0x87, 0xb8, 0x01, 0x4f, ...]
```

**Step 3: multiplication of bytes** (GF(2^8), reduction polynomial `0x11d`)

```python
def gf_mul(a, b):
    result = 0
    while b:
        if b & 1:
            result ^= a
        a <<= 1
        if a & 0x100:
            a ^= 0x11D
        b >>= 1
    return result
# gf_mul(0x18, 3) = 0x28
```

**Step 4: one round** (substitute → shift columns → mix rows → add key)

```python
C = [0x01, 0x01, 0x04, 0x01, 0x08, 0x05, 0x02, 0x09]

def round_function(grid, key):
    sub   = [[SBOX[grid[i][j]] for j in range(8)] for i in range(8)]
    shift = [[sub[(i - j) % 8][j] for j in range(8)] for i in range(8)]   # column j down by j
    mix   = [[0] * 8 for _ in range(8)]
    for i in range(8):
        for j in range(8):
            for k in range(8):
                mix[i][j] ^= gf_mul(shift[i][k], C[(j - k) % 8])
    return xor(mix, key)
```

Round 1 for `"abc"`:

```text
sub                       shift                     mix                       + key = round 1
0f d5 80 97 18 18 18 18   0f 18 18 18 18 18 18 18   3f 3f 74 3f 90 63 06 87   0f 34 9a ff 3f f3 2f e0
18 18 18 18 18 18 18 18   18 d5 18 18 18 18 18 18   c3 e5 e5 3b e5 0e f6 af   eb cd cd 13 cd 26 de 87
18 18 18 18 18 18 18 18   18 18 80 18 18 18 18 18   05 04 b0 b0 72 b0 9c ea   2d 2c 98 98 5a 98 b4 c2
18 18 18 18 18 18 18 18   18 18 18 97 18 18 18 18   a1 2b ab a7 a7 2e a7 24   89 03 83 8f 8f 06 8f 0c
18 18 18 18 18 18 18 18   18 18 18 18 18 18 18 18   28 28 28 28 28 28 28 28   00 00 00 00 00 00 00 00
18 18 18 18 18 18 18 18   18 18 18 18 18 18 18 18   28 28 28 28 28 28 28 28   00 00 00 00 00 00 00 00
18 18 18 18 18 18 18 18   18 18 18 18 18 18 18 1d   2d 3c 2d 00 39 22 05 2d   05 14 05 28 11 0a 2d 05
18 18 18 18 18 18 18 1d   18 18 18 18 18 18 18 18   28 28 28 28 28 28 28 28   00 00 00 00 00 00 00 00
```

**Step 5: the block cipher** (10 rounds; the key goes through the same rounds)

```python
def encrypt(block, key):
    state = xor(block, key)
    for r in range(1, 11):
        constant = [SBOX[8 * (r - 1):8 * r]] + [[0] * 8 for _ in range(7)]
        key = round_function(key, constant)
        state = round_function(state, key)
    return state
# "abc", key after round 1:
# 30 0b ee c0 af 90 29 67
# 28 28 28 28 28 28 28 28   (rows 1 to 7 the same)
```

**Step 6: all blocks, then output row by row**

```python
def whirlpool(message):
    padded = pad(message, 64, 32, "big")
    H = [[0] * 8 for _ in range(8)]
    for i in range(0, len(padded), 64):
        m = to_grid(padded[i:i + 64])
        H = xor(xor(encrypt(m, H), H), m)
    return to_bytes(H)

print(whirlpool(b"abc").hex())
# 4e2448a4c6f486bb16b6562c73b4020bf3043e3a731bce721ae1b303d97e6d4c
# 7181eebdb6c57e277d0e34957114cbd6c797fc9d95d8b582d225292076d4eef5
```

The state after every round, for `"abc"` and other messages, is in the
official ISO test data (`whirlpool-iso-intermediate-values.txt`, distributed
with the Whirlpool reference implementation).

---

### Test vectors

```text
M0 = ""                       (empty)
M1 = "abc"
M2 = "abcdbcdecdefdefgefghfghighijhijkijkljklmklmnlmnomnopnopq"      (56 bytes: 2 blocks for MD5 / SHA-256)
M3 = "abcdefghbcdefghicdefghijdefghijkefghijklfghijklmghijklmnhijklmnoijklmnopjklmnopqklmnopqrlmnopqrsmnopqrstnopqrstu"
                                                                     (112 bytes: 2 blocks for SHA-512)
M4 = "a" repeated 1,000,000 times
```

**MD5**

```text
M0  d41d8cd98f00b204e9800998ecf8427e
M1  900150983cd24fb0d6963f7d28e17f72
M2  8215ef0796a20bcaaae116d3876c664a
M3  03dd8807a93175fb062dfb55dc7d359c
M4  7707d6ae4e027c70eea2a935c2296f21
```

**SHA-224**

```text
M0  d14a028c2a3a2bc9476102bb288234c415a2b01f828ea62ac5b3e42f
M1  23097d223405d8228642a477bda255b32aadbce4bda0b3f7e36c9da7
M2  75388b16512776cc5dba5da1fd890150b0c6455cb4f58b1952522525
M3  c97ca9a559850ce97a04a96def6d99a9e0e0e2ab14e6b8df265fc0b3
M4  20794655980c91d8bbb4c1ea97618a4bf03f42581948b2ee4ee7ad67
```

**SHA-256**

```text
M0  e3b0c44298fc1c149afbf4c8996fb92427ae41e4649b934ca495991b7852b855
M1  ba7816bf8f01cfea414140de5dae2223b00361a396177a9cb410ff61f20015ad
M2  248d6a61d20638b8e5c026930c3e6039a33ce45964ff2167f6ecedd419db06c1
M3  cf5b16a778af8380036ce59e7b0492370b249b11e8f07a51afac45037afee9d1
M4  cdc76e5c9914fb9281a1c7e284d73e67f1809a48a497200e046d39ccc7112cd0
```

**SHA-384**

```text
M0  38b060a751ac96384cd9327eb1b1e36a21fdb71114be07434c0cc7bf63f6e1da274edebfe76f65fbd51ad2f14898b95b
M1  cb00753f45a35e8bb5a03d699ac65007272c32ab0eded1631a8b605a43ff5bed8086072ba1e7cc2358baeca134c825a7
M2  3391fdddfc8dc7393707a65b1b4709397cf8b1d162af05abfe8f450de5f36bc6b0455a8520bc4e6f5fe95b1fe3c8452b
M3  09330c33f71147e83d192fc782cd1b4753111b173b3b05d22fa08086e3b0f712fcc7c71a557e2db966c3e9fa91746039
M4  9d0e1809716474cb086e834e310a4a1ced149e9c00f248527972cec5704c2a5b07b8b3dc38ecc4ebae97ddd87f3d8985
```

**SHA-512**

```text
M0  cf83e1357eefb8bdf1542850d66d8007d620e4050b5715dc83f4a921d36ce9ce47d0d13c5d85f2b0ff8318d2877eec2f63b931bd47417a81a538327af927da3e
M1  ddaf35a193617abacc417349ae20413112e6fa4e89a97ea20a9eeee64b55d39a2192992a274fc1a836ba3c23a3feebbd454d4423643ce80e2a9ac94fa54ca49f
M2  204a8fc6dda82f0a0ced7beb8e08a41657c16ef468b228a8279be331a703c33596fd15c13b1b07f9aa1d3bea57789ca031ad85c7a71dd70354ec631238ca3445
M3  8e959b75dae313da8cf4f72814fc143f8f7779c6eb9f7fa17299aeadb6889018501d289e4900f7e4331b99dec4b5433ac7d329eeb6dd26545e96e55b874be909
M4  e718483d0ce769644e2e42c7bc15b4638e1f98b13b2044285632a803afa973ebde0ff244877ea60a4cb0432ce577c31beb009c5c2c49aa2e4eadb217ad8cc09b
```

**Whirlpool**

```text
M0  19fa61d75522a4669b44e39c1d2e1726c530232130d407f89afee0964997f7a73e83be698b288febcf88e3e03c4f0757ea8964e59b63d93708b138cc42a66eb3
M1  4e2448a4c6f486bb16b6562c73b4020bf3043e3a731bce721ae1b303d97e6d4c7181eebdb6c57e277d0e34957114cbd6c797fc9d95d8b582d225292076d4eef5
M2  526b2394d85683e24b29acd0fd37f7d5027f61366a1407262dc2a6a345d9e240c017c1833db1e6db6a46bd444b0c69520c856e7c6e9c366d150a7da3aeb160d1
M3  14aa95962750ed385bed2b9f43fbad41483a8910221723a5f15a0614e74fb12fe7d5523abd8ab0c09cb77852159deef4e9eb9808e54a0b83f26865d121af3c0b
M4  0c99005beb57eff50a7cf005560ddf5d29057fd86b20bfd62deca0f1ccea4af51fc15490eddc47af32bb2b66c34ff9ad8c6008ad677f77126953b226e4ed8b01
```

### Common mistakes

- Byte order: MD5 is little-endian, the others big-endian.
- The length is in **bits**: `"abc"` → `0x18`.
- Padding can need one extra block: test lengths 55, 56, 63, 64 (MD5,
  SHA-224/256), 111, 112 (SHA-384/512) and 31, 32 (Whirlpool).
- Add the old state back at the end of each block.
- SHA-224 and SHA-384 have their own initial state.
- To print a constant table for your own code: `print([hex(k) for k in K256])`.

---

## How the hash code is organised

Each algorithm exposes one function:

```c
void	ft_sha256(const uint8_t *data, size_t len, uint8_t *digest);
```

It receives the whole input (`data`, `len` bytes) and writes the raw digest
(`LEN_SHA256` = 32 bytes, see [include/hash.h](include/hash.h)) into `digest`.

All of them are block hashes, so the cutting and padding is shared:
`hash_message()` ([src/hash/hash_message.c](src/hash/hash_message.c)) takes a
`t_block_hash` (block size, length field size, byte order, transform and its
state), feeds every full block to the transform, then the padded tail.
Each algorithm file only provides its transform and initial state:

| File               | Contents                                              |
| ------------------ | ----------------------------------------------------- |
| `hash_message.c`   | padding and block loop shared by every algorithm      |
| `md5.c`            | MD5 transform and `ft_md5`                            |
| `sha256_core.c`    | SHA-256 transform, `sha256_run` (used by 224 and 256) |
| `sha256.c`         | `ft_sha224` and `ft_sha256` (initial states)          |
| `sha512_core.c`    | SHA-512 transform, `sha512_run` (used by 384 and 512) |
| `sha512.c`         | `ft_sha384` and `ft_sha512` (initial states)          |
| `whirlpool.c`      | S-box, W block cipher and `ft_whirlpool`              |

## Adding a new hash

1. Create `src/hash/<name>.c` with its transform, `ft_<name>()` (filling a
   `t_block_hash` and calling `hash_message()`) and its descriptor:
   `const t_hash g_<name> = {"<name>", "<LABEL>", <digest bytes>, ft_<name>};`
2. Declare both in [include/hash.h](include/hash.h).
3. Add one line `{"<name>", digest_command, &g_<name>},` to the command table
   in [src/commands.c](src/commands.c).
4. Add the file to `SRCS` in the [Makefile](Makefile), and the name to
   `ALL_ALGOS` in `tests/run_tests.sh` and `tests/compare.sh`.

All flags and output formats work automatically.

---

## Project layout

```
include/
  ft_ssl.h          command table types
  digest.h          t_hash (one per algorithm), flags, digest command
  hash.h            every ft_<hash>, its descriptor and t_block_hash
src/
  main.c            ft_ssl entry point: finds the command
  commands.c        command table (name -> function)
  digest/
    digest_cmd.c    flag parsing and processing order
    digest_input.c  stdin / string / file inputs
    digest_output.c output formats and error messages
  hash/
    hash_message.c  padding and block loop shared by every algorithm
    md5.c           MD5
    sha256_core.c   SHA-256 compression
    sha256.c        sha224 and sha256
    sha512_core.c   SHA-512 compression
    sha512.c        sha384 and sha512
    whirlpool.c     Whirlpool
libft/              libft + get_next_line + ft_printf (ft_dprintf)
  ft_read_all.c     reads a whole fd into memory (binary safe)
  ft_bits/          rotations, endian loads/stores, bytes to hex
tests/
  run_tests.sh      full test suite (make test)
  compare.sh        one input vs openssl (make compare)
```
