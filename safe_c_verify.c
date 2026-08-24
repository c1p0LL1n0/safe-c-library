#define SAFE_C_STDIO
#include "safe_c.c"
#include <stdio.h>
#include <string.h>

static int errs = 0, tnum = 0;

#define T(cond, lbl) do { tnum++; \
    if (!(cond)) { printf("FAIL T%d: %s\n", tnum, lbl); errs++; } \
} while(0)

int main(void)
{
    printf("========== SAFE_C AUTO-VERIFY ==========\n\n");

    /* ---- strcmp ---- */
    {
        t_safe_str a = MAKE_SAFE_STR(32);
        ft_strcpy_safe(&a, "Hello");
        T(ft_strcmp_safe(&a, "Hello") == 0, "strcmp eq");
        T(ft_strcmp_safe(&a, "Hell") > 0, "strcmp gt shorter");
        T(ft_strcmp_safe(&a, "Hello!") < 0, "strcmp lt longer");
        T(ft_strcmp_safe(&a, "HelloWorld") < 0, "strcmp lt much longer");

        ft_strcpy_safe(&a, "");
        T(ft_strcmp_safe(&a, "") == 0, "strcmp empty eq");
        T(ft_strcmp_safe(&a, "X") < 0, "strcmp empty lt X");

        t_safe_str t = MAKE_SAFE_STR(5);
        ft_strcpy_safe(&t, "HellXam");
        T(t.len == 4, "truncated len=4");
        T(ft_strcmp_safe(&t, "HellX") < 0, "truncated Hell vs HellX");
        T(ft_strcmp_safe(&t, "Hell") == 0, "truncated Hell eq Hell");
/* strcmp: minimal buffer */
    }
/* ---- strncmp ---- */
    {
        t_safe_str a = MAKE_SAFE_STR(32);
        ft_strcpy_safe(&a, "Hello");
        T(ft_strncmp_safe(&a, "Hello", 0) == 0, "strncmp n=0");
        T(ft_strncmp_safe(&a, "Hello", -5) == 0, "strncmp n<0");
        T(ft_strncmp_safe(&a, "HeXYZ", 2) == 0, "strncmp partial eq");
        T(ft_strncmp_safe(&a, "HeXYZ", 3) > 0, "strncmp Hel vs HeX");
        T(ft_strncmp_safe(&a, "HdXXX", 2) > 0, "strncmp partial gt");
    }

    /* ---- strchr/strrchr ---- */
    {
        t_safe_str a = MAKE_SAFE_STR(32);
        ft_strcpy_safe(&a, "abcabc");
        T(ft_strchr_safe(&a, 'a') == &a.ptr[0], "strchr first");
        T(ft_strchr_safe(&a, 'c') == &a.ptr[2], "strchr mid");
        T(ft_strchr_safe(&a, 'z') == NULL, "strchr not found");
        T(ft_strchr_safe(&a, '\0') == &a.ptr[6], "strchr nul");

        ft_strcpy_safe(&a, "");
        T(ft_strchr_safe(&a, '\0') == &a.ptr[0], "strchr nul empty");
        T(ft_strchr_safe(&a, 'A') == NULL, "strchr not found empty");

        ft_strcpy_safe(&a, "abcabc");
        T(ft_strrchr_safe(&a, 'a') == &a.ptr[3], "strrchr last");
        T(ft_strrchr_safe(&a, '\0') == &a.ptr[6], "strrchr nul");
    }

    /* ---- strstr ---- */
    {
        t_safe_str a = MAKE_SAFE_STR(32);
        ft_strcpy_safe(&a, "hello world");
        T(ft_strstr_safe(&a, "") == a.ptr, "strstr empty needle");
        T(ft_strstr_safe(&a, "world") == &a.ptr[6], "strstr found");
        T(ft_strstr_safe(&a, "xyz") == NULL, "strstr not found");
        T(ft_strstr_safe(&a, "hello world!") == NULL, "strstr too long");
        T(ft_strstr_safe(&a, "hello world") == a.ptr, "strstr full match");
    }

    /* ---- substr ---- */
    {
        t_safe_str src = MAKE_SAFE_STR(32);
        t_safe_str dst = MAKE_SAFE_STR(16);
        ft_strcpy_safe(&src, "abcdef");

        ft_substr_safe(&dst, &src, 0, 3);
        T(ft_strcmp_safe(&dst, "abc") == 0, "substr(0,3)=abc");

        ft_substr_safe(&dst, &src, 3, 99);
        T(ft_strcmp_safe(&dst, "def") == 0, "substr(3,99)=def");

        ft_substr_safe(&dst, &src, 10, 3);
        T(dst.len == 0, "substr out of range empty");

        ft_substr_safe(&dst, &src, -1, 3);
        T(dst.len == 0, "substr neg start");

        ft_substr_safe(&dst, &src, 0, 0);
        T(dst.len == 0, "substr len=0");

        t_safe_str small = MAKE_SAFE_STR(3);
        ft_substr_safe(&small, &src, 0, 99);
        T(small.len == 2, "substr truncated to dest");
        T(ft_strcmp_safe(&small, "ab") == 0, "substr truncated ab");
    }

/* ---- strcat/strlcat ---- */
    {
        t_safe_str a = MAKE_SAFE_STR(10);
        ft_strcpy_safe(&a, "Hi");
        ft_strcat_safe(&a, "There");
        T(a.len == 7, "strcat len");
        T(ft_strcmp_safe(&a, "HiThere") == 0, "strcat result");

        t_safe_str b = MAKE_SAFE_STR(6);
        ft_strcpy_safe(&b, "ABCDE");
        ft_strcat_safe(&b, "FGH");
        T(b.len == 5, "strcat full buf");
        T(ft_strcmp_safe(&b, "ABCDE") == 0, "strcat full unchanged");

        ft_strcpy_safe(&a, "Base");
        ft_strlcat_safe(&a, "Append", 0);
        T(ft_strcmp_safe(&a, "Base") == 0, "strlcat n=0");
        ft_strlcat_safe(&a, "X", -1);
        T(ft_strcmp_safe(&a, "Base") == 0, "strlcat n<0");
    }

    /* ---- strjoin ---- */
    {
        t_safe_str d = MAKE_SAFE_STR(32);
        ft_strjoin_safe(&d, "A", "B");
        T(ft_strcmp_safe(&d, "AB") == 0, "join A+B");
        ft_strjoin_safe(&d, NULL, "B");
        T(ft_strcmp_safe(&d, "B") == 0, "join NULL+B");
        ft_strjoin_safe(&d, "A", NULL);
        T(ft_strcmp_safe(&d, "A") == 0, "join A+NULL");
        ft_strjoin_safe(&d, NULL, NULL);
        T(d.len == 0, "join NULL+NULL");
    }
/* itoa: zero in minimal buffer */
        {
            t_safe_str z = MAKE_SAFE_STR(2);
            ft_itoa_safe(0, &z);
            T(ft_strcmp_safe(&z, "0") == 0, "itoa 0 in buf[2]");
        }
        {
            t_safe_str z = MAKE_SAFE_STR(1);
            ft_itoa_safe(0, &z);
            T(z.len == 0, "itoa 0 in buf[1] cannot fit");
        }

    /* ---- itoa ---- */
    {
        t_safe_str n = MAKE_SAFE_STR(12);
        ft_itoa_safe(0, &n);
        T(ft_strcmp_safe(&n, "0") == 0, "itoa 0");
        ft_itoa_safe(-1, &n);
        T(ft_strcmp_safe(&n, "-1") == 0, "itoa -1");
        ft_itoa_safe(-2147483648, &n);
        T(ft_strcmp_safe(&n, "-2147483648") == 0, "itoa INT_MIN");

        t_safe_str t = MAKE_SAFE_STR(3);
        ft_itoa_safe(12345, &t);
        T(t.len == 2 && ft_strcmp_safe(&t, "12") == 0, "itoa trunc");

        t_safe_str t2 = MAKE_SAFE_STR(2);
        ft_itoa_safe(42, &t2);
        T(t2.len == 1 && ft_strcmp_safe(&t2, "4") == 0, "itoa tiny");
    }

    /* ---- trim ---- */
    {
        t_safe_str src = MAKE_SAFE_STR(32);
        t_safe_str dst = MAKE_SAFE_STR(32);
        ft_strcpy_safe(&src, "  hello  ");
        ft_strtrim_safe(&dst, &src, NULL);
        T(ft_strcmp_safe(&dst, "hello") == 0, "trim spaces");
        ft_strcpy_safe(&src, "   ");
        ft_strtrim_safe(&dst, &src, NULL);
        T(dst.len == 0, "trim all spaces");
    }

    /* ---- memset ---- */
    {
        t_safe_str m = MAKE_SAFE_STR(8);
        ft_memset_safe(&m, 'A', 4);
        T(m.len == 4 && m.ptr[0]=='A' && m.ptr[3]=='A' && m.ptr[4]=='\0', "memset");
        ft_memset_safe(&m, 'B', 0);
        T(m.ptr[0] == 'A', "memset n=0 noop");
/* ---- memset deeper check ---- */
    {
        t_safe_str m = MAKE_SAFE_STR(8);
        ft_memset_safe(&m, 'Z', 0);
        T(m.len == 0, "memset n=0 len untouched");
        T(m.ptr[0] == '\0', "memset n=0 data untouched");
    }
        ft_memset_safe(&m, 'C', 8);
        T(m.len == 7 && m.ptr[6]=='C' && m.ptr[7]=='\0', "memset n=8");
    }

    /* ---- memcpy/memmove ---- */
    {
        t_safe_str m = MAKE_SAFE_STR(16);
        ft_strcpy_safe(&m, "SAVED");
        ft_memcpy_safe(&m, "CHANGE", 0);
        T(ft_strcmp_safe(&m, "SAVED") == 0, "memcpy n=0");
        ft_strcpy_safe(&m, "ABCDEFG");
        ft_memmove_safe(&m, m.ptr + 2, 4);
        T(ft_strcmp_safe(&m, "CDEF") == 0, "memmove overlap");
    }

    /* ---- overflow buf[1] ---- */
    {
        t_safe_str t = MAKE_SAFE_STR(1);
        ft_strcpy_safe(&t, "ABC");
        T(t.len == 0 && t.ptr[0] == '\0', "buf[1] overflow");
    }

    /* ---- NULL ---- */
    {
        t_safe_str a = MAKE_SAFE_STR(16);
        T(ft_strcpy_safe(NULL, "x") == NULL, "NULL strcpy");
        T(ft_strcat_safe(NULL, "x") == NULL, "NULL strcat");
        T(ft_strcmp_safe(NULL, "x") == -1, "NULL strcmp");
        T(ft_strchr_safe(NULL, 'A') == NULL, "NULL strchr");
        T(ft_strlen_safe(NULL) == 0, "NULL strlen");
    }

    if (errs)
        printf("\n========== %d FAILURES / %d TESTS ==========\n", errs, tnum);
    else
        printf("\n========== ALL %d TESTS PASSED ==========\n", tnum);
    return (errs);
}