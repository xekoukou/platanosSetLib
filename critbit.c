/*
    Copyright contributors as noted in the AUTHORS file.
                
    This file is part of PLATANOS.

    PLATANOS is free software; you can redistribute it and/or modify it under
    the terms of the GNU Affero General Public License as published by
    the Free Software Foundation; either version 3 of the License, or
    (at your option) any later version.
            
    PLATANOS is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Affero General Public License for more details.
        
    You should have received a copy of the GNU Affero General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/




#include"critbit.h"


int
critbit0_contains (critbit0_tree * t, const char *u)
{

    if (!t->root) {
        return 0;
    }

    return (NULL != critbit0_subtree_contains (t, t->root, u));

}

critbit0_node *
critbit0_subtree_contains (critbit0_tree * t, critbit0_node * p,
                           const char *u)
{
    const uint8_t *ubytes = (void *) u;
    const size_t ulen = strlen (u);

    critbit0_node *q;
    while (1 & (intptr_t) p) {
        q = (void *) (p - 1);

        uint8_t c = 0;
        if (q->byte < ulen) {
            c = ubytes[q->byte];
        }
        const int direction = (1 + (q->otherbits | c)) >> 8;


        p = q->child[direction];
    }


    if (0 == strcmp (u, (const char *) p)) {
        return ((void *) q + 1);
    }
    else {
        return NULL;
    }


}





int
critbit0_insert (critbit0_tree * t, const char *u)
{
    const uint8_t *const ubytes = (void *) u;
    const size_t ulen = strlen (u);
    uint8_t *p = t->root;


    if (!p) {
        char *x;
        int a = posix_memalign ((void **) &x, sizeof (void *), ulen + 1);
        if (a)
            return 0;
        memcpy (x, u, ulen + 1);
        t->root = x;
        return 2;
    }


    while (1 & (intptr_t) p) {
        critbit0_node *q = (void *) (p - 1);

        uint8_t c = 0;
        if (q->byte < ulen)
            c = ubytes[q->byte];
        const int direction = (1 + (q->otherbits | c)) >> 8;


        p = q->child[direction];
    }


    uint32_t newbyte;
    uint32_t newotherbits;

    for (newbyte = 0; newbyte < ulen; ++newbyte) {
        if (p[newbyte] != ubytes[newbyte]) {
            newotherbits = p[newbyte] ^ ubytes[newbyte];
            goto different_byte_found;
        }
    }

    if (p[newbyte] != 0) {
        newotherbits = p[newbyte];
        goto different_byte_found;
    }
    return 1;

  different_byte_found:


    newotherbits |= newotherbits >> 1;
    newotherbits |= newotherbits >> 2;
    newotherbits |= newotherbits >> 4;
    newotherbits = (newotherbits & ~(newotherbits >> 1)) ^ 255;
    uint8_t c = p[newbyte];
    int newdirection = (1 + (newotherbits | c)) >> 8;




    critbit0_node *newnode;
    if (posix_memalign
        ((void **) &newnode, sizeof (void *), sizeof (critbit0_node)))
        return 0;

    char *x;
    if (posix_memalign ((void **) &x, sizeof (void *), ulen + 1)) {
        free (newnode);
        return 0;
    }
    memcpy (x, ubytes, ulen + 1);

    newnode->byte = newbyte;
    newnode->otherbits = newotherbits;
    newnode->child[1 - newdirection] = x;


    void **wherep = &t->root;
    for (;;) {
        uint8_t *p = *wherep;
        if (!(1 & (intptr_t) p))
            break;
        critbit0_node *q = (void *) (p - 1);
        if (q->byte > newbyte)
            break;
        if (q->byte == newbyte && q->otherbits > newotherbits)
            break;
        uint8_t c = 0;
        if (q->byte < ulen)
            c = ubytes[q->byte];
        const int direction = (1 + (q->otherbits | c)) >> 8;
        wherep = q->child + direction;
    }

    newnode->child[newdirection] = *wherep;
    *wherep = (void *) (1 + (char *) newnode);





    return 2;
}


int
critbit0_delete (critbit0_tree * t, const char *u)
{
    const uint8_t *ubytes = (void *) u;
    const size_t ulen = strlen (u);
    uint8_t *p = t->root;
    void **wherep = &t->root;
    void **whereq = 0;
    critbit0_node *q = 0;
    int direction = 0;


    if (!p)
        return 0;


    while (1 & (intptr_t) p) {
        whereq = wherep;
        q = (void *) (p - 1);
        uint8_t c = 0;
        if (q->byte < ulen)
            c = ubytes[q->byte];
        direction = (1 + (q->otherbits | c)) >> 8;
        wherep = q->child + direction;
        p = *wherep;
    }



    if (0 != strcmp (u, (const char *) p))
        return 0;
    free (p);


    if (!whereq) {
        t->root = 0;
        return 1;
    }

    *whereq = q->child[1 - direction];
    free (q);



    return 1;
}


static void
traverse (void *top)
{

    uint8_t *p = top;

    if (1 & (intptr_t) p) {
        critbit0_node *q = (void *) (p - 1);
        traverse (q->child[0]);
        traverse (q->child[1]);
        free (q);
    }
    else {
        free (p);
    }


}

void
critbit0_clear (critbit0_tree * t)
{
    if (t->root)
        traverse (t->root);
    t->root = NULL;
}


static int
allprefixed_traverse (uint8_t * top,
                      int (*handle) (const char *, void *), void *arg)
{

    if (1 & (intptr_t) top) {
        critbit0_node *q = (void *) (top - 1);
        int direction;
        for (direction = 0; direction < 2; ++direction)
            switch (allprefixed_traverse (q->child[direction], handle, arg)) {
            case 1:
                break;
            case 0:
                return 0;
            default:
                return -1;
            }
        return 1;
    }


    return handle ((const char *) top, arg);

}

int
critbit0_allprefixed (critbit0_tree * t, const char *prefix,
                      int (*handle) (const char *, void *), void *arg)
{
    const uint8_t *ubytes = (void *) prefix;
    const size_t ulen = strlen (prefix);
    uint8_t *p = t->root;
    uint8_t *top = p;

    if (!p)
        return 1;

    while (1 & (intptr_t) p) {
        critbit0_node *q = (void *) (p - 1);
        uint8_t c = 0;
        if (q->byte < ulen)
            c = ubytes[q->byte];
        const int direction = (1 + (q->otherbits | c)) >> 8;
        p = q->child[direction];
        if (q->byte < ulen)
            top = p;
    }

    size_t i;
    for (i = 0; i < ulen; ++i) {
        if (p[i] != ubytes[i])
            return 1;
    }



    return allprefixed_traverse (top, handle, arg);
}
