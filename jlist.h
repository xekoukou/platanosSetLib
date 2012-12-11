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

/*
this skiplist is used specifically by the join operation done on the columns 
of the cache of platanos
its main feauture is that all memory is externally given, and thus reused for
the next join

*/

#ifndef JLIST_H
#define JLIST_H
//look below after changing this
#define MAX_DIM_INTER 20

#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <assert.h>
#include <string.h>


#ifdef __cplusplus
extern "C"
{
#endif

    struct _jnode_t
    {
        struct _jnode_t *next;
        struct _jnode_t *prev;
        uint64_t key;
        uint8_t dim[MAX_DIM_INTER];     //used to be able to remove the nodes
//from the jlist and update the apropriate arrays
        uint8_t dim_size;
    };


    typedef struct _jnode_t jnode_t;
    //dim will be initialized by the array itself
    int comp_jnode_t (jnode_t * first, jnode_t * second);
    void jnode_init (jnode_t * node, uint64_t key, int dim);

    void jnode_clear (jnode_t * node);


    struct _jlist_t
    {
        jnode_t head;
    };



    typedef struct _jlist_t jlist_t;


//initss the skiplist,so as to be used by someone else
    void jlist_init (jlist_t * jlist, int max_height);

//the head always points at the last
    jnode_t *jlist_last (jlist_t * jlist);

//  Add a strictly postive key to the skip list, returns 0 if already present
//node is an externally provided memory
//it is assumed that a pointer is externally kept to that node
    jnode_t *jlist_add (jlist_t * jlist, jnode_t * after, jnode_t * node);

//  Remove a key from the skip list, returns 0 if not present
    int jlist_delete (jlist_t * jlist, jnode_t * node);


#ifdef __cplusplus
}
#endif





#endif
