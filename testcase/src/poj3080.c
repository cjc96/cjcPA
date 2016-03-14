char input_buffer[] = 
"\x33\x0A\x32\x0A\x47\x41\x54\x41\x43\x43"
"\x41\x47\x41\x54\x41\x43\x43\x41\x47\x41"
"\x54\x41\x43\x43\x41\x47\x41\x54\x41\x43"
"\x43\x41\x47\x41\x54\x41\x43\x43\x41\x47"
"\x41\x54\x41\x43\x43\x41\x47\x41\x54\x41"
"\x43\x43\x41\x47\x41\x54\x41\x43\x43\x41"
"\x47\x41\x54\x41\x0A\x41\x41\x41\x41\x41"
"\x41\x41\x41\x41\x41\x41\x41\x41\x41\x41"
"\x41\x41\x41\x41\x41\x41\x41\x41\x41\x41"
"\x41\x41\x41\x41\x41\x41\x41\x41\x41\x41"
"\x41\x41\x41\x41\x41\x41\x41\x41\x41\x41"
"\x41\x41\x41\x41\x41\x41\x41\x41\x41\x41"
"\x41\x41\x41\x41\x41\x0A\x33\x0A\x47\x41"
"\x54\x41\x43\x43\x41\x47\x41\x54\x41\x43"
"\x43\x41\x47\x41\x54\x41\x43\x43\x41\x47"
"\x41\x54\x41\x43\x43\x41\x47\x41\x54\x41"
"\x43\x43\x41\x47\x41\x54\x41\x43\x43\x41"
"\x47\x41\x54\x41\x43\x43\x41\x47\x41\x54"
"\x41\x43\x43\x41\x47\x41\x54\x41\x0A\x47"
"\x41\x54\x41\x43\x54\x41\x47\x41\x54\x41"
"\x43\x54\x41\x47\x41\x54\x41\x43\x54\x41"
"\x47\x41\x54\x41\x43\x54\x41\x41\x41\x47"
"\x47\x41\x41\x41\x47\x47\x47\x41\x41\x41"
"\x41\x47\x47\x47\x47\x41\x41\x41\x41\x41"
"\x47\x47\x47\x47\x47\x41\x41\x41\x41\x0A"
"\x47\x41\x54\x41\x43\x43\x41\x47\x41\x54"
"\x41\x43\x43\x41\x47\x41\x54\x41\x43\x43"
"\x41\x47\x41\x54\x41\x43\x43\x41\x41\x41"
"\x47\x47\x41\x41\x41\x47\x47\x47\x41\x41"
"\x41\x41\x47\x47\x47\x47\x41\x41\x41\x41"
"\x41\x47\x47\x47\x47\x47\x41\x41\x41\x41"
"\x0A\x33\x0A\x43\x41\x54\x43\x41\x54\x43"
"\x41\x54\x43\x43\x43\x43\x43\x43\x43\x43"
"\x43\x43\x43\x43\x43\x43\x43\x43\x43\x43"
"\x43\x43\x43\x43\x43\x43\x43\x43\x43\x43"
"\x43\x43\x43\x43\x43\x43\x43\x43\x43\x43"
"\x43\x43\x43\x43\x43\x43\x43\x43\x43\x43"
"\x43\x43\x43\x0A\x41\x43\x41\x54\x43\x41"
"\x54\x43\x41\x54\x41\x41\x41\x41\x41\x41"
"\x41\x41\x41\x41\x41\x41\x41\x41\x41\x41"
"\x41\x41\x41\x41\x41\x41\x41\x41\x41\x41"
"\x41\x41\x41\x41\x41\x41\x41\x41\x41\x41"
"\x41\x41\x41\x41\x41\x41\x41\x41\x41\x41"
"\x41\x41\x41\x41\x0A\x41\x41\x43\x41\x54"
"\x43\x41\x54\x43\x41\x54\x54\x54\x54\x54"
"\x54\x54\x54\x54\x54\x54\x54\x54\x54\x54"
"\x54\x54\x54\x54\x54\x54\x54\x54\x54\x54"
"\x54\x54\x54\x54\x54\x54\x54\x54\x54\x54"
"\x54\x54\x54\x54\x54\x54\x54\x54\x54\x54"
"\x54\x54\x54\x54\x54\x0A"
;

char answer_buffer[] = 
"\x6E\x6F\x20\x73\x69\x67\x6E\x69\x66\x69"
"\x63\x61\x6E\x74\x20\x63\x6F\x6D\x6D\x6F"
"\x6E\x61\x6C\x69\x74\x69\x65\x73\x0A\x41"
"\x47\x41\x54\x41\x43\x0A\x43\x41\x54\x43"
"\x41\x54\x43\x41\x54\x0A"
;

#include "trap.h"
#include <stdarg.h>
#include <string.h>

/* TEMPLATE CODE */

char output_buffer[sizeof(answer_buffer) + 0x100];


/* only usable when a >= 0 */
void LLdiv10(unsigned long long a, unsigned long long *q, int *r)
{
    union {
        long long value;
        unsigned short s[4];
    } LL;
    
    int i;
    unsigned int cur, sq, sr = 0;
    LL.value = a;

    for (i = 3; i >= 0; i--) {
        cur = sr * 0x10000 + LL.s[i];
        sq = cur / 10;
        sr = cur % 10;
        LL.s[i] = sq;
    }

    *q = LL.value;
    *r = sr;
}

/* useful functions */

int naive_isdigit(char c)
{
    return '0' <= c && c <= '9';
}

int naive_isspace(char c)
{
    switch (c) {
        case ' ': case '\f': case '\n': case '\r': case '\t': case '\v':
            return 1;
        default:
            return 0;
    }
}

/* write_TYPE(): print TYPE to output buffer
 * return outputed char cnt
 */

char *write_ptr = output_buffer;

void finish_output()
{
    *write_ptr = '\0';
}

int write_char(char c)
{
    *write_ptr++ = c;
    return 1;
}

int write_string(char *s)
{
    int cnt = 0;
    while (*s) cnt += write_char(*s++);
    return cnt;
}

int write_llint(long long lld)
{
    int buf[100];
    int p = 0, ret;
    unsigned long long llu;
    if (lld < 0) { write_char('-'); lld = -lld; }
    llu = lld;
    do {
        LLdiv10(llu, &llu, &    buf[p++]);
    } while (llu > 0);
    ret = p;
    while (p > 0) write_char('0' + buf[--p]);
    return ret;
}

int write_int(int d)
{
    char buf[100];
    int p = 0, ret;
    unsigned int u;
    if (d < 0) { write_char('-'); d = -d; }
    u = d;
    do {
        buf[p++] = u % 10;
        u /= 10;
    } while (u > 0);
    ret = p;
    while (p > 0) write_char('0' + buf[--p]);
    return ret;
}

int naive_printf(const char *fmt, ...)
{
    va_list ap;
    long long lld;
    int d;
    char c, *s;
    int cnt = 0;
    
    va_start(ap, fmt);
    while (*fmt) {
        if (*fmt == '%') {
            switch (*++fmt) {
                case 's':
                    s = va_arg(ap, char *);
                    cnt += write_string(s);
                    break;
                case 'd':
                    d = va_arg(ap, int);
                    cnt += write_int(d);
                    break;
                case 'c':
                    c = va_arg(ap, int);
                    cnt += write_char(c);
                    break;
                case 'l':
                    if (*++fmt == 'l' && *++fmt == 'd') {
                        lld = va_arg(ap, long long);
                        cnt += write_llint(lld);
                    }
                    break;
                case '%':
                    cnt += write_char('%');
                    break;
            }
        } else {
            cnt += write_char(*fmt);
        }
        fmt++;
    }
    va_end(ap);
    return cnt;
}


/* read_TYPE(): read TYPE from input buffer
 * return read object count
 */

char *read_ptr = input_buffer;

int read_char(char *cp)
{
    if (*read_ptr) {
        *cp = *read_ptr++;
        return 1;
    } else {
        return 0;
    }
}

void unread_char(char c)
{
    *--read_ptr = c;
}

void read_space()
{
    char c;
    while (read_char(&c) && naive_isspace(c));
    unread_char(c);
}

int read_string(char *s)
{
    int flag = 0;
    char c;
    read_space();
    while (read_char(&c) && !naive_isspace(c)) {
        *s++ = c;
        flag = 1;
    }
    unread_char(c);
    if (flag) *s = '\0';
    return flag;
}

int read_llint(long long *lldp)
{
    int flag = 0;
    long long lld = 0;
    int f = 0;
    char c;
    read_space();
    read_char(&c);
    if (c == '-') f = 1; else unread_char(c);
    while (read_char(&c) && naive_isdigit(c)) {
        lld = lld * 10 + (c - '0');
        flag = 1;
    }
    unread_char(c);
    if (flag) *lldp = f ? -lld : lld;
    if (!flag && f) unread_char('-');
    return flag;
}

int read_int(int *dp)
{
    int flag = 0;
    int d = 0;
    int f = 0;
    char c;
    read_space();
    read_char(&c);
    if (c == '-') f = 1; else unread_char(c);
    while (read_char(&c) && naive_isdigit(c)) {
        d = d * 10 + (c - '0');
        flag = 1;
    }
    unread_char(c);
    if (flag) *dp = f ? -d : d;
    if (!flag && f) unread_char('-');
    return flag;
}

int naive_scanf(const char *fmt, ...)
{
    va_list ap;
    long long *lldp;
    int *dp;
    char c, *cp, *s;
    int cnt = 0;
    int flag = 0;
    
    va_start(ap, fmt);
    while (*fmt) {
        if (*fmt == '%') {
            switch (*++fmt) {
                case 's':
                    s = va_arg(ap, char *);
                    cnt += flag = read_string(s);
                    break;
                case 'd':
                    dp = va_arg(ap, int *);
                    cnt += flag = read_int(dp);
                    break;
                case 'c':
                    cp = va_arg(ap, char *);
                    cnt += flag = read_char(cp);
                    break;
                case 'l':
                    if (*++fmt == 'l' && *++fmt == 'd') {
                        lldp = va_arg(ap, long long *);
                        cnt += flag = read_llint(lldp);
                    }
                    break;
                case '%':
                    read_char(&c);
                    flag = (c == '%');
                    break;
            }
        } else {
            read_char(&c);
            flag = (c == *fmt);
        }
        if (!flag) goto done;
        fmt++;
    }
done:
    va_end(ap);
    return cnt;
}



int naive_memcmp(void *p1, void *p2, int n)
{
    unsigned char *s1 = p1, *s2 = p2;
    int i;
    for (i = 0; i < n; i++)
        if (s1[i] < s2[i])
            return -1;
        else if (s1[i] > s2[i])
            return 1;
    return 0;
}

void naive_memset(void *p, char c, int n)
{
    unsigned char *s = p;
    int i;
    for (i = 0; i < n; i++)
        s[i] = c;
}








int program_main();

#define FILLCHAR 'E'

int main()
{
    naive_memset(output_buffer, FILLCHAR, sizeof(output_buffer));    
    program_main();
    finish_output();
    nemu_assert(naive_memcmp(output_buffer, answer_buffer, sizeof(answer_buffer)) == 0);
    nemu_assert(memcmp(output_buffer, answer_buffer, sizeof(answer_buffer)) == 0);
    HIT_GOOD_TRAP;
    return 0;
}

#define main program_main
#define scanf naive_scanf
#define printf naive_printf
#define puts(str) naive_printf("%s\n", (str))
#define putchar(ch) naive_printf("%c", (ch))

/* REAL USER PROGRAM */

#include <stdlib.h>
#include <string.h>
#define N 60
#define MAXM 10
char s[MAXM][N + 1];
char buf[N * N][N + 1];
char *t[N * N];

int cmp(const void *a, const void *b)
{
    return strcmp(*(char **)a, *(char **)b);
}

void solve()
{
    int M;
    int i, j, l;
    scanf("%d", &M);
    for (i = 0; i < M; i++)
        scanf("%s", s[i]);
    int sz = 0;
    for (i = 0; i < N; i++)
        for (j = i + 3; j <= N; j++) {
            char bu = s[0][j];
            s[0][j] = 0;
            strcpy(t[sz++], s[0] + i);
            s[0][j] = bu;
        }
    qsort(t, sz, sizeof(char *), cmp);
    for (l = N; l >= 3; l--) {
        for (i = 0; i < sz; i++) {
            if (strlen(t[i]) != l) continue;
            for (j = 0; j < M; j++)
                if (!strstr(s[j], t[i])) break;
            if (j >= M) {
                puts(t[i]);
                return;
            }
        }
    }
    puts("no significant commonalities");
}
int main()
{
    int i;
    for (i = 0; i < N * N; i++)
        t[i] = buf[i];
    int T;
    scanf("%d", &T);
    while (T--) solve();
    return 0;
}