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

#include <locale.h>

typedef struct {
  char* name;
  unsigned char age;
} Student, *LPStudent;

int copy_func(void* dst, void* src) {
  LPStudent d = (LPStudent)dst;
  LPStudent s = (LPStudent)src;
  d->name = (char*)malloc(3);
  if (d->name != NULL_POINTER) strcpy(d->name, s->name);

  d->age = s->age;

  return TRUE;
}

int compare_func(void* dst, void* src) {
  LPStudent d = (LPStudent)dst;
  LPStudent s = (LPStudent)src;
  if (strcmp(d->name, s->name) == 0 && d->age == s->age) return TRUE;

  return FALSE;
}

int free_func(void* dst) {
  LPStudent lp_s = (LPStudent)dst;
  free(lp_s->name);
  return TRUE;
}

void test_dynaarray() {
  LPDynaArray lp_da = (LPDynaArray)malloc(sizeof(DynaArray));
  assert(lp_da != NULL_POINTER);

  long capacity, ele_size;
  capacity = 10;
  ele_size = sizeof(Student);
  int r;

  r = DynaArray_init(lp_da, capacity, ele_size, copy_func, compare_func,
                     free_func);
  assert(r == TRUE);

  r = DynaArray_reallocate(lp_da, capacity * 2);
  assert(r == TRUE);

  char* mm = malloc(3);
  char* gg = malloc(3);
  char* jj = malloc(3);
  char* dd = "dd";

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

  // print inserted elements
  printf("增加4个学生，并打印\n");
  for (long i = 0; i < lp_da->eles_num; i++) {
    LPStudent s = (LPStudent)((char*)lp_da->eles + i * lp_da->ele_size);
    printf(" %s, %d\n", s->name, s->age);
  }

  LPDynaArray lp_poses = (LPDynaArray)malloc(sizeof(DynaArray));
  assert(lp_poses != NULL_POINTER);
  r = DynaArray_init(lp_poses, 10, sizeof(long), NULL_POINTER, NULL_POINTER,
                     NULL_POINTER);
  assert(r != FALSE);

  r = DynaArray_edit_by_ele(lp_da, &sdd, &sgg, lp_poses);
  assert(r == TRUE);

  // print inserted elements
  printf("修改弟弟的名字为哥哥的名字，年龄为哥哥的年龄, 并打印\n");
  for (long i = 0; i < lp_da->eles_num; i++) {
    LPStudent s = (LPStudent)((char*)lp_da->eles + i * lp_da->ele_size);
    printf(" %s, %d\n", s->name, s->age);
  }
  printf("可以看到dd 16被改为了GG 18\n");

  r = DynaArray_get_pos_by_ele(lp_da, &sgg, lp_poses);
  assert(r != FALSE);

  LPStudent out = (LPStudent)malloc(sizeof(Student));
  assert(NULL_POINTER != memset((char*)out, 0, sizeof(Student)));
  assert(out != NULL_POINTER);

  printf("根据索引获取元素\n");
  r = DynaArray_get_by_pos(lp_da, 0, out);
  assert(r != FALSE);

  printf(" pos 0, %s, %d\n", out->name, out->age);

  r = DynaArray_get_by_pos(lp_da, 1, out);
  assert(r != FALSE);

  printf(" pos 1, %s, %d\n", out->name, out->age);

  r = DynaArray_get_by_pos(lp_da, 2, out);
  assert(r != FALSE);

  printf(" pos 2, %s, %d\n", out->name, out->age);

  r = DynaArray_get_by_pos(lp_da, 3, out);
  assert(r != FALSE);

  printf(" pos 3, %s, %d\n", out->name, out->age);

  printf("获取索引4的学生信息，由于最多只有4个，所以返回false\n");
  r = DynaArray_get_by_pos(lp_da, 4, out);
  assert(r == FALSE);

  r = DynaArray_del_by_pos(lp_da, 0);
  r = DynaArray_del_by_pos(lp_da, lp_da->eles_num - 1);
  printf("删除第1个和第4个元素之后再打印\n");
  for (long i = 0; i < lp_da->eles_num; i++) {
    LPStudent s = (LPStudent)((char*)lp_da->eles + i * lp_da->ele_size);
    printf(" %s, %d\n", s->name, s->age);
  }

  r = DynaArray_free(lp_da);
  assert(r == TRUE);

  r = DynaArray_free(lp_poses);
  assert(r == TRUE);

  free(lp_da);
  free(lp_poses);
}

void test_linkedlist() {
  int r;
  LPLinkedList lp_ll = (LPLinkedList)malloc(sizeof(LinkedList));
  assert(lp_ll != NULL_POINTER);

  r = LinkedList_init(lp_ll, sizeof(Student), copy_func, compare_func,
                      free_func);
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
  r = LinkedList_insert(lp_ll, 0, &cw);
  r = LinkedList_insert(lp_ll, 2, &cw);
  // r = LinkedList_del_by_pos(lp_ll, 4);

  LPLinkedListNode lp_lln = lp_ll->lp_head;
  LPStudent s;
  printf("打印容器所有元素\n");
  for (long i = 0; i < lp_ll->eles_num; i++) {
    s = (LPStudent)lp_lln->ele;
    printf("  %s, %d\n", s->name, s->age);
    lp_lln = lp_lln->next;
  }

  Student oo;
  r = LinkedList_get_by_pos(lp_ll, lp_ll->eles_num - 1, &oo);
  assert(r != FALSE);
  printf("根据索引获取最后一个元素\n");
  printf(" %s, %d\n", oo.name, oo.age);

  printf("打印链表头和链表尾元素\n");
  if (lp_ll->eles_num != 0) {
    LPStudent o = (LPStudent)lp_ll->lp_head->ele;
    LPStudent u = (LPStudent)lp_ll->lp_tail->ele;
    printf(" h %s, %d t %s, %d\n", o->name, o->age, u->name, u->age);
  }

  LPDynaArray lp_poses = (LPDynaArray)malloc(sizeof(DynaArray));
  assert(lp_poses != NULL_POINTER);
  r = DynaArray_init(lp_poses, 10, sizeof(long), NULL_POINTER, NULL_POINTER,
                     NULL_POINTER);
  assert(r != FALSE);

  r = LinkedList_get_pos_by_ele(lp_ll, &cw, lp_poses);
  assert(r != FALSE);

  char* arr = (char*)lp_poses->eles;
  char* src;
  printf("根据元素获取元素的索引，由于容器中有两个cw，并且在末尾\n");
  for (long i = 0; i < lp_poses->eles_num; i++) {
    src = arr + lp_poses->ele_size * i;
    printf(" %d\n", *(long*)src);
  }

  r = LinkedList_del_by_pos(lp_ll, 0);
  r = LinkedList_del_by_pos(lp_ll, 3);
  r = LinkedList_del_by_pos(lp_ll, lp_ll->eles_num - 1);
  printf("删除第一个中间元素和最后一个元素再打印\n");
  lp_lln = lp_ll->lp_head;
  for (long i = 0; i < lp_ll->eles_num; i++) {
    s = (LPStudent)lp_lln->ele;
    printf("  %s, %d\n", s->name, s->age);
    lp_lln = lp_lln->next;
  }
  r = LinkedList_free(lp_ll);
  assert(r != FALSE);
  free(lp_ll);

  r = DynaArray_free(lp_poses);
  assert(r != FALSE);
  free(lp_poses);
}

void test_hashmap() {
  LPHashMap lp_hm = (LPHashMap)malloc(sizeof(HashMap));
  int r = HashMap_init(lp_hm, copy_func, compare_func, free_func);
  assert(r != FALSE);
  char* key = "abcdefg";
  long hash = HashMap_gen_hashcode(key, 7);
  printf("hash计算\n");
  printf(" %s %d\n", key, hash);
}

int main() {
  // 设置 UTF-8 编码的本地环境
  setlocale(LC_ALL, "zh_CN.UTF-8");

  printf("DynaArray tests..........\n");
  test_dynaarray();
  printf("LinkedList tests.........\n");
  test_linkedlist();
  printf("HashMap tests...............\n");
  test_hashmap();
  return 0;
}