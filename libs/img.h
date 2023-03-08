#include <errno.h>
#include <pwd.h>
#include <Imlib2.h>
#include <openssl/md5.h>

static void setimagesize(int width, int height);
static void setimageopts(void);
static void cleanupimage(void);
static void drawimage(void);
static void rotateimage(void);
static void flipimage(void);
static void loadimage(const char *file, int *width, int *height);
static void loadimagecache(const char *file, int *width, int *height);
static void resizetoimageheight(int imageheight);

static Imlib_Image image = NULL;
