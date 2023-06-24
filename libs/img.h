/* See LICENSE file for copyright and license details. */

#if USEIMAGE
#include <errno.h>
#include <pwd.h>
#include <Imlib2.h>
#include <openssl/evp.h>

static void setimagesize(int width, int height);
static void setimageopts(void);
static void cleanupimage(void);
static void drawimage(void);
static void flipimage(void);
static void loadimage(const char *file, int *width, int *height);
static void loadimagecache(const char *file, int *width, int *height);
static void resizetoimageheight(int imageheight);
#if USEWAYLAND
static void resizetoimageheight_wl(int imageheight);
#endif
#if USEX
static void resizetoimageheight_x11(int imageheight);
#endif
static void store_image_vars(void);

static Imlib_Image image = NULL;
#endif
