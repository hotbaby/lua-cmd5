/*************************************************************************
  > File Name: md5.c
  > Author: yy
  > Mail: mengyy_linux@163.com
 ************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <openssl/md5.h>


void do_fp(FILE *f, char **mdsum);
void pt(unsigned char *md);

int main(int argc, char **argv)
{

    const char *path = argv[1];
    FILE *fp = NULL;

    char *md5sum = malloc(128);

    fp=fopen(path, "r");
    do_fp(fp, &md5sum);
    fclose(fp);

    fprintf(stderr, "%s\n", md5sum);

    return 0;
}

void do_fp(FILE *f, char **md5sum)
{
#define BUFSIZE     16*1024

    MD5_CTX ctx;
    unsigned char md[MD5_DIGEST_LENGTH];
    int fd;
    ssize_t n;
    unsigned char buf[BUFSIZE];

    fd=fileno(f);
    MD5_Init(&ctx);
    for (;;)
    {
        n = read(fd, buf, BUFSIZE);
        if (n <= 0)
        {
            break;
        }
        MD5_Update(&ctx, buf, (unsigned long)n);
    }
    MD5_Final(&(md[0]), &ctx);

    int i;
    char *s = *md5sum;
    for (i = 0; i < MD5_DIGEST_LENGTH; i++)
    {
        sprintf(s, "%02x",md[i]);
        s += 2;
    }
    //pt(md);
}

void pt(unsigned char *md)
{
    int i;

    for (i = 0; i < MD5_DIGEST_LENGTH; i++)
    {
        printf("%02x",md[i]);
    }
    printf("\n");
}
