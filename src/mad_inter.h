#ifndef MAD_INTER_H
#define MAD_INTER_H

// interface

/* Warning:
   these functions must be called close to each other
   they backup some states of the current node in global variables (side effects)
   that must be restored. Hence, node iteration are forbidden between their call.
*/
int interpolate_node(int *nint);
int reset_interpolation(void);

int start_interp_node(double* step, double* el);
int fetch_interp_node(double* dl);

#endif // MAD_INTER_H

