/* ============================================================
   Safe C Library — Полностью безопасная работа со строками
   ============================================================
   Все функции:
   - Проверяют указатели на NULL перед использованием
   - Никогда не выходят за пределы max_capacity буфера
   - Всегда ставят завершающий '\0'
   - Обновляют поле len в структуре
   - Не используют malloc — только стековые буферы
   ============================================================ */

#include <stddef.h>   /* NULL, size_t */

/* ---------- Структура безопасной строки ---------- */
typedef struct s_safe_str
{
    char        *ptr;           /* Указатель на массив памяти              */
    const int   max_capacity;   /* Жесткий предел буфера (только чтение)   */
    int         len;            /* Текущая заполненная длина строки        */
} t_safe_str;

/* ---------- Макрос-конструктор: безопасная строка на стеке ---------- */
#define MAKE_SAFE_STR(size) \
    (t_safe_str){ .ptr = (char[size]){0}, .max_capacity = size, .len = 0 }

/* ============================================================
   БАЗОВЫЕ СТРОКОВЫЕ ОПЕРАЦИИ
   ============================================================ */

/* Безопасная длина строки (через структуру, O(1)) */
static inline int   ft_strlen_safe(const t_safe_str *s)
{
    if (!s)
        return (0);
    return (s->len);
}

/* Безопасное копирование строки */
t_safe_str  *ft_strcpy_safe(t_safe_str *dest, const char *src)
{
    int i;

    if (!dest || !dest->ptr || !src)
        return (dest);
    i = 0;
    while (src[i] != '\0' && i < (dest->max_capacity - 1))
    {
        dest->ptr[i] = src[i];
        i++;
    }
    dest->ptr[i] = '\0';
    dest->len = i;
    return (dest);
}

/* Безопасное копирование не более n символов */
t_safe_str  *ft_strncpy_safe(t_safe_str *dest, const char *src, int n)
{
    int i;

    if (!dest || !dest->ptr || !src || n <= 0)
        return (dest);
    i = 0;
    while (src[i] != '\0' && i < n && i < (dest->max_capacity - 1))
    {
        dest->ptr[i] = src[i];
        i++;
    }
    dest->ptr[i] = '\0';
    dest->len = i;
    return (dest);
}

/* Безопасная конкатенация (дописывает src в конец dest) */
t_safe_str  *ft_strcat_safe(t_safe_str *dest, const char *src)
{
    int i;
    int j;

    if (!dest || !dest->ptr || !src)
        return (dest);
    i = dest->len;
    if (i < 0)
        i = 0;
    if (i > (dest->max_capacity - 1))
        i = dest->max_capacity - 1;
    j = 0;
    while (src[j] != '\0' && (i + j) < (dest->max_capacity - 1))
    {
        dest->ptr[i + j] = src[j];
        j++;
    }
    dest->ptr[i + j] = '\0';
    dest->len = i + j;
    return (dest);
}

/* Безопасная ограниченная конкатенация (аналог strlcat) */
t_safe_str  *ft_strlcat_safe(t_safe_str *dest, const char *src, int n)
{
    int i;
    int j;

    if (!dest || !dest->ptr || !src || n <= 0)
        return (dest);
    i = dest->len;
    if (i < 0)
        i = 0;
    if (i > (dest->max_capacity - 1))
        i = dest->max_capacity - 1;
    j = 0;
    while (src[j] != '\0' && j < n && (i + j) < (dest->max_capacity - 1))
    {
        dest->ptr[i + j] = src[j];
        j++;
    }
    dest->ptr[i + j] = '\0';
    dest->len = i + j;
    return (dest);
}

/* Безопасное объединение двух строк в dest (стирает предыдущее содержимое) */
t_safe_str  *ft_strjoin_safe(t_safe_str *dest, const char *s1, const char *s2)
{
    int i;
    int j;

    if (!dest || !dest->ptr)
        return (dest);
    i = 0;
    if (s1)
    {
        while (s1[i] != '\0' && i < (dest->max_capacity - 1))
        {
            dest->ptr[i] = s1[i];
            i++;
        }
    }
    j = 0;
    if (s2)
    {
        while (s2[j] != '\0' && (i + j) < (dest->max_capacity - 1))
        {
            dest->ptr[i + j] = s2[j];
            j++;
        }
    }
    dest->ptr[i + j] = '\0';
    dest->len = i + j;
    return (dest);
}
/* ============================================================
   СРАВНЕНИЕ СТРОК
   ============================================================ */

/* Безопасное сравнение строк */
int ft_strcmp_safe(const t_safe_str *a, const char *b)
{
    int i;

    if (!a || !a->ptr || !b)
    {
        if ((!a || !a->ptr) && !b)
            return (0);
        if (!a || !a->ptr)
            return (-1);
        return (1);
    }
    i = 0;
    while (i < a->len && b[i] && i < a->max_capacity)
    {
        if ((unsigned char)a->ptr[i] != (unsigned char)b[i])
            return ((unsigned char)a->ptr[i] - (unsigned char)b[i]);
        i++;
    }
    if (i >= a->len && i >= a->max_capacity)
        return (0);
    return ((unsigned char)a->ptr[i] - (unsigned char)b[i]);
}

/* Безопасное сравнение не более n символов */
int ft_strncmp_safe(const t_safe_str *a, const char *b, int n)
{
    int i;

    if (n <= 0)
        return (0);
    if (!a || !a->ptr || !b)
    {
        if ((!a || !a->ptr) && !b)
            return (0);
        if (!a || !a->ptr)
            return (-1);
        return (1);
    }
    i = 0;
    while (i < n && i < a->len && b[i] && i < a->max_capacity)
    {
        if ((unsigned char)a->ptr[i] != (unsigned char)b[i])
            return ((unsigned char)a->ptr[i] - (unsigned char)b[i]);
        i++;
    }
    if (i == n || i >= a->max_capacity)
        return (0);
    return ((unsigned char)a->ptr[i] - (unsigned char)b[i]);
}

/* Проверка на точное равенство строк */
int ft_strequ_safe(const t_safe_str *a, const char *b)
{
    return (ft_strcmp_safe(a, b) == 0);
}
/* ============================================================
   ПОИСК
   ============================================================ */

/* Поиск первого вхождения символа */
char    *ft_strchr_safe(const t_safe_str *s, int c)
{
    int i;

    if (!s || !s->ptr)
        return (NULL);
    i = 0;
    while (i < s->len && i < s->max_capacity)
    {
        if (s->ptr[i] == (char)c)
            return (&s->ptr[i]);
        i++;
    }
    if ((char)c == '\0')
        return (&s->ptr[i]);
    return (NULL);
}

/* Поиск последнего вхождения символа */
char    *ft_strrchr_safe(const t_safe_str *s, int c)
{
    int i;

    if (!s || !s->ptr)
        return (NULL);
    i = s->len;
    if ((char)c == '\0')
        return (&s->ptr[i]);
    while (i > 0)
    {
        i--;
        if (s->ptr[i] == (char)c)
            return (&s->ptr[i]);
    }
    return (NULL);
}

/* Поиск подстроки */
char    *ft_strstr_safe(const t_safe_str *haystack, const char *needle)
{
    int i;
    int j;

    if (!haystack || !haystack->ptr)
        return (NULL);
    if (!needle || needle[0] == '\0')
        return (haystack->ptr);
    i = 0;
    while (i < haystack->len && i < haystack->max_capacity)
    {
        j = 0;
        while (needle[j]
            && (i + j) < haystack->len
            && (i + j) < haystack->max_capacity
            && haystack->ptr[i + j] == needle[j])
        {
            j++;
        }
        if (needle[j] == '\0')
            return (&haystack->ptr[i]);
        i++;
    }
    return (NULL);
}
/* ============================================================
   ПОДСТРОКИ
   ============================================================ */

/* Безопасное извлечение подстроки */
t_safe_str  *ft_substr_safe(t_safe_str *dest, const t_safe_str *src,
                              int start, int len)
{
    int i;

    if (!dest || !dest->ptr || !src || !src->ptr)
        return (dest);
    if (start >= src->len || start < 0 || len <= 0)
    {
        dest->ptr[0] = '\0';
        dest->len = 0;
        return (dest);
    }
    i = 0;
    while (i < len && (start + i) < src->len && i < (dest->max_capacity - 1))
    {
        dest->ptr[i] = src->ptr[start + i];
        i++;
    }
    dest->ptr[i] = '\0';
    dest->len = i;
    return (dest);
}

/* ============================================================
   ТРАНСФОРМАЦИЯ
   ============================================================ */

/* Приведение к верхнему регистру (in-place) */
t_safe_str  *ft_toupper_safe(t_safe_str *s)
{
    int i;

    if (!s || !s->ptr)
        return (s);
    i = 0;
    while (i < s->len && i < s->max_capacity)
    {
        if (s->ptr[i] >= 'a' && s->ptr[i] <= 'z')
            s->ptr[i] -= 32;
        i++;
    }
    return (s);
}

/* Приведение к нижнему регистру (in-place) */
t_safe_str  *ft_tolower_safe(t_safe_str *s)
{
    int i;

    if (!s || !s->ptr)
        return (s);
    i = 0;
    while (i < s->len && i < s->max_capacity)
    {
        if (s->ptr[i] >= 'A' && s->ptr[i] <= 'Z')
            s->ptr[i] += 32;
        i++;
    }
    return (s);
}
/* Удаление пробельных символов с начала и конца (результат в dest) */
t_safe_str  *ft_strtrim_safe(t_safe_str *dest, const t_safe_str *src,
                               const char *set)
{
    int start;
    int end;
    int k;

    if (!dest || !dest->ptr || !src || !src->ptr)
        return (dest);
    if (!set)
        set = " \t\n\r\v\f";
    start = 0;
    while (start < src->len && start < src->max_capacity)
    {
        k = 0;
        while (set[k])
        {
            if (src->ptr[start] == set[k])
                break;
            k++;
        }
        if (set[k] == '\0')
            break;
        start++;
    }
    end = src->len;
    while (end > start)
    {
        k = 0;
        while (set[k])
        {
            if (src->ptr[end - 1] == set[k])
                break;
            k++;
        }
        if (set[k] == '\0')
            break;
        end--;
    }
    return (ft_substr_safe(dest, src, start, end - start));
}

/* ============================================================
   ОПЕРАЦИИ С ПАМЯТЬЮ
   ============================================================ */

/* Безопасное заполнение памяти */
t_safe_str  *ft_memset_safe(t_safe_str *dest, int c, int n)
{
    int i;

    if (!dest || !dest->ptr || n <= 0)
        return (dest);
    i = 0;
    while (i < n && i < dest->max_capacity)
    {
        dest->ptr[i] = (unsigned char)c;
        i++;
    }
    if (i < dest->max_capacity)
        dest->ptr[i] = '\0';
    else
        dest->ptr[dest->max_capacity - 1] = '\0';
    dest->len = (i < dest->max_capacity) ? i : (dest->max_capacity - 1);
    return (dest);
}

/* Безопасное копирование памяти */
t_safe_str  *ft_memcpy_safe(t_safe_str *dest, const void *src, int n)
{
    int                 i;
    const unsigned char *s;

    if (!dest || !dest->ptr || !src || n <= 0)
        return (dest);
    s = (const unsigned char *)src;
    i = 0;
    while (i < n && i < (dest->max_capacity - 1))
    {
        dest->ptr[i] = s[i];
        i++;
    }
    dest->ptr[i] = '\0';
    dest->len = i;
    return (dest);
}

/* Безопасное перемещение памяти (с учетом перекрытия) */
t_safe_str  *ft_memmove_safe(t_safe_str *dest, const void *src, int n)
{
    int                 i;
    const unsigned char *s;

    if (!dest || !dest->ptr || !src || n <= 0)
        return (dest);
    s = (const unsigned char *)src;
    if (dest->ptr > (char *)src)
    {
        i = n - 1;
        while (i >= 0 && i < (dest->max_capacity - 1))
        {
            dest->ptr[i] = s[i];
            i--;
        }
    }
    else
    {
        i = 0;
        while (i < n && i < (dest->max_capacity - 1))
        {
            dest->ptr[i] = s[i];
            i++;
        }
    }
    i = (n < dest->max_capacity - 1) ? n : dest->max_capacity - 1;
    dest->ptr[i] = '\0';
    dest->len = i;
    return (dest);
}

/* Безопасное обнуление памяти */
t_safe_str  *ft_bzero_safe(t_safe_str *dest)
{
    int i;

    if (!dest || !dest->ptr)
        return (dest);
    i = 0;
    while (i < dest->max_capacity)
    {
        dest->ptr[i] = 0;
        i++;
    }
    dest->len = 0;
    return (dest);
}

/* ============================================================
   ПРЕОБРАЗОВАНИЕ ЧИСЕЛ
   ============================================================ */

static int  fill_safe_itoa(long n, t_safe_str *s, int idx)
{
    if (n > 9)
        idx = fill_safe_itoa(n / 10, s, idx);
    if (idx < (s->max_capacity - 1))
    {
        s->ptr[idx] = "0123456789"[n % 10];
        return (idx + 1);
    }
    return (idx);
}

t_safe_str  *ft_itoa_safe(int n, t_safe_str *dest)
{
    long    nbr;
    int     idx;

    if (!dest || !dest->ptr)
        return (dest);
    nbr = n;
    idx = 0;
    if (nbr < 0)
    {
        if (idx < (dest->max_capacity - 1))
            dest->ptr[idx++] = '-';
        nbr = -nbr;
    }
    idx = fill_safe_itoa(nbr, dest, idx);
    dest->ptr[idx] = '\0';
    dest->len = idx;
    return (dest);
}

/* ============================================================
   УПРАВЛЕНИЕ БУФЕРОМ
   ============================================================ */

/* Очистка содержимого (быстрый сброс) */
t_safe_str  *ft_strclear_safe(t_safe_str *s)
{
    if (!s || !s->ptr)
        return (s);
    s->ptr[0] = '\0';
    s->len = 0;
    return (s);
}
/* Копирование безопасной строки в безопасную строку */
t_safe_str  *ft_strdup_safe(t_safe_str *dest, const t_safe_str *src)
{
    if (!dest || !dest->ptr || !src || !src->ptr)
        return (dest);
    return (ft_strcpy_safe(dest, src->ptr));
}

/* ============================================================
   КЛАССИФИКАЦИЯ СИМВОЛОВ
   ============================================================ */

static inline int   ft_isalpha(int c)
{
    return ((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z'));
}

static inline int   ft_isdigit(int c)
{
    return (c >= '0' && c <= '9');
}

static inline int   ft_isalnum(int c)
{
    return (ft_isalpha(c) || ft_isdigit(c));
}

static inline int   ft_isascii(int c)
{
    return (c >= 0 && c <= 127);
}

static inline int   ft_isprint(int c)
{
    return (c >= 32 && c <= 126);
}

static inline int   ft_isspace(int c)
{
    return (c == ' ' || (c >= '\t' && c <= '\r'));
}

static inline int   ft_toupper(int c)
{
    if (c >= 'a' && c <= 'z')
        return (c - 32);
    return (c);
}

static inline int   ft_tolower(int c)
{
    if (c >= 'A' && c <= 'Z')
        return (c + 32);
    return (c);
}

/* ============================================================
   КЛАССИФИКАЦИЯ ЦЕЛЫХ СТРОК
   ============================================================ */

/* Все символы — буквы */
int ft_str_isalpha_safe(const t_safe_str *s)
{
    int i;

    if (!s || !s->ptr || s->len == 0)
        return (0);
    i = 0;
    while (i < s->len && i < s->max_capacity)
    {
        if (!ft_isalpha((unsigned char)s->ptr[i]))
            return (0);
        i++;
    }
    return (1);
}

/* Все символы — цифры */
int ft_str_isdigit_safe(const t_safe_str *s)
{
    int i;

    if (!s || !s->ptr || s->len == 0)
        return (0);
    i = 0;
    while (i < s->len && i < s->max_capacity)
    {
        if (!ft_isdigit((unsigned char)s->ptr[i]))
            return (0);
        i++;
    }
    return (1);
}

/* ============================================================
   ПРИМЕНЕНИЕ ФУНКЦИИ К КАЖДОМУ СИМВОЛУ (in-place)
   ============================================================ */

t_safe_str  *ft_striteri_safe(t_safe_str *s,
                void (*f)(unsigned int, char *))
{
    unsigned int    i;

    if (!s || !s->ptr || !f)
        return (s);
    i = 0;
    while ((int)i < s->len && (int)i < s->max_capacity)
    {
        f(i, &s->ptr[i]);
        i++;
    }
    return (s);
}

/* ============================================================
   ВЫВОД
   ============================================================ */

#ifdef SAFE_C_STDIO

#include <stdio.h>

void    ft_putstr_safe(const t_safe_str *s)
{
    if (!s || !s->ptr)
        return ;
    printf("%.*s", s->len, s->ptr);
}

void    ft_putendl_safe(const t_safe_str *s)
{
    ft_putstr_safe(s);
    printf("\n");
}

#else

void    ft_putstr_safe(const t_safe_str *s);
void    ft_putendl_safe(const t_safe_str *s);

#endif