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
        int height;
        struct _jnode_t **next;
        uint64_t position;
        uint64_t key;
        uint8_t value;
        uint8_t size;           //size of key in varint;
        uint8_t dim;    //used to be able to remove the nodes
//from the jlist and update the apropriate arrays
    };


    typedef struct _jnode_t jnode_t;
    //dim will be initialized by the array itself
    int comp_jnode_t (jnode_t * first, jnode_t * second);
    jnode_t *jnode_init (jnode_t * node, uint64_t key, uint8_t size,
                        uint64_t position, uint8_t value);

    typedef struct _jlist_t jlist_t;

//  Create a new skip list
    jlist_t *jlist_new (int max_height);

//  Destroy a skip list
    void jlist_destroy (jlist_t ** jlist);

//clears the skiplist,so as to be used by someone else
    void jlist_clear (jlist_t * jlist);

jnode_t * jlist_first(jlist_t *jlist);

//  Add a strictly postive key to the skip list, returns 0 if already present
//node is an externally provided memory
//it is assumed that a pointer is externally kept to that node
    int jlist_add (jlist_t * jlist, jnode_t * node);

//  Remove a key from the skip list, returns 0 if not present
    int jlist_delete (jlist_t * jlist, uint64_t key);

//  Search a key, returns the key or zero if not present
    uint64_t jlist_search (jlist_t * jlist, uint64_t key);

//  Search a key between 2 limits, returns the sklist node that contains it
    jnode_t *jlist_lsearch (jlist_t * t, uint64_t key,
                            jnode_t * llimit, jnode_t * ulimit);


//  Selt test of this class
    void jlist_test (int verbose);



#ifdef __cplusplus
}
#endif





#endif
