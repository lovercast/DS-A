#include <stdio.h>
#include <stdlib.h>
#include <string.h>


typedef struct rb_tree {
    unsigned short color : 1;
    struct rb_tree *p;
    struct rb_tree *l;
    struct rb_tree *r;
    long key;
    long len;
    char value[];
} *rb_tree;

enum color {
    BLACK = 0,
    RED   = 1
};

const struct rb_tree t_nil = {
    .color=0,
    .p=NULL,
    .l=NULL,
    .r=NULL,
    .key=-1,
    .len=0,
    .value={}
};

rb_tree rb_insert(rb_tree root, long key, long len, char *value);
void rb_insert_fixup(rb_tree t);
rb_tree rb_new(long key, long len, char *value);
void *xmalloc(size_t size);

int main(int argc, char *argv[])
{
    long key = 42;
    char *value = "hello world";
    printf("%lu\n",strlen(value));
    long len = strlen(value); // strlen returns the number of characters that precede the terminating null
    rb_tree t = rb_new(key, len, value);
    return 0;
}

/* insert to where what item? */
/* assume len is not nul terminated  */
rb_tree rb_insert(rb_tree root, long key, long len, char *value)
{
    rb_tree t = rb_new(key, len, value);
    while (1) {
        if (key < root->key) {
            if (root->l == &t_nil) {
                root->l = t;
                t->p = root;
                break;
            } else {
                root = root->l;
            }
        } else {
            if (root->r == &t_nil) {
                root->r = t;
                t->p = root;
                break;
            } else {
                root = root->r;
            }
        }
    }
    return t;
}

rb_tree rb_new(long key, long len, char *value)
{
    rb_tree x = xmalloc(sizeof *x + len * sizeof *value);
    x->color = BLACK;
    x->p = &t_nil;
    x->l = &t_nil;
    x->r = &t_nil;
    x->key = key;
    x->len = len+1; // add nul byte
    strncpy(x->value, value, len+1);
    return x;
}

void *xmalloc(size_t size)
{
    void *ptr;
    if ((ptr = malloc(size)) == NULL) {
        perror("Malloc failed");
        exit(1);
    }
    return ptr;
}
