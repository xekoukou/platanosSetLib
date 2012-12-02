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


#include "jlist.h"




int
comp_node_t (jnode_t * first, jnode_t * second)
{
    if (first->key > second->key) {
        return 1;
    }
    else {

        if (first->key < second->key) {
            return -1;
        }
        else {
            return 0;
        }
    }

}


jnode_t *
node_init (jnode_t * node, uint64_t key, uint64_t position)
{
    node->key = key;
    node->position = position;

    return node;
}



struct _jlist_t
{
    jnode_t *head;
    int max_height;
};


jlist_t *
jlist_new (int max_height)
{
    jlist_t *jlist = calloc (1, sizeof (jlist_t));
    jlist->head = calloc (1, sizeof (jnode_t));
    jlist->head = node_init (jlist->head, 0, 0);
    jlist->head->height = 1;
    jlist->head->next = calloc (max_height, sizeof (jnode_t *));
    jlist->max_height = max_height;
    return jlist;
}

void
jlist_destroy (jlist_t ** jlist)
{

    free ((*jlist)->head->next);
    free ((*jlist)->head);
    free (*jlist);
    jlist = NULL;

}

void
jlist_clear (jlist_t * jlist)
{
    jlist->head->next = NULL;
    jlist->head->height = 1;
}


int
jlist_add_ (jlist_t * jlist, int height, jnode_t * node, uint64_t key,
            uint64_t position)
{
    assert (key > 0);

    jnode_t *prev_list[height];
    node_init (node, key, position);

    int iter = height;
    jnode_t *ptr = jlist->head;
    while (iter) {
        if (!(ptr->next[iter - 1])) {
            prev_list[iter] = ptr;
            iter--;
        }
        else {
            int comp = comp_node_t (node, ptr->next[iter - 1]);
            if (comp == 1) {
                ptr = ptr->next[iter - 1];
            }
            else {
                if (comp == -1) {
                    prev_list[iter] = ptr;
                    iter--;
                }
                else {
                    return 0;
                }
            }
        }
    }

    node->height = 1;
    if ((node->height < jlist->max_height) && ((rand () % 4) == 0)) {
        node->height++;
    }

    for (iter = 1; iter <= height; iter++) {
        node->next[iter - 1] = prev_list[iter]->next[iter - 1];
        prev_list[iter]->next[iter - 1] = node;
    }

    return 1;

}

//returns 0 if the key already exists
int
jlist_add (jlist_t * jlist, jnode_t * node, uint64_t key, uint64_t position)
{
    return jlist_add_ (jlist, jlist->head->height, node, key, position);
}

int
jlist_delete_ (jlist_t * jlist, int height, uint64_t key)
{

    assert (key > 0);
    jnode_t node;
    node.key = key;

    jnode_t *prev_list[height];

    int iter = height;
    jnode_t *ptr = jlist->head;
    int comp = 1;
    while (iter) {
        if (!(ptr->next[iter - 1])) {
            prev_list[iter] = ptr;
            iter--;
        }
        else {
            comp = comp_node_t (&node, ptr->next[iter - 1]);
            if (comp == 1) {
                ptr = ptr->next[iter - 1];
            }
            else {
                prev_list[iter] = ptr;
                iter--;

            }
        }
    }

    if (!comp) {
        ptr = ptr->next[0];

        for (iter = 1; iter <= height; iter++) {
            prev_list[iter]->next[iter - 1] = ptr->next[iter - 1];
        }


        return 1;

    }
    else {
        return 0;

    }
}


int
jlist_delete (jlist_t * jlist, uint64_t key)
{
    return jlist_delete_ (jlist, jlist->head->height, key);
}




uint64_t
jlist_search (jlist_t * jlist, uint64_t key)
{
    if (jlist_lsearch (jlist, key, jlist->head, NULL)) {
        return key;
    }
    else {
        return 0;
    }

}

jnode_t *
jlist_lsearch (jlist_t * t, uint64_t key, jnode_t * llimit, jnode_t * ulimit)
{
    jnode_t node;
    node.key = key;

    int iter = llimit->height;
    if (ulimit) {
        if (ulimit->height < iter) {
            iter = ulimit->height;
        }
    }
    jnode_t *ptr = llimit;
    while (iter) {
        if (!(ptr->next[iter - 1])) {
            iter--;
        }
        else {
            int comp = comp_node_t (&node, ptr->next[iter - 1]);
            if (comp == 1) {
                ptr = ptr->next[iter - 1];
            }
            else {
                if (comp == -1) {
                    iter--;
                }
                else {
                    return ptr;
                }
            }
        }
    }

    return NULL;
}


void
jlist_test (int verbose)
{
    printf (" * jlist: ");
    jlist_t *list = jlist_new (10);
    jnode_t *array = calloc (100, sizeof (jnode_t));
    int iter;
    for (iter = 0; iter < 100; iter++) {
        array[iter].next = calloc (10, sizeof (jnode_t *));
    }

    uint64_t key[100];
    for (iter = 0; iter < 100; iter++) {
        key[iter] = rand () % 10000 + 1;
        jlist_add (list, array + iter, key[iter], 0);
    }
    if (verbose) {
        printf ("\nkey:%lu , search result:%lu", key[77],
                jlist_search (list, key[77]));
        printf ("\nkey:%lu , search result:%lu", key[33],
                jlist_search (list, key[33]));
        printf ("\nkey:%lu , search result:%lu", key[66],
                jlist_search (list, key[66]));
        printf ("\nkey:%lu , search result:%lu", key[22],
                jlist_search (list, key[22]));
    }
    assert (key[77] == jlist_search (list, key[77]));
    assert (key[33] == jlist_search (list, key[33]));
    assert (key[66] == jlist_search (list, key[66]));
    assert (key[22] == jlist_search (list, key[22]));

    assert (jlist_delete (list, key[77]));
    assert (0 == jlist_search (list, key[77]));

    assert (jlist_delete (list, key[33]));
    assert (0 == jlist_search (list, key[33]));

    jlist_destroy (&list);

    printf ("OK\n");
}
