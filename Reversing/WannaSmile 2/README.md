# Reversing - WannaSmile 2

## Challenge
Thank you for helping me stop the virus! can you recover my files now?

ssh wannasmile@challenges1.hexionteam.com -p 4000
Password: hexctf

Author: moka

## Solution
This is a reverse challenge. With Ghidra we can decompile the binary.
Interesting function was `FUN_00101351` (only interesting part below):
```
[...]
plaintext = (uchar *)calloc(plaintext_len,1);
out = calloc(plaintext_len + 0x10,1);
fread(plaintext,1,plaintext_len,local_48);
fclose(local_48);
sizeOf_enc_key = memcmp("WANNASMILE",plaintext,10);
if (sizeOf_enc_key != 0) {
  FUN_0010127a(key,0x20);
  ctx = EVP_CIPHER_CTX_new();
  cipher_type = EVP_aes_256_cbc();
  EVP_EncryptInit_ex(ctx,cipher_type,(ENGINE *)0x0,key,(uchar *)0x0);
  EVP_EncryptUpdate(ctx,(uchar *)((long)local_5c + (long)out),&out_len,plaintext,(int)plaintext_len);
  local_5c = local_5c + out_len;
  EVP_EncryptFinal_ex(ctx,(uchar *)((long)out_len + (long)out),&out_len);
  local_5c = local_5c + out_len;
  EVP_CIPHER_CTX_free(ctx);
  rsa = DAT_00303a20;
  size_of_key = strlen((char *)key);
  RSA_public_encrypt((int)size_of_key,key,enc_key,rsa,1);
  local_48 = fopen(param_1,"wb");
  fwrite("WANNASMILE",1,10,local_48);
  sizeOf_enc_key = RSA_size(DAT_00303a20);
  fwrite(enc_key,1,(long)sizeOf_enc_key,local_48);
  fwrite(out,1,(long)local_5c,local_48);
[...]
```
From code above we can read how encrypted files are created:
1. Program checks if there is a string `WANNASMILE` in first 10 bytes of a file.
2. If it's not there, program generates 32 bytes long key.
3. Use that key to encrypt the file using AES 256 CBC.
4. Next the key is encrypted with private RSA key
5. At the end new binary is constructed:
 - first 10 bytes with string `WANNASMILE`.
 - next 256 bytes encrypted key
 - rest of the file encrypted original file

On remote server we can find some interesting files:
```
wannasmile@c93d605716ed:~$ ls -al /var/wannasmile2/
total 1608
dr-xr-xr-x 1 root root    4096 Apr 12 22:18 .
drwxr-xr-x 1 root root    4096 Apr 12 22:18 ..
-r--r--r-- 1 root root   91722 Apr 12 22:19 HexionLogo.png
-r--r--r-- 1 root root  253706 Apr 12 22:19 aaaaa.png
-r--r--r-- 1 root root     314 Apr 12 22:19 flag2.txt
-r--r--r-- 1 root root 1277978 Apr 12 22:19 present.pptx
```
`flag2.txt` looks interesting.

Based on the recipe from above, we can revert operations:
a) Remove `WANNASMILE`:
```
dd bs=1 skip=10 if=flag2.txt of=flag2.txt.clean
```
b) Extract encrypted key:
```
dd bs=1 count=256 if=flag2.txt.clean of=flag2.txt.rsa
```
c) Extract encrypted content:
```
dd bs=1 skip=256 if=flag2.txt.clean of=flag2.txt.enc
```
Now we need to restore encryption key, good news private RSA key is inside binary.

With that I wrote a small code in - [rsa.c](rsa.c).

As a result we can get encryption key:
```
$ gcc rsa.c -o rsa -L/usr/lib -lssl -lcrypto; ./rsa
rsa data size: 256
Decrypted Text = jmgcgmseonnehdkwpapcygwyabpwqvob
Decrypted Length = 32
```

Result this code we can pass to another small program where I will decode
original file - [enc.c](enc.c)
```
$ gcc enc.c -o enc -L/usr/lib -lssl -lcrypto; ./enc
$ cat flag2.txt.dec
hexCTF{1_d0nt_w4nt_t0_smil3_4nym0r3}
```

## Flag
```
hexCTF{1_d0nt_w4nt_t0_smil3_4nym0r3}
```
