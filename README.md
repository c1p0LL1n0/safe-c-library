# safe-c-library

Полностью безопасная замена стандартной библиотеки C — без `malloc`, без переполнений буфера, без падений на `NULL`.

**99 програмных тестов** — каждый краевой случай проверен.

## Структура

| Файл | Назначение | Строк |
|------|-----------|-------|
| `safe_c.c` | Безопасные строки, память, itoa, классификация | 676 |
| `safe_c_demo.c` | Визуальная демонстрация (40+ тестов) | ~280 |
| `safe_c_verify.c` | Программный авто-верификатор (64 теста) | 200 |
| `safe_stdlib.c` | Врапперы stdlib: арифметика, atoi, вектор, файлы, snprintf, qsort, malloc | 259 |
| `safe_stdlib_demo.c` | Визуальная демонстрация stdlib (20+ тестов) | ~115 |
| `safe_stdlib_verify.c` | Программный авто-верификатор stdlib (35 тестов) | 90 |

## Быстрый старт

```bash
git clone https://github.com/c1p0LL1n0/safe-c-library.git
cd safe-c-library

# Авто-тесты (главное!)
gcc -Wall -Wextra -std=c99 safe_c_verify.c -o verify && ./verify
gcc -Wall -Wextra -std=c99 safe_stdlib_verify.c -o verify2 && ./verify2

# Визуальные демо
gcc -Wall -Wextra -std=c99 safe_c_demo.c -o demo && ./demo
gcc -Wall -Wextra -std=c99 safe_stdlib_demo.c -o demo2 && ./demo2
```

## Структура `t_safe_str`

```c
typedef struct s_safe_str {
    char        *ptr;           // указатель на буфер
    const int   max_capacity;   // жёсткий предел (только чтение)
    int         len;            // текущая длина строки
} t_safe_str;

// Конструктор без malloc — составной литерал на стеке
#define MAKE_SAFE_STR(size) \
    (t_safe_str){ .ptr = (char[size]){0}, .max_capacity = size, .len = 0 }
```
## API: `safe_c.c` — 31 функция

### Строковые операции

| Функция | Аналог | Возвращает |
|---------|--------|------------|
| `ft_strlen_safe(s)` | `strlen` | `int` — O(1) |
| `ft_strcpy_safe(dest, src)` | `strcpy` | `t_safe_str*` |
| `ft_strncpy_safe(dest, src, n)` | `strncpy` | `t_safe_str*` |
| `ft_strcat_safe(dest, src)` | `strcat` | `t_safe_str*` |
| `ft_strlcat_safe(dest, src, n)` | `strlcat` | `t_safe_str*` |
| `ft_strjoin_safe(dest, s1, s2)` | — | `t_safe_str*` |
| `ft_strdup_safe(dest, src)` | `strdup` | `t_safe_str*` |
| `ft_strclear_safe(s)` | — | `t_safe_str*` |

### Сравнение

| Функция | Возвращает |
|---------|-----------|
| `ft_strcmp_safe(a, b)` | `<0`, `0`, `>0` |
| `ft_strncmp_safe(a, b, n)` | `<0`, `0`, `>0` |
| `ft_strequ_safe(a, b)` | `1`/`0` |

### Поиск

| Функция | Аналог |
|---------|--------|
| `ft_strchr_safe(s, c)` | `strchr` |
| `ft_strrchr_safe(s, c)` | `strrchr` |
| `ft_strstr_safe(hay, needle)` | `strstr` |

### Подстроки и трансформация

| Функция | Примечание |
|---------|-----------|
| `ft_substr_safe(dest, src, start, len)` | |
| `ft_strtrim_safe(dest, src, set)` | `set=NULL` → пробелы |
| `ft_toupper_safe(s)` | in-place |
| `ft_tolower_safe(s)` | in-place |
| `ft_striteri_safe(s, callback)` | in-place |

### Память

| Функция | Аналог |
|---------|--------|
| `ft_memset_safe(d, c, n)` | `memset` |
| `ft_memcpy_safe(d, s, n)` | `memcpy` |
| `ft_memmove_safe(d, s, n)` | `memmove` |
| `ft_bzero_safe(d)` | `bzero` |

### Числа

`ft_itoa_safe(n, dest)` → `t_safe_str*`

### Классификация символов

`ft_isalpha` · `ft_isdigit` · `ft_isalnum` · `ft_isascii` · `ft_isprint` · `ft_isspace` · `ft_toupper` · `ft_tolower`

### Классификация строк
## API: `safe_stdlib.c` — 18 функций

Все возвращают `1` (успех) или `0` (ошибка), результат через указатель.

### Безопасная арифметика

| Функция | Проверка |
|---------|----------|
| `ft_add_safe(a, b, &r)` | `a + b` без переполнения |
| `ft_sub_safe(a, b, &r)` | `a - b` без переполнения |
| `ft_mul_safe(a, b, &r)` | `a * b` без переполнения |
| `ft_div_safe(a, b, &r)` | `b != 0`, не `INT_MIN / -1` |
| `ft_mod_safe(a, b, &r)` | `b != 0`, не `INT_MIN % -1` |
| `ft_long_to_int_safe(n, &r)` | `n` помещается в `int` |

### Конвертация строк

| Функция | Аналог | Возврат |
|---------|--------|---------|
| `ft_atoi_safe(str, &r)` | `atoi` | 1/0 |
| `ft_strtoi_safe(str, &end, &r)` | `strtol` | 1/0 |
| `ft_atol_safe(str, &r)` | `atol` | 1/0 |

### Динамический вектор

```c
typedef struct s_safe_vec {
    void *data; int elem_size, capacity, len, error;
} t_safe_vec;
```

`ft_vec_new(sz, cap)` · `ft_vec_get(v, i)` · `ft_vec_push(v, e)` · `ft_vec_pop(v)` · `ft_vec_free(v)`

### Файлы / форматирование / память

`ft_fopen_safe` · `ft_fgets_safe` · `ft_getline_safe` · `ft_snprintf_safe` · `ft_qsort_safe` · `ft_malloc_safe` · `ft_calloc_safe` · `ft_free_safe`

## Пример

```c
#define SAFE_C_STDIO
#include "safe_stdlib.c"

int main(void) {
    t_safe_str name = MAKE_SAFE_STR(64);
    ft_strcpy_safe(&name, "World");

    t_safe_str greeting = MAKE_SAFE_STR(128);
    ft_strjoin_safe(&greeting, "Hello, ", name.ptr);
    printf("%s\n", greeting.ptr);

    int val;
    if (ft_atoi_safe("12345", &val))
        printf("Parsed: %d\n", val);

    int result;
    if (ft_mul_safe(1000000, 3000, &result))
        printf("1000000*3000 = %d\n", result);
    else
        printf("Overflow prevented!\n");

    return (0);
}
```

## Свойства безопасности

- **NULL-safe** — ни одна функция не падает на `NULL`
- **Bound-check** — ни один байт не пишется за `max_capacity`
- **Всегда `\0`** — гарантированная нуль-терминация
- **Нет malloc** в `safe_c.c` — только стековые буферы
- **O(1) длина** — поле `len` в структуре
- **Составные литералы** — буферы на лету: `&(t_safe_str){.ptr=(char[32]){0}, .max_capacity=32}`
- **Коды ошибок** — `safe_stdlib.c`: 1 = успех, 0 = ошибка

## Компиляция

```bash
gcc -Wall -Wextra -std=c99 safe_c_verify.c -o verify
```

Только `#include "safe_c.c"` или `#include "safe_stdlib.c"` — без внешних зависимостей.
## ⚠️ Дисклеймер

> **Используйте с осторожностью.** Эта библиотека — учебный проект, написанный в рамках задания по безопасному программированию на C. Автор не даёт гарантий отсутствия багов, уязвимостей или undefined behavior. Перед использованием в production-окружении проведите собственный аудит кода.
>
> Библиотека прошла **99 програмных тестов** (64 — `safe_c`, 35 — `safe_stdlib`) и обрабатывает все известные краевые случаи, но абсолютная безопасность не может быть гарантирована ввиду сложности языка C и платформенных различий.

## Лицензия (MIT)

```
MIT License

Copyright (c) 2026 c1p0LL1n0

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
```

**Коротко:** делайте с кодом что хотите — используйте, модифицируйте, распространяйте. Указывайте авторство. Без гарантий.

## Как внести вклад

Pull request'ы приветствуются! Что можно улучшить:

- 🐛 **Нашли баг?** — откройте issue, приложите минимальный пример
- 🔧 **Новые функции** — обёртки для оставшихся опасных функций (`strtok`, `realpath`, `system`, …)
- 🧪 **Тесты** — новые краевые случаи, fuzz-тестирование
- 📦 **CMake / Makefile** — система сборки
- 🌐 **Безопасная работа с сетью** — обёртки `socket`, `bind`, `connect`
- 🧵 **Потокобезопасность** — `_r`-версии функций

**Стиль кода:** никакого `malloc` в `safe_c.c`, обязательная проверка `NULL` и границ буфера, все возвращаемые значения осмысленны (нет `void`).