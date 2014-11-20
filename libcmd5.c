/*************************************************************************
> File Name: gworker.c
> Author: yy
> Mail: mengyy_linux@163.com
************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <openssl/md5.h>

#include <lauxlib.h>
#include <lua.h>

#define TRUE            (1)
#define FALSE           (0)
#define MODULE_NAME     ("cmd5")
#define WORKER_META     ("cmd5.meta")

typedef unsigned int bool;

static char g_md5sum[128] = {0};

static int cmd5_lua_gc(lua_State *L)
{
    return 0;
}

static bool do_caculate_file_md5sum(const char* path)
{
#define BUFSIZE     (16*1024)

    int fd;
    ssize_t n;
    MD5_CTX ctx;
    unsigned char md[MD5_DIGEST_LENGTH];
    unsigned char buf[BUFSIZE];

    if (path == NULL)
    {
        fprintf(stderr, "param error.\n");
        return FALSE;
    }

    fd = open(path, O_RDONLY);
    if (fd < 0)
    {
        perror("open");
        return FALSE;
    }

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

    close(fd);
    fd = -1;

    int i = 0;
    char *s = g_md5sum;
    for (i = 0; i < MD5_DIGEST_LENGTH; i++)
    {
        sprintf(s, "%02x",md[i]);
        s += 2;
    }

    return TRUE;
}

static int cmd5_lua_file_sum(lua_State *L)
{
    bool ret;
    const char *path = NULL;

    fprintf(stderr, "call function %s\n", __func__);

    path = strdup(lua_tostring(L, 1));
    ret = do_caculate_file_md5sum(path);
    if (ret == TRUE)
    {
        lua_pushstring(L, g_md5sum);
        return 1;
    }
    else
    {
        return 0;
    }
}

static const luaL_Reg cmd5[] = {
    {"__gc",    cmd5_lua_gc},
    {"file_sum",     cmd5_lua_file_sum},
    {NULL, NULL}
};

int luaopen_cmd5(lua_State *L)
{
    /*create module */
    luaL_register(L, MODULE_NAME, cmd5);

    /* ceate metatable */
    luaL_newmetatable(L, WORKER_META);

    /* Fill metatable */
    luaL_register(L, NULL, cmd5);

    /* metatable.__index = metatable */
    lua_setfield(L, -2, "__index");

    return 0;
}
