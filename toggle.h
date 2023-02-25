/* Toggle patches
 * This header allows you to enable/disable patches that can break compability or decrease performance with certain OSes or configurations.
 *
 * If it's disabled (set to 0), it will not be compiled in with spmenu. This keeps the binary small and may make the experience better.
 */

/* Image related patches */
#define USEIMAGE 1 /* Include image support. Requires imlib2 */
