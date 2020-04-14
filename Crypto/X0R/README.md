# Crypto - X0R

## Challenge
XOR is best method for OTPs, especially for flags.

Author: Yarin
[x0r.zip](x0r.zip)

## Solution
Inside `x0r.zip` we will find two files, `flag.enc` with encrypted flag and
`enc.py`. Based on the logic of `enc.py` we can find out how flag was encrypted.

- key was randomly selected from `ascii_letters`
- key size was between 8-16 characters
- encryption is a simple XOR

To revert this operation we need to XOR again: `flag.enc` ^ key.

First 7 key characters are easy to find, because we know flag will starts with
as: `hexCTF{`. This will give us key: `JtmZzCJ`.

Another information we know, the flag needs to end with `}`.

With some experiments we can notice they're only 2 key lengths ending with `}`
within a range of 8-16 characters - 9 and 12 characters.

Starting with 9 characters. We know first 7 so we just need to bruteforce last 2.

With excluding all not possible combinations we end up with something like:
```
[...]
hexCTF{QbpercaliDeagilistKtexpialiFxcious}
hexCTF{QcpercaliDdagilistKuexpialiFycious}
hexCTF{RbpercaliGeagilistHtexpialiExcious}
hexCTF{RcpercaliGdagilistHuexpialiEycious}
hexCTF{SbpercaliFeagilistItexpialiDxcious}
hexCTF{ScpercaliFdagilistIuexpialiDycious}
hexCTF{TbpercaliAeagilistNtexpialiCxcious}
hexCTF{TcpercaliAdagilistNuexpialiCycious}
hexCTF{VbpercaliCeagilistLtexpialiAxcious}
hexCTF{VcpercaliCdagilistLuexpialiAycious}
```

If we search in Google we will be corrected to `supercalifragilisticexpialidocious`.

```
$ python dec.py | grep supercalifragilisticexpialidocious
hexCTF{supercalifragilisticexpialidocious}
```

## Flag
```
hexCTF{supercalifragilisticexpialidocious}
```
