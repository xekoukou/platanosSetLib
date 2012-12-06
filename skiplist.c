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


#include "skiplist.h"



struct _sklnode_t
{
    int height;
    struct _sklnode_t **next;
    uint64_t key;
};


int
comp_node_t (sklnode_t * first, sklnode_t * second)
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


sklnode_t *
node_new (uint64_t key)
{
    sklnode_t *node = calloc (1, sizeof (sklnode_t));
    node->key = key;

    return node;
}

void
node_destroy (sklnode_t ** node)
{
    if ((*node)->next) {
        free ((*node)->next);
    }
    free (*node);
    node = NULL;
}





struct _sklist_t
{
    sklnode_t *head;
    int max_height;
};


sklist_t *
sklist_new (int max_height)
{
    sklist_t *sklist = malloc (sizeof (sklist_t));
    sklist->head = node_new (0);
    sklist->head->height = max_height;
    sklist->head->next = calloc (1, sizeof (sklnode_t *));
    sklist->max_height = max_height;
    return sklist;
}

void
sklist_destroy (sklist_t ** sklist)
{

    while ((*sklist)->head->next[0]) {
        sklist_delete (*sklist, (*sklist)->head->next[0]->key);
    }
    free ((*sklist)->head->next);
    free ((*sklist)->head);
    free (*sklist);
    sklist = NULL;

}


int
sklist_add_ (sklist_t * sklist, int height, uint64_t key)
{
    assert (key > 0);
    sklnode_t *node = node_new (key);

    sklnode_t *prev_list[height];

    int iter = height;
    sklnode_t *ptr = sklist->head;
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
                    node_destroy (&node);
                    return 0;
                }
            }
        }
    }

    node->height = 1;
    if ((node->height < sklist->max_height) && ((rand () % 4) == 0)) {
        node->height++;
    }

    node->next = calloc (node->height, sizeof (sklnode_t *));

    for (iter = 1; iter <= height; iter++) {
        node->next[iter - 1] = prev_list[iter]->next[iter - 1];
        prev_list[iter]->next[iter - 1] = node;
    }

    return 1;

}

//returns 0 if the key already exists
int
sklist_add (sklist_t * sklist, uint64_t key)
{
    return sklist_add_ (sklist, sklist->head->height, key);
}

int
sklist_delete_ (sklist_t * sklist, int height, uint64_t key)
{

    assert (key > 0);
    sklnode_t node;
    node.key = key;

    sklnode_t *prev_list[height];

    int iter = height;
    sklnode_t *ptr = sklist->head;
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
        node_destroy (&ptr);


        return 1;

    }
    else {
        return 0;

    }
}


int
sklist_delete (sklist_t * sklist, uint64_t key)
{
    return sklist_delete_ (sklist, sklist->head->height, key);
}




uint64_t
sklist_search (sklist_t * sklist, uint64_t key)
{
    if (sklist_lsearch (sklist, key, sklist->head, NULL)) {
        return key;
    }
    else {
        return 0;
    }

}

sklnode_t *
sklist_lsearch (sklist_t * t, uint64_t key,
                sklnode_t * llimit, sklnode_t * ulimit)
{
    sklnode_t node;
    node.key = key;

    int iter = llimit->height;
    if (ulimit) {
        if (ulimit->height < iter) {
            iter = ulimit->height;
        }
    }
    sklnode_t *ptr = llimit;
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
sklist_test (int verbose)
{
    printf (" * sklist: ");
    sklist_t *list = sklist_new (10);

    uint64_t key[100];
    int iter;
    for (iter = 0; iter < 100; iter++) {
        key[iter] = rand () % 10000 + 1;
        sklist_add (list, key[iter]);
    }
    if (verbose) {
        printf ("\nkey:%lu , search result:%lu", key[77],
                sklist_search (list, key[77]));
        printf ("\nkey:%lu , search result:%lu", key[33],
                sklist_search (list, key[33]));
        printf ("\nkey:%lu , search result:%lu", key[66],
                sklist_search (list, key[66]));
        printf ("\nkey:%lu , search result:%lu", key[22],
                sklist_search (list, key[22]));
    }
    assert (key[77] == sklist_search (list, key[77]));
    assert (key[33] == sklist_search (list, key[33]));
    assert (key[66] == sklist_search (list, key[66]));
    assert (key[22] == sklist_search (list, key[22]));

    assert (sklist_delete (list, key[77]));
    assert (0 == sklist_search (list, key[77]));

    assert (sklist_delete (list, key[33]));
    assert (0 == sklist_search (list, key[33]));

    sklist_destroy (&list);

    printf ("OK\n");
}
