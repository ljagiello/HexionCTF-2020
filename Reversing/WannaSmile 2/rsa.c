#include <openssl/pem.h>
#include <openssl/ssl.h>
#include <openssl/rsa.h>
#include <openssl/evp.h>
#include <openssl/bio.h>
#include <openssl/err.h>
#include <stdio.h>

int padding = RSA_PKCS1_PADDING;

RSA * createRSA(unsigned char * key,int public)
{
  RSA *rsa= NULL;
  BIO *keybio ;
  keybio = BIO_new_mem_buf(key, -1);
  if (keybio==NULL)
  {
      printf( "Failed to create key BIO");
      return 0;
  }
  if(public)
  {
      rsa = PEM_read_bio_RSA_PUBKEY(keybio, &rsa,NULL, NULL);
  }
  else
  {
      rsa = PEM_read_bio_RSAPrivateKey(keybio, &rsa,NULL, NULL);
  }
  if(rsa == NULL)
  {
      printf( "Failed to create RSA");
  }

  return rsa;
}

int private_decrypt(unsigned char * enc_data,int data_len,unsigned char * key, unsigned char *decrypted)
{
  RSA * rsa = createRSA(key,0);
  int  result = RSA_private_decrypt(data_len,enc_data,decrypted,rsa,padding);
  return result;
}

void printLastError(char *msg)
{
  char * err = malloc(256);;
  ERR_load_crypto_strings();
  ERR_error_string(ERR_get_error(), err);
  printf("%s ERROR: %s\n",msg, err);
  free(err);
}

int main(){

  char privateKey[]="-----BEGIN RSA PRIVATE KEY-----\n"\
"MIIEowIBAAKCAQEAujCxmc9GU2RHVWRgQKhHkU+vrD35XbAbWIGFu+a5mghCvLWc\n"\
"aCm0HDW7zU8t8InSXkCa03mUrZLtERl00JviFEdREYpV3ATpKM9CG7WCMmCvT7+k\n"\
"j9ReKhqNwmJTfYxf/yEN3STMtl8d2UUDAXLDX6RMijuVF4r1hhHJcmzbQsfi4SzL\n"\
"xmBkTEdr+0P+8WyLGb6eI1NsCPhLw5ctma2Sf/iAuN1VZ4EFrhP8DMQR683Ha0w5\n"\
"zllh6Olz15d6AoMtOAKpt1/d+oebmimnweUdoUji3M09g0RlzcbkcjGdydFmkTfl\n"\
"RfXruneYwx/1ajbkzOIKSYtvsQ68qfutEv5UYwIDAQABAoIBAHV45YUavs88lLku\n"\
"xH/m7IilEzyEOYI/63Dy/3KnRvEFRz4TA6TAsy+dQ2ltDJoYCs+GyWil7AV7jvFb\n"\
"81TvFTwgJuj/M97GhmlGIvqikYvBbrrNOkG5BtRfvpMnQHAVEjhBHaMqZbKm3olH\n"\
"vna6dsQwQE5y+0VIWlKFhGkaf1ho+HlghrTs1hp8q53LjYOkSzIiLC2llSqX+TJI\n"\
"4likIe3SJc9I3tvBcNXjKE4lDtJv8UVvvOX870Okaiz4gcezghWKAP0jlBnjGt4h\n"\
"aNPyCLN2QN5AsF6cL+VN6vKQqiTP6IO/Z27RvmZpFnZCS4lCdxIWVQplZSAQvhZr\n"\
"S3vlU0kCgYEA/fP/Gum81jsZILHYQyHXD7c+mpDMsF9q8UcEgJWhsFwl7gpmXhKF\n"\
"UkbbaAZ41LdRLEMULN9lrj9KsZgUSL+q1dbgg58QfeWeT23GrBryOLc4+73O2tDY\n"\
"GLKlm1iDbzUcH90rt9N/l1P1mchum5yhPgce9s/Vxd+FsT+JrdKa2E0CgYEAu7Dg\n"\
"TOIwZGkkAPChuzvgGwQc5zyB2W9JMIu6f/vjv5sO9ULml7YPNKLl5lQb3tLClLKE\n"\
"mLH+km5Ktt9aSegxFMYxHBOjbPNHiQzNucziFld/gnLxZH2VecqAztu1eIaRLFsO\n"\
"RN+M08Ci5AIcgNELopihwRWoOBIlxIm1YGPqN28CgYEAkz6ZCJLD8fccL6/Oc8Y4\n"\
"Ox2Fh/szI4R/rlv+CUw5DtevCmEvT5YvhTAeYSd3gea0gxR1qzrwauK5vVUvVasE\n"\
"M66+SLK+zjCH5QgCbKHHFgluWdCJEqiK028pxHPC6YUCxJrpkVFpJmYA/mSPdVyQ\n"\
"xTU9x99j1LIwg0AS5R06uPUCgYBbwCJV0zh+zVr5TqhxI9rwufj7HPwI87rejV+1\n"\
"EsiJsN4GDRc2UJxsSbmC99jL+1F9aSsijdh5cor5jIPRnzzm/9+f7wWzy9VLbMPg\n"\
"sQRQkaNWfKP+UNnSOKjw9jxm17l96mE2T+XC2eeG+HVAj3BVorFn9xNNnalywzK0\n"\
"FQO45wKBgGSNstScPA8i1Jrqa0NaMMbRvP1ifhID/snDt3mfgybvD1GEUcNIQUDu\n"\
"1hUE/K5lMaijtcbsuvpW+9y8mL0ZTI3kk8voOV8VkJ8cMy8/RH7zRMC2lC+GU7es\n"\
"nGUfgUSTuxJ06sNCNF3AqGwRmaIrQ/e7qRBmzrcU3QmkXTBiaEL9\n"\
"-----END RSA PRIVATE KEY-----\n";

  unsigned char decrypted[33];

  FILE    *file;
  unsigned char *buffer;
  unsigned long fileLen, origFileLen;

  file = fopen("flag2.txt.rsa", "rb");

  if(file == NULL)
    return 1;

  fseek(file, 0, SEEK_END);
  fileLen=ftell(file);
  fseek(file, 0, SEEK_SET);

  buffer=(char *)malloc(fileLen+1);

  if(buffer == NULL)
    return 1;

  fread(buffer, fileLen, 1, file);
  fclose(file);

  int decrypted_length = private_decrypt(buffer,fileLen,privateKey, decrypted);
  if(decrypted_length == -1)
  {
    printLastError("Private Decrypt failed ");
  }
  if(decrypted_length != -1)
  {
    printf("rsa data size: %li\n", fileLen);
    printf("Decrypted Text = %s\n",decrypted);
    printf("Decrypted Length = %d\n",decrypted_length);
  }
}
