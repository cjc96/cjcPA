char input_buffer[] = 
"\x32\x0A\x34\x20\x31\x20\x31\x0A\x31\x0A"
"\x32\x0A\x33\x20\x32\x20\x33\x0A\x31\x20"
"\x31\x0A\x31\x20\x31\x0A"
;

char answer_buffer[] = 
"\x53\x28\x31\x29\x3D\x31\x0A\x53\x28\x32"
"\x29\x3D\x32\x0A\x53\x28\x33\x29\x3D\x34"
"\x0A\x53\x28\x34\x29\x3D\x38\x0A\x61\x6E"
"\x73\x3D\x31\x35\x0A\x53\x28\x33\x29\x3D"
"\x32\x0A\x53\x28\x36\x29\x3D\x38\x0A\x53"
"\x28\x39\x29\x3D\x33\x34\x0A\x61\x6E\x73"
"\x3D\x34\x34\x0A"
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

typedef unsigned long long ULL;
typedef long long LL;

void ULLdivULL(ULL a, ULL b, ULL *qp, ULL *rp)
{
    ULL r = 0, q = 0;
    int i;
    for (i = 0; i < 64; i++) {
        r = (r << 1) + (a >> 63);
        a <<= 1;
        q <<= 1;
        if (r >= b) {
            r -= b;
            q += 1;
        }
    }
    if (qp) *qp = q;
    if (rp) *rp = r;
}

void LLdivLL(LL a, LL b, LL *qp, LL *rp)
{
    int qf = 0, rf = 0;
    if (a < 0) { qf = rf = 1; a = -a; }
    if (b < 0) { qf ^= 1; b = -b; }

    ULLdivULL(a, b, (ULL *) qp, (ULL *) rp);

    if (qp && qf) *qp = -*qp;
    if (rp && rf) *rp = -*rp;
}


#define MOD 1000000007
int S[100], A[100];
int N, R, K;
long long s(int n)
{
    if (n <= R) return S[n];
    long long ans = 0;
    int i;
    for (i = 1; i <= R; i++) {
        ans += A[i] * s(n - i);
        LLdivLL(ans, MOD, NULL, &ans);
    }
    return ans;
}
void solve()
{
    int i;
    scanf("%d%d%d", &N, &R, &K);
    for (i = 1; i <= R; i++)
        scanf("%d", &S[i]);
    for (i = 1; i <= R; i++)
        scanf("%d", &A[i]);
    
    long long ans = 0;
    long long cur;
    for (i = 1; i <= N; i++) {
        cur = s(i * K);
        printf("S(%d)=%d\n", i * K, (int) cur);
        ans += cur;
        LLdivLL(ans, MOD, NULL, &ans);
    }

    printf("ans=%d\n", (int) ans);
}
int main()
{
    int T;
    scanf("%d", &T);
    while (T--) solve();
    return 0;
}
