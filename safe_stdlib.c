/* ============================================================ */
/*   safe_stdlib.c -- Safe wrappers for C standard library         */
/*   Returns 1 on success, 0 on error. Never crashes on NULL.      */
/* ============================================================ */

#include "safe_c.c"
#include <stdlib.h>
#include <limits.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <stdarg.h>

/* ---------- safe integer arithmetic ---------- */

int ft_add_safe(int a, int b, int *res) {
    if (!res) return (0);
    if ((b > 0 && a > INT_MAX - b) || (b < 0 && a < INT_MIN - b))
        return (0);
    *res = a + b; return (1);
}

int ft_sub_safe(int a, int b, int *res) {
    if (!res) return (0);
    if ((b < 0 && a > INT_MAX + b) || (b > 0 && a < INT_MIN + b))
        return (0);
    *res = a - b; return (1);
}

int ft_mul_safe(int a, int b, int *res) {
    if (!res) return (0);
    if (a == 0 || b == 0) { *res = 0; return (1); }
    if (a > 0) {
        if (b > 0) { if (a > INT_MAX / b) return (0); }
        else      { if (b < INT_MIN / a) return (0); }
    } else {
        if (b > 0) { if (a < INT_MIN / b) return (0); }
        else      { if (a < INT_MAX / b) return (0); }
    }
    *res = a * b; return (1);
}

int ft_div_safe(int a, int b, int *res) {
    if (!res || b == 0) return (0);
    if (a == INT_MIN && b == -1) return (0);
    *res = a / b; return (1);
}

int ft_mod_safe(int a, int b, int *res) {
    if (!res || b == 0) return (0);
    if (a == INT_MIN && b == -1) return (0);
    *res = a % b; return (1);
}

/* ---------- safe string-to-number conversions ---------- */

int ft_atoi_safe(const char *str, int *res)
{
    long n; int sign;
    if (!str || !res) return (0);
    while (*str == ' ' || (*str >= '\t' && *str <= '\r')) str++;
    sign = 1;
    if (*str == '-' || *str == '+') {
        if (*str == '-') sign = -1;
        str++;
    }
    if (!ft_isdigit((unsigned char)*str)) return (0);
    n = 0;
    while (ft_isdigit((unsigned char)*str)) {
        n = n * 10 + (*str - '0');
        if ((sign == 1 && n > INT_MAX)
            || (sign == -1 && -n < INT_MIN))
            return (0);
        str++;
    }
    *res = (int)(n * sign);
    return (1);
}

int ft_strtoi_safe(const char *str, char **endptr, int *res)
{
    if (!str || !res) return (0);
    while (*str == ' ' || (*str >= '\t' && *str <= '\r')) str++;
    if (!ft_atoi_safe(str, res)) return (0);
    if (endptr) {
        while (*str == '-' || *str == '+'
            || ft_isdigit((unsigned char)*str)) str++;
        *endptr = (char *)str;
    }
    return (1);
}

int ft_atol_safe(const char *str, long *res)
{
    unsigned long n; int sign;
    if (!str || !res) return (0);
    while (*str == ' ' || (*str >= '\t' && *str <= '\r')) str++;
    sign = 1;
    if (*str == '-' || *str == '+') {
        if (*str == '-') sign = -1;
        str++;
    }
    if (!ft_isdigit((unsigned char)*str)) return (0);
    n = 0;
    while (ft_isdigit((unsigned char)*str)) {
        n = n * 10 + (*str - '0');
        if ((sign == 1 && n > (unsigned long)LONG_MAX)
            || (sign == -1 && n > (unsigned long)LONG_MAX + 1))
            return (0);
        str++;
    }
    *res = (long)(n * sign);
    return (1);
}
int ft_long_to_int_safe(long n, int *res) {
    if (!res) return (0);
    if (n > INT_MAX || n < INT_MIN) return (0);
    *res = (int)n; return (1);
}

/* ---------- safe dynamic vector ---------- */

typedef struct s_safe_vec {
    void *data;
    int elem_size;
    int capacity;
    int len;
    int error;
} t_safe_vec;

t_safe_vec ft_vec_new(int elem_size, int init_cap)
{
    t_safe_vec v;
    if (elem_size <= 0 || init_cap <= 0) {
        v.data = NULL; v.elem_size = 0; v.capacity = 0;
        v.len = 0; v.error = 2; return (v);
    }
    v.data = malloc((size_t)(elem_size * init_cap));
    if (!v.data) {
        v.elem_size = 0; v.capacity = 0;
        v.len = 0; v.error = 1; return (v);
    }
    v.elem_size = elem_size; v.capacity = init_cap;
    v.len = 0; v.error = 0;
    return (v);
}

void *ft_vec_get(t_safe_vec *v, int idx)
{
    if (!v || !v->data || idx < 0 || idx >= v->len) {
        if (v) v->error = 2;
        return (NULL);
    }
    return ((char *)v->data + idx * v->elem_size);
}

int ft_vec_push(t_safe_vec *v, const void *elem)
{
    void *nd; int nc;
    if (!v || !v->data || !elem) { if (v) v->error = 2; return (-1); }
    if (v->len >= v->capacity) {
        nc = v->capacity * 2;
        nd = malloc((size_t)(nc * v->elem_size));
        if (!nd) { v->error = 1; return (-1); }
        memcpy(nd, v->data, (size_t)(v->len * v->elem_size));
        free(v->data); v->data = nd; v->capacity = nc;
    }
    memcpy((char *)v->data + v->len * v->elem_size, elem,
           (size_t)v->elem_size);
    v->len++; v->error = 0;
    return (0);
}

int ft_vec_pop(t_safe_vec *v)
{
    if (!v || !v->data || v->len <= 0) { if (v) v->error = 2; return (-1); }
    v->len--; v->error = 0;
    return (0);
}

void ft_vec_free(t_safe_vec *v)
{
    if (!v || !v->data) return;
    free(v->data); v->data = NULL;
    v->capacity = 0; v->len = 0; v->error = 0;
}
/* ---------- safe file I/O ---------- */

FILE *ft_fopen_safe(const char *path, const char *mode)
{
    if (!path || !mode) return (NULL);
    return (fopen(path, mode));
}

int ft_fgets_safe(t_safe_str *dest, FILE *fp)
{
    int i, c;
    if (!dest || !dest->ptr || !fp) return (-1);
    i = 0;
    while (i < (dest->max_capacity - 1)) {
        c = fgetc(fp);
        if (c == EOF || c == '\n') break;
        dest->ptr[i++] = (char)c;
    }
    dest->ptr[i] = '\0'; dest->len = i;
    if (i == 0 && c == EOF) return (-1);
    return (i);
}

int ft_getline_safe(t_safe_str *dest)
{
    return (ft_fgets_safe(dest, stdin));
}

/* ---------- safe snprintf ---------- */

int ft_snprintf_safe(t_safe_str *dest, const char *fmt, ...)
{
    va_list ap; int n;
    if (!dest || !dest->ptr || !fmt) return (-1);
    va_start(ap, fmt);
    n = vsnprintf(dest->ptr, (size_t)dest->max_capacity, fmt, ap);
    va_end(ap);
    if (n < 0) return (-1);
    if (n >= dest->max_capacity) dest->len = dest->max_capacity - 1;
    else dest->len = n;
    dest->ptr[dest->len] = '\0';
    return (dest->len);
}

/* ---------- safe qsort ---------- */

int ft_qsort_safe(void *base, int nmemb, int size,
                   int (*compar)(const void *, const void *))
{
    if (!base || nmemb <= 0 || size <= 0 || !compar) return (-1);
    qsort(base, (size_t)nmemb, (size_t)size, compar);
    return (0);
}

/* ---------- safe malloc/calloc/free ---------- */

void *ft_malloc_safe(size_t size)
{
    if (size == 0) return (NULL);
    errno = 0; return (malloc(size));
}

void *ft_calloc_safe(size_t nmemb, size_t size)
{
    if (nmemb == 0 || size == 0) return (NULL);
    errno = 0; return (calloc(nmemb, size));
}

void ft_free_safe(void **ptr)
{
    if (!ptr || !*ptr) return;
    free(*ptr); *ptr = NULL;
}
