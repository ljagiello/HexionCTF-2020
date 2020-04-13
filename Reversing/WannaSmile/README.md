# Reversing - WannaSmile

## Challenge
I accidentally executed this weird file that encrypted my important files! please help me understand what it does

ssh wannasmile@challenges1.hexionteam.com -p 4000
Password: hexctf

* This challenge consists of 2 parts.
** Attached version has the first flag removed.

Author: moka

## Solution
If we run the binary, we will see:
```
:~$ ./wannasmile
-- Roleplay mode activate ---
SEND 1627 BITCOINS IF YOU WANT YOUR FILES BACK
Address: 12t9YDPgwueZ9NyMgw519p7AA8isjr6SMw
```

I usually check everything with `strace` to see what program does. For a better
visibility lets narrow down to network only:
```
:~$ strace -s 4096 -ff -e trace=network ./wannasmile
socket(AF_UNIX, SOCK_STREAM|SOCK_CLOEXEC|SOCK_NONBLOCK, 0) = 3
connect(3, {sa_family=AF_UNIX, sun_path="/var/run/nscd/socket"}, 110) = -1 ENOENT (No such file or directory)
socket(AF_UNIX, SOCK_STREAM|SOCK_CLOEXEC|SOCK_NONBLOCK, 0) = 3
connect(3, {sa_family=AF_UNIX, sun_path="/var/run/nscd/socket"}, 110) = -1 ENOENT (No such file or directory)
socket(AF_INET, SOCK_DGRAM|SOCK_CLOEXEC|SOCK_NONBLOCK, IPPROTO_IP) = 3
connect(3, {sa_family=AF_INET, sin_port=htons(53), sin_addr=inet_addr("10.0.0.1")}, 16) = 0
sendto(3, "t)\1\0\0\1\0\0\0\0\0\0\20uaresuchacrybaby\0\0\1\0\1", 34, MSG_NOSIGNAL, NULL, 0) = 34
recvfrom(3, "t)\201\203\0\1\0\0\0\1\0\0\20uaresuchacrybaby\0\0\1\0\1\0\0\6\0\1\0\1Q\200\0@\1a\froot-servers\3net\0\5nstld\fverisign-grs\3com\0xga\361\0\0\7\10\0\0\3\204\0\t:\200\0\1Q\200", 1024, 0, {sa_family=AF_INET, sin_port=htons(53), sin_addr=inet_addr("10.0.0.1")}, [28->16]) = 109
strace: Process 13463 attached
-- Roleplay mode activate ---
SEND 1627 BITCOINS IF YOU WANT YOUR FILES BACK
Address: 12t9YDPgwueZ9NyMgw519p7AA8isjr6SMw
[...]
```
Ok, so the program tries to resolve domain: `uaresuchacrybaby` and obviously
fails. If we add this domain to `/etc/hosts`, result is promessing:
```
:~$ ./wannasmile
<REDACTED> (shell server has flag here)
```
It looks like the easiest way to resolve this challange is to add missing
domain to dns responses. Unfortunately remote server is significantly
more restrict what we can do and for sure we cannot modify `/etc/hosts`.

After poking around I've found directory with write access - `/var/lock`.
Now we need to cheat resolver. We can use `libnss-wrapper` and preload
this library before executing `wannasmile`.

The library is not there, but we can scp version for Ubuntu 18.04 to directory
with write access.

With everything in place we can execute binary on remote server:
```
wannasmile@1de660fa6809:/var/lock$ ls -al
total 64
drwxrwxrwt 1 root       root        4096 Apr 12 18:06 .
drwxr-xr-x 1 root       root        4096 Apr 12 18:10 ..
-rw-rw-r-- 1 wannasmile wannasmile    59 Apr 12 18:06 fakedns
-rw-r--r-- 1 wannasmile wannasmile 22760 Apr 12 18:00 libresolv_wrapper.so
drwxr-xr-x 2 root       root        4096 Apr 11 16:37 subsys
wannasmile@1de660fa6809:/var/lock$ cat fakedns
A uaresuchacrybaby 127.0.0.1
A uaresuchacrybaby. 127.0.0.1
wannasmile@1de660fa6809:/var/lock$ LD_PRELOAD="./libnss_wrapper.so" NSS_WRAPPER_HOSTS="./fakehosts" /home/wannasmile/wannasmile
hexCTF{wh0_put5_k1ll_sw1tch3s_in_the1r_m4lw4r3}
```

## Links
* https://cwrap.org/nss_wrapper.html

## Flag
```
hexCTF{wh0_put5_k1ll_sw1tch3s_in_the1r_m4lw4r3}
```
