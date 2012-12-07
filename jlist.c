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
comp_jnode_t (jnode_t * first, jnode_t * second)
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


void
jnode_init (jnode_t * node, uint64_t key
            )
{
    node->key = key;

}



struct _jlist_t
{
    jnode_t head;
    int max_height;
};



jnode_t *
jlist_first (jlist_t * jlist)
{
    return jlist->head.next[0];

}

void
jlist_init (jlist_t * jlist,int max_height)
{
    jnode_init (&(jlist->head), 0);
    jlist->head.height = max_height;


    int iter;
    for (iter = 0; iter < jlist->head.height; iter++) {
        jlist->head.next[iter] = NULL;
    }
}


int
jlist_add_ (jlist_t * jlist, int height, jnode_t * node)
{
    jnode_t *prev_list[height];

    int iter = height;
    jnode_t *ptr = &(jlist->head);
    while (iter) {
        if (!(ptr->next[iter - 1])) {
            prev_list[iter-1] = ptr;
            iter--;
        }
        else {
            int comp = comp_jnode_t (node, ptr->next[iter - 1]);
            if (comp == 1) {
                ptr = ptr->next[iter - 1];
            }
            else {
                if (comp == -1) {
                    prev_list[iter-1] = ptr;
                    iter--;
                }
                else {
//in essence we create a new list that has this node as a start
//searching till this node is also possible if at least one node remains in the skiplist
//we should either delete all or none
                    node->height = ptr->next[iter - 1]->height;
                    memcpy (node->next, ptr->next[iter - 1]->next,
                            node->height * sizeof (jnode_t *));
                    return 0;
                }
            }
        }
    }

    node->height = 1;
    if ((node->height <= jlist->max_height) && ((rand () % 4) == 0)) {
        node->height++;
    }

    for (iter = 1; iter <= node->height; iter++) {
        node->next[iter - 1] = prev_list[iter-1]->next[iter - 1];
        prev_list[iter-1]->next[iter - 1] = node;
    }

    return 1;

}

//returns 0 if the key already exists
int
jlist_add (jlist_t * jlist, jnode_t * node)
{
    return jlist_add_ (jlist, jlist->head.height, node);
}

int
jlist_delete_ (jlist_t * jlist, int height, uint64_t key)
{

    assert (key > 0);
    jnode_t node;
    node.key = key;

    jnode_t *prev_list[height];

    int iter = height;
    jnode_t *ptr = &(jlist->head);
    int comp = 1;
    while (iter) {
        if (!(ptr->next[iter - 1])) {
            prev_list[iter-1] = ptr;
            iter--;
        }
        else {
            comp = comp_jnode_t (&node, ptr->next[iter - 1]);
            if (comp == 1) {
                ptr = ptr->next[iter - 1];
            }
            else {
                prev_list[iter-1] = ptr;
                iter--;

            }
        }
    }

    if (!comp) {
        ptr = ptr->next[0];

        for (iter = 1; iter <= ptr->height; iter++) {
            prev_list[iter-1]->next[iter - 1] = ptr->next[iter - 1];
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
    return jlist_delete_ (jlist, jlist->head.height, key);
}




uint64_t
jlist_search (jlist_t * jlist, uint64_t key)
{
    if (jlist_lsearch (jlist, key, &(jlist->head), NULL)) {
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
            int comp = comp_jnode_t (&node, ptr->next[iter - 1]);
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
    jlist_t list;
    jlist_init (&list,10);
    jnode_t array[100];
    int iter;

    uint64_t key[100];
    for (iter = 0; iter < 100; iter++) {
        key[iter] = rand () % 10000 + 1;
        array[iter].key = key[iter];
        jlist_add (&list, array + iter);
    }
    if (verbose) {
        printf ("\nkey:%lu , search result:%lu", key[77],
                jlist_search (&list, key[77]));
        printf ("\nkey:%lu , search result:%lu", key[33],
                jlist_search (&list, key[33]));
        printf ("\nkey:%lu , search result:%lu", key[66],
                jlist_search (&list, key[66]));
        printf ("\nkey:%lu , search result:%lu", key[22],
                jlist_search (&list, key[22]));
    }
    assert (key[77] == jlist_search (&list, key[77]));
    assert (key[33] == jlist_search (&list, key[33]));
    assert (key[66] == jlist_search (&list, key[66]));
    assert (key[22] == jlist_search (&list, key[22]));

    assert (jlist_delete (&list, key[77]));
    assert (0 == jlist_search (&list, key[77]));

    assert (jlist_delete (&list, key[33]));
    assert (0 == jlist_search (&list, key[33]));


    printf ("OK\n");
}
