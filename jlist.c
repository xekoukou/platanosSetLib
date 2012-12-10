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
jnode_init (jnode_t * node, uint64_t key, int dim)
{
    node->key = key;
    node->dim[node->dim_size] = dim;
    node->dim_size++;

}

void
jnode_clear (jnode_t * node)
{

    node->dim_size = 0;
}





jnode_t *
jlist_last (jlist_t * jlist)
{
    return jlist->head.next;

}

void
jlist_init (jlist_t * jlist, int max_height)
{
    jnode_init (&(jlist->head), 0, 0);


    jlist->head.next = NULL;
}


int
jlist_add (jlist_t * jlist, jnode_t * after, jnode_t * node)
{

    jnode_t *ptr = after;
    while (ptr->next) {
        int comp = comp_jnode_t (node, ptr->next);
        if (comp == -1) {
            ptr = ptr->next;
        }
        else {
            if (comp == 1) {
                node->next = ptr->next;
                node->prev = ptr;
                ptr->next = node;

                return 1;
            }
            else {
//that assumes that any new node is cleared before added to the jlist
                jnode_init (ptr->next, node->key, node->dim[0]);
                return 0;
            }
        }
    }
    ptr->next = node;
    node->next = NULL;
    node->prev = ptr;
    return 1;

}

int
jlist_delete (jlist_t * jlist, jnode_t * node)
{
    node->prev->next = node->next;
    node->next->prev = node->prev;
    return 1;

}
