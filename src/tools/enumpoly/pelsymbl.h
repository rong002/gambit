/* *****************************************************************

   This file, and all other pel*.h and pel*.cc files in the Gambit
source code, are derived from the source code for Pelican, an
implementation of the Huber-Sturmfels algorithm for solving sparse
systems of polynomials written by Birk Huber.  That code in turn
incorporated the source code for HOMPACK and Qhull, which are included
here after being modified in the construction of both Pelican and
Gambit.  We are grateful to the authors of Pelican, Qhull, and
HOMPACK, for code that has been enormously useful in Gambit.
    More information about Qhull can be obtained from:

http://www.geom.edu/software/qhull/

    For more information concerning HOMPACK see:

http://netlib2.cs.utk.edu/hompack/

***************************************************************** */

/*
**    copyright (c) 1995  Birk Huber
*/

#ifndef SYMBOL_H
#define SYMBOL_H

#include <cstdlib>
#include <cstdio>

#include "pelgntyp.h"

typedef struct Sym_ent_tag *Sym_ent;
struct Sym_ent_tag{
    char *name;
    Gen_node def;
    Sym_ent next;
    int lock;};

Sym_ent Slookup(char *);
Gen_node Glookup();
Sym_ent install(char *s, Gen_node t);
Sym_ent lock(Sym_ent s);
int locked(Sym_ent s);
void empty_symbol_table();
void init_symbol_table();

#endif /* SYMBOL_H */
