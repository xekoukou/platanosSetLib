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



#ifndef SKIPLIST_H
#define SKIPLIST_H

#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>


#ifdef __cplusplus
extern "C"
{
#endif

    typedef struct _sklnode_t sklnode_t;

    typedef struct _sklist_t sklist_t;

//  Create a new skip list
    sklist_t *sklist_new (int max_height);

//  Destroy a skip list
    void sklist_destroy (sklist_t ** sklist);

//  Add a strictly postive key to the skip list, returns 0 if already present
    int sklist_add (sklist_t * sklist, uint64_t key);

//  Remove a key from the skip list, returns 0 if not present
    int sklist_delete (sklist_t * sklist, uint64_t key);

//  Search a key, returns the key or zero if not present
    uint64_t sklist_search (sklist_t * sklist, uint64_t key);

//  Search a key between 2 limits, returns the sklist node that contains it
    sklnode_t *sklist_lsearch (sklist_t * t, uint64_t key,
                               sklnode_t * llimit, sklnode_t * ulimit);


//  Selt test of this class
    void sklist_test (int verbose);



#ifdef __cplusplus
}
#endif





#endif
