#define SAFE_C_STDIO
#include "safe_stdlib.c"
#include <stdio.h>

static int errs=0, tnum=0;

#define T(cond, lbl) do { tnum++; if (!(cond)) { \
    printf("FAIL T%d: %s\n", tnum, lbl); errs++; } } while(0)

static int cmp(const void *a, const void *b)
{ return (*(int*)a - *(int*)b); }

int main(void)
{
    int v, r; long lv;

    printf("========== STDLIB AUTO-VERIFY ==========\n\n");

    /* ---- arithmetic ---- */
    T(ft_add_safe(2,3,&v) && v==5, "add 2+3");
    T(!ft_add_safe(INT_MAX,1,&v), "add overflow");
    T(!ft_add_safe(5,5,NULL), "add NULL");
T(ft_sub_safe(0,5,&v) && v==-5, "sub 0-5");
    T(!ft_sub_safe(INT_MAX,-1,&v), "sub overflow");

    T(ft_mul_safe(3,4,&v) && v==12, "mul 3*4");
    T(ft_mul_safe(-2,3,&v) && v==-6, "mul -2*3");
    T(ft_mul_safe(0,99,&v) && v==0, "mul 0*99");
    T(!ft_mul_safe(INT_MAX,2,&v), "mul overflow");
    T(!ft_mul_safe(INT_MIN,-1,&v), "mul MIN*-1");

    T(ft_div_safe(10,3,&v) && v==3, "div 10/3");
    T(!ft_div_safe(5,0,&v), "div by 0");
    T(!ft_div_safe(INT_MIN,-1,&v), "div MIN/-1");

    T(ft_mod_safe(10,3,&v) && v==1, "mod 10%3");
    T(!ft_mod_safe(5,0,&v), "mod by 0");

    T(ft_sub_safe(5,2,&v) && v==3, "sub 5-2");
/* ---- atoi ---- */
    T(ft_atoi_safe("42",&v) && v==42, "atoi 42");
    T(ft_atoi_safe("  -99",&v) && v==-99, "atoi -99");
    T(!ft_atoi_safe("hello",&v), "atoi bad");
    T(!ft_atoi_safe("999999999999",&v), "atoi overflow");
    T(!ft_atoi_safe(NULL,&v), "atoi NULL");

    /* ---- atol ---- */
    T(ft_atol_safe("1234567890",&lv) && lv==1234567890L, "atol");

    /* ---- vector ---- */
    {
        t_safe_vec vec = ft_vec_new(sizeof(int), 2);
        T(vec.error==0, "vec_new");
        int vals[]={10,20,30};
        ft_vec_push(&vec,&vals[0]); ft_vec_push(&vec,&vals[1]);
        ft_vec_push(&vec,&vals[2]);
        T(vec.len==3, "vec push");
        T(*(int*)ft_vec_get(&vec,1)==20, "vec get");
        T(ft_vec_pop(&vec)==0 && vec.len==2, "vec pop");
        ft_vec_free(&vec); T(vec.data==NULL, "vec free");
    }

    /* ---- snprintf ---- */
    {
        t_safe_str buf = MAKE_SAFE_STR(32);
        r = ft_snprintf_safe(&buf, "val=%d", 42);
        T(r==6, "snprintf ret");
        T(ft_strcmp_safe(&buf, "val=42")==0, "snprintf buf");
    }

    /* ---- qsort ---- */
    {
        int arr[] = {3,1,4,1,5};
        ft_qsort_safe(arr,5,sizeof(int),cmp);
        T(arr[0]==1 && arr[4]==5, "qsort");
        T(ft_qsort_safe(NULL,5,4,cmp)==-1, "qsort NULL");
    }

    /* ---- malloc ---- */
    {
        int *p = ft_malloc_safe(sizeof(int)*100);
        T(p!=NULL, "malloc");
        ft_free_safe((void**)&p);
        T(p==NULL, "free NULLs");
        T(ft_calloc_safe(10,sizeof(int))!=NULL, "calloc");
        T(ft_malloc_safe(0)==NULL, "malloc(0)");
    }

    if (errs)
        printf("\n========== %d FAILURES / %d TESTS ==========\n", errs, tnum);
    else
        printf("\n========== ALL %d TESTS PASSED ==========\n", tnum);
    return (errs);
}