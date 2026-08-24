#define SAFE_C_STDIO
#include "safe_stdlib.c"
#include <stdio.h>

static int errs = 0;

static void check(int cond, const char *msg)
{
    if (!cond) { printf("[FAIL]  %s\n", msg); errs++; }
    else printf("[EDGE]  %s\n", msg);
}

static int cmp_int(const void *a, const void *b)
{
    return (*(int *)a - *(int *)b);
}

int main(void)
{
    int v, r;
    long lv;

    printf("========== safe_stdlib Edge-Case Suite ==========\n\n");

    /* ---- 1. Arithmetic: add ---- */
    printf("--- 1. Arithmetic: add ---\n");
    check(ft_add_safe(2,3,&v) && v==5, "add(2,3)=5");
    check(ft_add_safe(-1,1,&v) && v==0, "add(-1,1)=0");
    check(!ft_add_safe(INT_MAX,1,&v), "add overflow MAX+1");
    check(!ft_add_safe(INT_MIN,-1,&v), "add overflow MIN-1");
    check(!ft_add_safe(5,5,NULL), "add res=NULL");
/* ---- 2. sub ---- */
    printf("\n--- 2. sub ---\n");
    check(ft_sub_safe(5,2,&v) && v==3, "sub(5,2)=3");
    check(ft_sub_safe(0,5,&v) && v==-5, "sub(0,5)=-5");
    check(!ft_sub_safe(INT_MAX,-1,&v), "sub overflow");

    /* ---- 3. mul ---- */
    printf("\n--- 3. mul ---\n");
    check(ft_mul_safe(3,4,&v) && v==12, "mul(3,4)=12");
    check(ft_mul_safe(-2,3,&v) && v==-6, "mul(-2,3)=-6");
    check(!ft_mul_safe(INT_MAX,2,&v), "mul overflow MAX*2");
    check(!ft_mul_safe(INT_MIN,-1,&v), "mul overflow MIN*-1");

    /* ---- 4. div/mod ---- */
    printf("\n--- 4. div/mod ---\n");
    check(ft_div_safe(10,3,&v) && v==3, "div(10,3)=3");
    check(!ft_div_safe(5,0,&v), "div by 0");
    check(!ft_div_safe(INT_MIN,-1,&v), "div MIN/-1");
    check(ft_mod_safe(10,3,&v) && v==1, "mod(10,3)=1");
    check(!ft_mod_safe(5,0,&v), "mod by 0");

    /* ---- 5. long_to_int ---- */
    printf("\n--- 5. long_to_int ---\n");
    check(ft_long_to_int_safe(42,&v) && v==42, "long_to_int(42)");
    check(!ft_long_to_int_safe((long)INT_MAX+1,&v), "overflow");
/* ---- 6. atoi ---- */
    printf("\n--- 6. atoi ---\n");
    check(ft_atoi_safe("42",&v) && v==42, "atoi(42)");
    check(ft_atoi_safe("  -99",&v) && v==-99, "atoi(-99)");
    check(ft_atoi_safe("+0",&v) && v==0, "atoi(+0)");
    check(!ft_atoi_safe("hello",&v), "atoi(bad)");
    check(!ft_atoi_safe("999999999999",&v), "atoi(overflow)");
    check(!ft_atoi_safe(NULL,&v), "atoi(NULL)");
    check(!ft_atoi_safe("",&v), "atoi(empty)");

    /* ---- 7. atol ---- */
    printf("\n--- 7. atol ---\n");
    check(ft_atol_safe("1234567890",&lv) && lv==1234567890L, "atol OK");
    check(!ft_atol_safe("bad",&lv), "atol bad");

    /* ---- 8. Vector ---- */
    printf("\n--- 8. Vector ---\n");
    t_safe_vec vec = ft_vec_new(sizeof(int), 2);
    check(vec.error == 0, "vec_new OK");
    int x = 10;
    check(ft_vec_push(&vec,&x)==0, "push 10");
    x = 20; ft_vec_push(&vec,&x);
    x = 30; ft_vec_push(&vec,&x);
    check(vec.len==3 && vec.capacity==4, "grew cap from 2 to 4");
    check(*(int*)ft_vec_get(&vec,1)==20, "get(1)=20");
    check(ft_vec_get(&vec,99)==NULL, "get(99)=NULL");
    check(ft_vec_pop(&vec)==0 && vec.len==2, "pop OK");
    ft_vec_free(&vec);
    check(vec.data==NULL, "free sets NULL");

    /* ---- 9. snprintf ---- */
    printf("\n--- 9. snprintf ---\n");
    t_safe_str buf = MAKE_SAFE_STR(32);
    r = ft_snprintf_safe(&buf, "val=%d", 42);
    check(r==6 && buf.len==6, "snprintf val=42");

    /* ---- 10. qsort ---- */
    printf("\n--- 10. qsort ---\n");
    int arr[] = {3,1,4,1,5};
    ft_qsort_safe(arr,5,sizeof(int),cmp_int);
    check(arr[0]==1 && arr[4]==5, "qsort sorted");
    check(ft_qsort_safe(NULL,5,4,cmp_int)==-1, "qsort NULL base");

    /* ---- 11. malloc/free ---- */
    printf("\n--- 11. malloc/free ---\n");
    int *p = ft_malloc_safe(sizeof(int)*100);
    check(p!=NULL, "malloc 400B");
    *p = 42;
    ft_free_safe((void**)&p);
    check(p==NULL, "free NULLs ptr");
    check(ft_calloc_safe(10,sizeof(int))!=NULL, "calloc OK");
    check(ft_malloc_safe(0)==NULL, "malloc(0)=NULL");

    if (errs) printf("\n========== %d ERRORS ==========\n", errs);
    else printf("\n========== ALL TESTS PASSED ==========\n");
    return (errs);
}