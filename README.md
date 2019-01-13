# IRB-Hash

### Verification centered hashing algorithm based on the recaman sequence.

#### CGSF is a command line based program, no gui is currently available. For optimal usage please don't use a blocksize above 2.

---

To compile, download and run build.sh.

`sudo sh build.sh`

---

Hashing a file takes a blocksize, a file input, and a file output arguement.

Ex:
`./irbh-hash 2 input.txt hash.irbh`


To verify a file you can use the same syntax.

Ex:
`./irbh-verify 2 input.txt hash.irbh`