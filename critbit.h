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
    } critbit_node;


    typedef struct
    {
        void *root;
    } critbit_tree;

    int critbit_contains (critbit_tree * t, const char *u, size_t ulen);

//parent is given the value of the last internal node
//p is the point where search starts
//return 1 on success 
    int critbit_scontains (critbit_tree * t,
                           critbit_node * p,
                           const char *u, size_t ulen, critbit_node ** parent);
    int critbit_insert (critbit_tree * t, const char *u, size_t ulen);
    int critbit_delete (critbit_tree * t, const char *u, size_t ulen);
    void critbit_clear (critbit_tree * t);


#ifdef __cplusplus
}
#endif


#endif                          // CRITBIT_H_
