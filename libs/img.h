#include <errno.h>
#include <pwd.h>
#include <Imlib2.h>
#include <openssl/md5.h>

static void setimageopts(void);

static Imlib_Image image = NULL;
