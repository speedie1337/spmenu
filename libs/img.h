/* See LICENSE file for copyright and license details. */

#if USEIMAGE
#include <errno.h>
#include <pwd.h>
#include <Imlib2.h>
#include <openssl/md5.h>

static void setimagesize(int width, int height);
#if USEX
static void setimageopts(void);
#endif
static void cleanupimage(void);
static void drawimage(void);
static void rotateimage(void);
static void flipimage(void);
static void loadimage(const char *file, int *width, int *height);
static void loadimagecache(const char *file, int *width, int *height);
static void resizetoimageheight(int imageheight);
static void store_image_vars(void);

static Imlib_Image image = NULL;
#endif
