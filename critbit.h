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





#ifndef CRITBIT_H_
#define CRITBIT_H_

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>


    typedef struct
    {
        void *child[2];
        uint32_t byte;
        uint8_t otherbits;
    } critbit0_node;


    typedef struct
    {
        void *root;
    } critbit0_tree;

    int critbit0_contains (critbit0_tree * t, const char *u);
    critbit0_node *critbit0_subtree_contains (critbit0_tree * t,
                                              critbit0_node * p,
                                              const char *u);
    int critbit0_insert (critbit0_tree * t, const char *u);
    int critbit0_delete (critbit0_tree * t, const char *u);
    void critbit0_clear (critbit0_tree * t);
    //less efficient critbit0_subtree_contains
    int critbit0_allprefixed (critbit0_tree * t, const char *prefix,
                              int (*handle) (const char *, void *),
                              void *arg);


#ifdef __cplusplus
}
#endif


#endif                          // CRITBIT_H_
