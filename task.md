typedef struct s_safe_str
{
    char        *ptr;      // Указатель на массив памяти
    const int   max_capacity; // Жесткий предел буфера (доступен только для чтения)
    int         len;       // Текущая заполненная длина строки
} t_safe_str;

// Макрос-конструктор: создает безопасную строку на стеке БЕЗ malloc с помощью составного литерала
#define MAKE_SAFE_STR(size) (t_safe_str){ .ptr = (char[size]){0}, .max_capacity = size, .len = 0 }


t_safe_str  *ft_strcpy_safe(t_safe_str *dest, const char *src)
{
    int i;

    if (!dest || !dest->ptr || !src)
        return (dest);
    i = 0;
    // Копируем только пока индекс строго меньше максимальной вместимости - 1 (для \0)
    while (src[i] != '\0' && i < (dest->max_capacity - 1))
    {
        dest->ptr[i] = src[i];
        i++;
    }
    dest->ptr[i] = '\0';
    dest->len = i;
    return (dest);
}



static int  fill_safe_itoa(long n, t_safe_str *s, int idx)
{
    if (n > 9)
        idx = fill_safe_itoa(n / 10, s, idx);
    
    // Проверка: если цифра пытается выйти за пределы буфера, аварийно останавливаемся
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
        nbr = -nbr; // Абсолютно безопасно для INT_MIN благодаря типу long
    }
    idx = fill_safe_itoa(nbr, dest, idx);
    dest->ptr[idx] = '\0';
    dest->len = idx;
    return (dest);
}


t_safe_str  *ft_strjoin_safe(t_safe_str *dest, const char *s1, const char *s2)
{
    int i;
    int j;

    if (!dest || !dest->ptr)
        return (dest);
    i = 0;
    // Копируем первую строку
    if (s1)
    {
        while (s1[i] != '\0' && i < (dest->max_capacity - 1))
        {
            dest->ptr[i] = s1[i];
            i++;
        }
    }
    // Конкатенируем вторую строку с места остановки
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


#include <stdio.h>

int main(void)
{
    // 1. Создаем безопасные контейнеры памяти «на лету» на стеке main
    t_safe_str str_num = MAKE_SAFE_STR(12); // Идеальный размер под int
    t_safe_str str_join = MAKE_SAFE_STR(30); // Буфер под объединение строк

    // 2. Безопасно переводим критический INT_MIN в строку
    ft_itoa_safe(-2147483648, &str_num);
    printf("Safe Itoa result: %s (Length: %d/%d)\n", 
           str_num.ptr, str_num.len, str_num.max_capacity);

    // 3. Безопасное объединение без malloc
    ft_strjoin_safe(&str_join, "Status code: ", str_num.ptr);
    printf("Safe Join result: %s\n", str_join.ptr);

    // 4. Демонстрация ЗАЩИТЫ от взлома памяти (Buffer Overflow Protection)
    // Пытаемся записать длинный текст в крошечный буфер на 5 байт
    t_safe_str tiny_str = MAKE_SAFE_STR(5);
    ft_strcpy_safe(&tiny_str, "HELLO_WORLD_LONG_STRING");
    
    // Программа НЕ упала, память вокруг буфера НЕ повреждена. 
    // Строка просто безопасно обрезалась под размер буфера.
    printf("Protected tiny string: %s\n", tiny_str.ptr); // Выведет: HELL

    return (0);
}




Zadacha perepisat i sdelat jazik ci polnostju bezopasnim vot primeri realizacii nekotorih funkcij