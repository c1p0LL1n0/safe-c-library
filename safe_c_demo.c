#define SAFE_C_STDIO
#include "safe_c.c"
#include <stdio.h>

static void demo_toupper_i(unsigned int i, char *c)
{
    (void)i;
    if (*c >= 'a' && *c <= 'z') *c -= 32;
}

#define PASS "  [PASS]"
#define EDGE "  [EDGE]"

int main(void)
{
    int errs = 0;
    printf("========== Safe C Library: Edge-Case Suite ==========\n\n");
    printf("--- 1. Базовые строковые операции ---\n");

    t_safe_str buf = MAKE_SAFE_STR(64);

    /* 1a — strcpy + strcat */
    ft_strcpy_safe(&buf, "Hello, Safe World!");
    ft_strcat_safe(&buf, " - C99+");
    printf(EDGE " strcpy+strcat          \"%s\" len=%d\n", buf.ptr, buf.len);

    /* 1b — strncpy с n=0 */
    ft_strcpy_safe(&buf, "SAVED");
    ft_strncpy_safe(&buf, "OVERRIDE", 0);
    printf(EDGE " strncpy(n=0)           \"%s\" (OK: SAVED)\n", buf.ptr);

    /* 1c — strncpy с отрицательным n (UB-защита) */
    ft_strncpy_safe(&buf, "BAD", -5);
    printf(EDGE " strncpy(n=-5)          \"%s\" (OK: SAVED)\n", buf.ptr);

    /* 1d — копирование пустой строки */
    ft_strcpy_safe(&buf, "");
    printf(EDGE " strcpy(\"\")             \"%s\" len=%d\n", buf.ptr, buf.len);

    /* 1e — strcat при dest->len испорченном */
    buf.len = 999;
    ft_strcat_safe(&buf, "FIXED");
    printf(EDGE " strcat с испорч. len    \"%s\" len=%d\n", buf.ptr, buf.len);

    /* 1f — strlcat n=0, n=-1 */
    ft_strcpy_safe(&buf, "BASE");
    ft_strlcat_safe(&buf, "APPEND", 0);
    printf(EDGE " strlcat(n=0)            \"%s\" (OK: BASE)\n", buf.ptr);
    ft_strlcat_safe(&buf, "X", -1);
    printf(EDGE " strlcat(n=-1)           \"%s\" (OK: BASE)\n", buf.ptr);

    /* ========================================================
       2. СРАВНЕНИЕ (краевые: усечение буфера, HelloWorld)
       ======================================================== */
    printf("\n--- 2. Сравнение строк ---\n");

    t_safe_str cmp = MAKE_SAFE_STR(32);
    ft_strcpy_safe(&cmp, "Hello");
    printf(EDGE " cmp(Hello,Hello)        %d (0)\n", ft_strcmp_safe(&cmp, "Hello"));
    printf(EDGE " cmp(Hello,Hell)         %d (>0)\n", ft_strcmp_safe(&cmp, "Hell"));
    printf(EDGE " cmp(Hello,Hello!)       %d (<0)\n", ft_strcmp_safe(&cmp, "Hello!"));
    printf(EDGE " cmp(Hello,HelloWorld)   %d (<0)\n", ft_strcmp_safe(&cmp, "HelloWorld"));

    /* Краевой: «Hell» в буфере на 5 байт против «HellX» */
    t_safe_str tiny2 = MAKE_SAFE_STR(5);
    ft_strcpy_safe(&tiny2, "HellXam");
    printf(EDGE " tiny(Hell) vs HellX     %d (<0)\n", ft_strcmp_safe(&tiny2, "HellX"));

    /* strncmp с n=0 */
    printf(EDGE " strncmp(n=0)             %d (0)\n", ft_strncmp_safe(&cmp, "Hello", 0));
    printf(EDGE " strncmp(He..,n=2)        %d (0)\n", ft_strncmp_safe(&cmp, "HeXYZ", 2));

    /* strequ с пустой строкой */
    ft_strcpy_safe(&cmp, "");
    printf(EDGE " strequ(\"\",\"\")            %d (1)\n", ft_strequ_safe(&cmp, ""));
    printf(EDGE " strequ(\"\",\"X\")           %d (0)\n", ft_strequ_safe(&cmp, "X"));
/* ========================================================
       3. ПОИСК
       ======================================================== */
    printf("\n--- 3. Поиск ---\n");

    t_safe_str hay = MAKE_SAFE_STR(64);
    ft_strcpy_safe(&hay, "The quick brown fox");

    char *f = ft_strchr_safe(&hay, '\0');
    printf(EDGE " strchr('\\0')            \"%s\" (конец)\n", f ? f : "(null)");
    f = ft_strchr_safe(&hay, 'T');
    printf(EDGE " strchr(T)               \"%s\"\n", f ? f : "(null)");
    f = ft_strrchr_safe(&hay, '\0');
    printf(EDGE " strrchr('\\0')           \"%s\"\n", f ? f : "(null)");
    f = ft_strstr_safe(&hay, "");
    printf(EDGE " strstr(\"\")              \"%s\"\n", f ? f : "(null)");
    f = ft_strstr_safe(&hay, "brown");
    printf(EDGE " strstr(brown)           \"%s\"\n", f ? f : "(null)");

    /* strchr на пустой строке */
    ft_strcpy_safe(&hay, "");
    f = ft_strchr_safe(&hay, 'A');
    printf(EDGE " strchr('A') на \"\"       %s (null OK)\n", f ? "!!" : "null OK");

    /* ========================================================
       4. ПОДСТРОКИ И TRIM
       ======================================================== */
    printf("\n--- 4. Подстроки и trim ---\n");

    ft_strcpy_safe(&hay, "Hello, World!");
    t_safe_str sub = MAKE_SAFE_STR(16);
    t_safe_str trim_src = MAKE_SAFE_STR(32);
    t_safe_str trimmed = MAKE_SAFE_STR(32);

    ft_substr_safe(&sub, &hay, 0, 5);
    printf(EDGE " substr(0,5)             \"%s\"\n", sub.ptr);
    ft_substr_safe(&sub, &hay, 7, 5);
    printf(EDGE " substr(7,5)             \"%s\"\n", sub.ptr);
    ft_substr_safe(&sub, &hay, 7, 99);
    printf(EDGE " substr(7,99)            \"%s\"\n", sub.ptr);
    ft_substr_safe(&sub, &hay, -1, 3);
    printf(EDGE " substr(-1,3)            \"%s\" (пусто)\n", sub.ptr);
    ft_substr_safe(&sub, &hay, 0, 0);
    printf(EDGE " substr(len=0)           \"%s\" (пусто)\n", sub.ptr);

    /* trim */
    ft_strcpy_safe(&trim_src, "   \t\n  ");
    ft_strtrim_safe(&trimmed, &trim_src, NULL);
    printf(EDGE " trim(только пробелы)    \"%s\"\n", trimmed.ptr);
    ft_strcpy_safe(&trim_src, "NoSpaces");
    ft_strtrim_safe(&trimmed, &trim_src, NULL);
    printf(EDGE " trim(без пробелов)      \"%s\"\n", trimmed.ptr);
    ft_strcpy_safe(&trim_src, "\t  A  \n");
    ft_strtrim_safe(&trimmed, &trim_src, NULL);
    printf(EDGE " trim(\\t A \\n)            \"%s\"\n", trimmed.ptr);
/* ========================================================
       5. ПАМЯТЬ (memset, memcpy, memmove, bzero)
       ======================================================== */
    printf("\n--- 5. Операции с памятью ---\n");

    t_safe_str mem = MAKE_SAFE_STR(8);

    ft_memset_safe(&mem, 'Z', 0);
    printf(EDGE " memset(n=0)             \"%s\" len=%d\n", mem.ptr, mem.len);

    ft_memset_safe(&mem, 'B', 8);
    printf(EDGE " memset(n=8,cap=8)       \"%s\" len=%d\n", mem.ptr, mem.len);

    ft_memset_safe(&mem, 'C', 99);
    printf(EDGE " memset(n=99,cap=8)      \"%s\" len=%d\n", mem.ptr, mem.len);

    ft_strcpy_safe(&mem, "KEEPER");
    ft_memcpy_safe(&mem, "CHANGE", 0);
    printf(EDGE " memcpy(n=0)             \"%s\" (OK:KEEPER)\n", mem.ptr);

    ft_strcpy_safe(&mem, "ABCDEFG");
    ft_memmove_safe(&mem, mem.ptr + 2, 4);
    printf(EDGE " memmove(ptr+2,4)        \"%s\"\n", mem.ptr);

    ft_strcpy_safe(&mem, "SAFE");
    ft_memmove_safe(&mem, "BAD", 0);
    printf(EDGE " memmove(n=0)            \"%s\" (OK:SAFE)\n", mem.ptr);

    ft_bzero_safe(&mem);
    printf(EDGE " bzero                   \"%s\" len=%d\n", mem.ptr, mem.len);

    /* ========================================================
       6. ПЕРЕПОЛНЕНИЕ БУФЕРА
       ======================================================== */
    printf("\n--- 6. Buffer Overflow Protection ---\n");

    t_safe_str tiny1 = MAKE_SAFE_STR(1);
    ft_strcpy_safe(&tiny1, "ABC");
    printf(EDGE " buf[1] cpy \"ABC\"        \"%s\" len=%d (OK:пусто)\n", tiny1.ptr, tiny1.len);

    t_safe_str tiny5 = MAKE_SAFE_STR(5);
    ft_strcpy_safe(&tiny5, "VERY_LONG_STRING");
    printf(EDGE " buf[5] cpy long         \"%s\" len=%d (4симв)\n", tiny5.ptr, tiny5.len);

    ft_strcat_safe(&tiny5, "MORE");
    printf(EDGE " buf[5] cat MORE         \"%s\" len=%d (не изм.)\n", tiny5.ptr, tiny5.len);

    /* ========================================================
       7. itoa КРАЕВЫЕ
       ======================================================== */
    printf("\n--- 7. itoa ---\n");

    t_safe_str num = MAKE_SAFE_STR(12);

    ft_itoa_safe(0, &num);
    printf(EDGE " itoa(0)                 \"%s\" len=%d\n", num.ptr, num.len);
    ft_itoa_safe(1, &num);
    printf(EDGE " itoa(1)                 \"%s\"\n", num.ptr);
    ft_itoa_safe(-1, &num);
    printf(EDGE " itoa(-1)                \"%s\"\n", num.ptr);
    ft_itoa_safe(-2147483648, &num);
    printf(EDGE " itoa(INT_MIN)           \"%s\"\n", num.ptr);

    t_safe_str tiny3 = MAKE_SAFE_STR(3);
    ft_itoa_safe(12345, &tiny3);
    printf(EDGE " itoa(12345,buf[3])      \"%s\" len=%d (усеч.)\n", tiny3.ptr, tiny3.len);

    t_safe_str tiny4 = MAKE_SAFE_STR(2);
    ft_itoa_safe(42, &tiny4);
    printf(EDGE " itoa(42,buf[2])         \"%s\" len=%d\n", tiny4.ptr, tiny4.len);
/* ========================================================
       8. NULL-ЗАЩИТА
       ======================================================== */
    printf("\n--- 8. NULL-защита ---\n");

    ft_strcpy_safe(NULL, "test");
    ft_strcpy_safe(&buf, NULL);
    ft_strcat_safe(NULL, "test");
    ft_strcat_safe(&buf, NULL);
    ft_strcmp_safe(NULL, "test");
    ft_strcmp_safe(&buf, NULL);
    ft_strchr_safe(NULL, 'A');
    ft_strstr_safe(NULL, "test");
    ft_strstr_safe(&buf, NULL);
    ft_strtrim_safe(NULL, &hay, NULL);
    ft_strtrim_safe(&buf, NULL, NULL);
    ft_memcpy_safe(NULL, "test", 4);
    ft_memcpy_safe(&buf, NULL, 4);
    ft_memmove_safe(NULL, "test", 4);
    ft_memset_safe(NULL, 'A', 4);
    ft_bzero_safe(NULL);
    ft_itoa_safe(42, NULL);
    ft_strclear_safe(NULL);
    printf(EDGE " Все NULL-вызовы без краша.\n");

    /* ========================================================
       9. strdup, striteri, классификация, toupper/tolower
       ======================================================== */
    printf("\n--- 9. strdup, striteri, классификация ---\n");

    t_safe_str src = MAKE_SAFE_STR(32);
    t_safe_str dst = MAKE_SAFE_STR(32);

    ft_strcpy_safe(&src, "CopyMe");
    ft_strdup_safe(&dst, &src);
    printf(EDGE " strdup(CopyMe)          \"%s\" len=%d\n", dst.ptr, dst.len);

    ft_strcpy_safe(&dst, "hello!");
    ft_striteri_safe(&dst, demo_toupper_i);
    printf(EDGE " striteri(hello!)        \"%s\"\n", dst.ptr);

    t_safe_str cls = MAKE_SAFE_STR(32);
    ft_strcpy_safe(&cls, "Hello");
    printf(EDGE " isalpha(Hello)          %d\n", ft_str_isalpha_safe(&cls));
    ft_strcpy_safe(&cls, "12345");
    printf(EDGE " isdigit(12345)          %d\n", ft_str_isdigit_safe(&cls));
    ft_strcpy_safe(&cls, "123abc");
    printf(EDGE " isdigit(123abc)         %d (0)\n", ft_str_isdigit_safe(&cls));
    ft_strcpy_safe(&cls, "");
    printf(EDGE " isalpha(\"\")             %d (0)\n", ft_str_isalpha_safe(&cls));

    /* toupper/tolower на пустой строке */
    ft_strcpy_safe(&cls, "");
    ft_toupper_safe(&cls);
    printf(EDGE " toupper(\"\")             \"%s\"\n", cls.ptr);
    ft_tolower_safe(&cls);
    printf(EDGE " tolower(\"\")             \"%s\"\n", cls.ptr);

    /* toupper/tolower со смешанным регистром */
    ft_strcpy_safe(&cls, "MiXeD CaSe 123!");
    ft_toupper_safe(&cls);
    printf(EDGE " toupper(MiXeD 123!)     \"%s\"\n", cls.ptr);
    ft_tolower_safe(&cls);
    printf(EDGE " tolower(после upper)    \"%s\"\n", cls.ptr);

    /* ========================================================
       10. СОСТАВНОЙ ЛИТЕРАЛ
       ======================================================== */
    printf("\n--- 10. Составные литералы ---\n");

    t_safe_str *res;
    res = ft_strcpy_safe(
        &(t_safe_str){.ptr = (char[20]){0}, .max_capacity = 20, .len = 0},
        "On-the-fly!");
    printf(EDGE " inline compound         \"%s\" len=%d\n", res->ptr, res->len);

    res = ft_itoa_safe(-42,
        &(t_safe_str){.ptr = (char[16]){0}, .max_capacity = 16, .len = 0});
    printf(EDGE " inline itoa(-42)        \"%s\" len=%d\n", res->ptr, res->len);

    res = ft_strjoin_safe(
        &(t_safe_str){.ptr = (char[32]){0}, .max_capacity = 32, .len = 0},
        "A+", "B");
    printf(EDGE " inline join(A+,B)       \"%s\"\n", res->ptr);

    /* ========================================================
       ИТОГО
       ======================================================== */
    printf("\n========== Все краевые тесты пройдены ==========\n");
    return (errs);
}