#include <error.h>
#include <dynaarray.h>
#include <linkedlist.h>
#include <hashmap.h>
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <assert.h>
#include <stdio.h>
#include <string.h>

typedef struct
{
    char *name;
    unsigned char age;
} Student, *LPStudent;

int copy_func(void *dst, void *src)
{
    LPStudent d = (LPStudent)dst;
    LPStudent s = (LPStudent)src;
    d->name = (char *)malloc(3);
    if (d->name != NULL_POINTER)
        strcpy(d->name, s->name);

    d->age = s->age;

    return TRUE;
}

int compare_func(void *dst, void *src)
{
    LPStudent d = (LPStudent)dst;
    LPStudent s = (LPStudent)src;
    if (strcmp(d->name, s->name) == 0 && d->age == s->age)
        return TRUE;

    return FALSE;
}

int free_func(void *dst)
{
    LPStudent lp_s = (LPStudent)dst;
    free(lp_s->name);
    return TRUE;
}

void test_dynaarray()
{
    LPDynaArray lp_da = (LPDynaArray)malloc(sizeof(DynaArray));
    assert(lp_da != NULL_POINTER);

    long capacity, ele_size;
    capacity = 10;
    ele_size = sizeof(Student);
    int r;

    r = DynaArray_init(lp_da, capacity, ele_size, copy_func, compare_func, free_func);
    assert(r == TRUE);

    r = DynaArray_reallocate(lp_da, capacity * 2);
    assert(r == TRUE);

    char *mm = malloc(3);
    char *gg = malloc(3);
    char *jj = malloc(3);
    char *dd = "dd";

    strcpy(mm, "MM");
    strcpy(gg, "GG");
    strcpy(jj, "JJ");

    Student smm = {mm, 16};
    Student sgg = {gg, 18};
    Student sjj = {jj, 17};
    Student sdd = {dd, 16};

    r = DynaArray_insert(lp_da, lp_da->eles_num, &smm);
    assert(r == TRUE);

    r = DynaArray_insert(lp_da, lp_da->eles_num, &sgg);
    assert(r == TRUE);

    r = DynaArray_insert(lp_da, lp_da->eles_num, &sjj);
    assert(r == TRUE);

    r = DynaArray_insert(lp_da, lp_da->eles_num, &sdd);
    assert(r == TRUE);

    // r = DynaArray_del_by_pos(lp_da, 2);
    // r = DynaArray_del_by_ele(lp_da, &sdd);
    // r = DynaArray_edit_by_pos(lp_da, 2, &sdd);

    LPDynaArray lp_poses = (LPDynaArray)malloc(sizeof(DynaArray));
    assert(lp_poses != NULL_POINTER);
    r = DynaArray_init(lp_poses, 10, sizeof(long), NULL_POINTER, NULL_POINTER, NULL_POINTER);
    assert(r != FALSE);

    // r = DynaArray_edit_by_ele(lp_da, &sdd, &sgg, lp_poses);
    assert(r == TRUE);

    // print inserted elements
    for (long i = 0; i < lp_da->eles_num; i++)
    {
        LPStudent s = (LPStudent)((char *)lp_da->eles + i * lp_da->ele_size);
        printf(" lp_da %s, %d\n", s->name, s->age);
    }

    r = DynaArray_get_pos_by_ele(lp_da, &sgg, lp_poses);
    assert(r != FALSE);

    // print poses
    for (long i = 0; i < lp_poses->eles_num; i++)
    {
        long *pos = (long *)((char *)lp_poses->eles + i * lp_poses->ele_size);
        printf(" lp_poses %d\n", *pos);
    }

    LPStudent out = (LPStudent)malloc(sizeof(Student));
    assert(NULL_POINTER != memset((char *)out, 0, sizeof(Student)));
    assert(out != NULL_POINTER);

    r = DynaArray_get_by_pos(lp_da, 0, out);
    assert(r != FALSE);

    printf(" out %s, %d\n", out->name, out->age);

    r = DynaArray_free(lp_da);
    assert(r == TRUE);

    r = DynaArray_free(lp_poses);
    assert(r == TRUE);

    free(lp_da);
    free(lp_poses);
}

void test_linkedlist()
{
    int r;
    LPLinkedList lp_ll = (LPLinkedList)malloc(sizeof(LinkedList));
    assert(lp_ll != NULL_POINTER);

    // r = LinkedList_init(lp_ll, sizeof(Student), NULL_POINTER, NULL_POINTER, NULL_POINTER);
    r = LinkedList_init(lp_ll, sizeof(Student), copy_func, compare_func, free_func);
    assert(r != FALSE);

    Student dd = {"DD", 15};
    Student mm = {"MM", 16};
    Student jj = {"JJ", 17};
    Student gg = {"GG", 18};
    Student cw = {"CW", 3};
    r = LinkedList_insert(lp_ll, lp_ll->eles_num, &dd);
    r = LinkedList_insert(lp_ll, lp_ll->eles_num, &mm);
    r = LinkedList_insert(lp_ll, lp_ll->eles_num, &jj);
    r = LinkedList_insert(lp_ll, lp_ll->eles_num, &gg);
    r = LinkedList_insert(lp_ll, 4, &cw);
    r = LinkedList_insert(lp_ll, 5, &cw);
    // r = LinkedList_del_by_pos(lp_ll, 4);

    LPDynaArray lp_poses = (LPDynaArray)malloc(sizeof(DynaArray));
    assert(lp_poses != NULL_POINTER);
    r = DynaArray_init(lp_poses, 10, sizeof(long), NULL_POINTER, NULL_POINTER, NULL_POINTER);
    assert(r != FALSE);

    // r = LinkedList_del_by_ele(lp_ll, &cw, lp_poses);
    Student oo;
    r = LinkedList_get_by_pos(lp_ll, lp_ll->eles_num - 1, &oo);
    assert(r != FALSE);
    printf(" oo %s, %d\n", oo.name, oo.age);

    // r = LinkedList_get_pos_by_ele(lp_ll, &cw, lp_poses);
    // r = LinkedList_edit_by_pos(lp_ll, lp_ll->eles_num - 1, &dd);
    r = LinkedList_edit_by_ele(lp_ll, &cw, &dd, lp_poses);
    assert(r != FALSE);

    char *arr = (char *)lp_poses->eles;
    char *src;
    for (long i = 0; i < lp_poses->eles_num; i++)
    {
        src = arr + lp_poses->ele_size * i;
        printf(" lp_poses %d\n", *(long *)src);
    }

    LPLinkedListNode lp_lln = lp_ll->lp_head;
    LPStudent s;
    for (long i = 0; i < lp_ll->eles_num; i++)
    {
        s = (LPStudent)lp_lln->ele;
        printf(" lp_ll %s, %d\n", s->name, s->age);
        lp_lln = lp_lln->next;
    }

    if (lp_ll->eles_num != 0)
    {
        LPStudent o = (LPStudent)lp_ll->lp_head->ele;
        LPStudent u = (LPStudent)lp_ll->lp_tail->ele;
        printf(" lp_ll h %s, %d t %s, %d\n", o->name, o->age, u->name, u->age);
    }

    r = LinkedList_free(lp_ll);
    assert(r != FALSE);
    free(lp_ll);

    r = DynaArray_free(lp_poses);
    assert(r != FALSE);
    free(lp_poses);
}

int hm_copy_func(void *dst, void *src)
{
	LPHashMapKV d = (LPHashMapKV)dst;
	LPHashMapKV s = (LPHashMapKV)src;
	d->key = (char*)malloc(s->key_size);
	d->value = (char*)malloc(s->value_size);
    d->key_size = s->key_size;
    d->value_size = s->value_size;
	if (d->key != NULL_POINTER)
		memcpy((char*)d->key, (char*)s->key, s->key_size);
	if (d->value != NULL_POINTER)
		memcpy((char*)d->value, (char*)s->value, s->value_size); 

    return TRUE;
}

int hm_compare_func(void *dst, void *src)
{
    LPHashMapKV d = (LPHashMapKV)dst;
    LPHashMapKV s = (LPHashMapKV)src;
    if (d->key_size == s->key_size && memcmp((char*)d->key, (char*)s->key, s->key_size) == 0)
        return TRUE;
    return FALSE;
}

int hm_free_func(void *dst)
{
    LPHashMapKV d = (LPHashMapKV)dst;
    free(d->key);
    free(d->value);
    free(d);

    return TRUE;
}

void test_hashmap()
{
    LPHashMap lp_hm = (LPHashMap)malloc(sizeof(HashMap));
    assert(lp_hm != NULL_POINTER);
    int r = HashMap_init(lp_hm, hm_copy_func, hm_compare_func, hm_free_func);
    assert(r != FALSE);

    char *ddn = "DD";
    char *mmn = "MM";
    char *jjn = "JJ";
    char *ggn = "GG";
    char *cwn = "CW";
    Student dd = {ddn, 15};
    Student mm = {mmn, 16};
    Student jj = {jjn, 17};
    Student gg = {ggn, 18};
    Student cw = {cwn, 3};
    long ss = sizeof(Student);

    r = HashMap_put(lp_hm, ddn, 3, &dd, ss);
    // printf(" err %d\n", get_last_error());
    assert(r != FALSE);
    HashMapKV hmkv;
    hmkv.key = ddn;
    hmkv.key_size = 3;

    r = HashMap_get(lp_hm, &hmkv);
    assert(r != FALSE);
    printf(" ss=%d, value_size=%d\n", ss, hmkv.value_size);
    LPStudent lps = (LPStudent)hmkv.value;
    printf(" name=%s, age=%d\n", lps->name, lps->age);

    HashMap_free(lp_hm);
    free(lp_hm);
}

int main()
{
    // printf("DynaArray tests..........\n");
    // test_dynaarray();
    // // printf("LinkedList tests.........\n");
    // test_linkedlist();
    printf("HashMap tests...............\n");
    test_hashmap();
    return 0;
}