/* See LICENSE file for copyright and license details. */

#if USEIMAGE
int draw_icon(struct item *item, int x, int y, int w, int h) {
    int ich = 0;
    int icw = 0;

    // cache
    int slen = 0, i;
    unsigned int digest_len = EVP_MD_size(EVP_md5());
    unsigned char *digest = (unsigned char *)OPENSSL_malloc(digest_len);
    char *xdg_cache, *home = NULL, *dsize, *buf = NULL;
    struct passwd *pw = NULL;

    if (hideimage) {
        return x;
    }

    if (item->image) {
        if (generatecache) {
            if(!(xdg_cache = getenv("XDG_CACHE_HOME"))) {
                if(!(home = getenv("HOME")) && (pw = getpwuid(getuid())))
                    home = pw->pw_dir;
                if(!home) {
                    fprintf(stderr, "spmenu: could not find home directory");
                    return x;
                }
            }

            dsize = "icon";

            slen = snprintf(NULL, 0, "file://%s", item->image)+1;

            if(!(buf = malloc(slen))) {
                return x;
            }

            sprintf(buf, "file://%s", item->image);

            EVP_MD_CTX *mdcontext = EVP_MD_CTX_new();
            EVP_DigestInit_ex(mdcontext, EVP_md5(), NULL);
            EVP_DigestUpdate(mdcontext, buf, slen);

            EVP_DigestFinal_ex(mdcontext, digest, &digest_len);
            EVP_MD_CTX_free(mdcontext);

            free(buf);

            char md5[digest_len*2+1];

            for (i = 0; i < digest_len; ++i)
                sprintf(&md5[i*2], "%02x", (unsigned int)digest[i]);

            if (!cachedir || !strcmp(cachedir, "default")) {
                if (xdg_cache || !strcmp(cachedir, "xdg"))
                    slen = snprintf(NULL, 0, "%s/thumbnails/%s/%s.png", xdg_cache, dsize, md5)+1;
                else
                    slen = snprintf(NULL, 0, "%s/.cache/thumbnails/%s/%s.png", home, dsize, md5)+1;
            } else {
                slen = snprintf(NULL, 0, "%s/%s/%s.png", cachedir, dsize, md5)+1;
            }

            if(!(buf = malloc(slen))) {
                return x;
            }

            if (!cachedir || !strcmp(cachedir, "default")) {
                if (xdg_cache)
                    sprintf(buf, "%s/thumbnails/%s/%s.png", xdg_cache, dsize, md5);
                else
                    sprintf(buf, "%s/.cache/thumbnails/%s/%s.png", home, dsize, md5);
            } else {
                    sprintf(buf, "%s/%s/%s.png", cachedir, dsize, md5);
            }

            image = imlib_load_image(buf);

            if (image) {
                imlib_context_set_image(image);

                icw = imlib_image_get_width();
                ich = imlib_image_get_height();
            }
        }

        if (!image || !generatecache) {
            image = imlib_load_image(item->image);

            if (!image) {
                return x;
            }

            imlib_context_set_image(image);

            icw = imlib_image_get_width();
            ich = imlib_image_get_height();

            if (!image) {
                return x;
            }

            image = imlib_create_cropped_scaled_image(0, 0, icw, ich, w, h); // w = h = bh - lrpad / 2
            imlib_free_image();
            imlib_context_set_image(image);

            icw = imlib_image_get_width();
            ich = imlib_image_get_height();
        }

        imlib_image_set_format("png");

        if (buf && generatecache && image) {
            createifnexist_rec(buf);
            imlib_save_image(buf);
            free(buf);
        }

        // Draw the image
        draw_set_img(draw, imlib_image_get_data(), w, h);
        draw_img(draw, x, y);

        x += w;
    }

    return x;
}
#endif
