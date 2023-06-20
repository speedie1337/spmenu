/* See LICENSE file for copyright and license details. */

#if USEIMAGE
void setimagesize(int width, int height) {
    if (!image || fullscreen || hideimage || height < 5 || width < 5 || width > mw) {
        return;
    }

    imageheight = height;
    imagewidth = width;
}

void flipimage(void) {
    // flip image
    switch (flip) {
        case 1: // horizontal
            imlib_image_flip_horizontal();
            break;
        case 2: // vertical
            imlib_image_flip_vertical();
            break;
        case 3: // diagonal
            imlib_image_flip_diagonal();
            break;
        default:
            flip = flip ? 1 : 0;
            return;
    }
}

void cleanupimage(void) {
    if (image) { // free image using imlib2
        imlib_free_image();
        image = NULL;
    }
}

void drawimage(void) {
    int width = 0, height = 0;
    char *limg = NULL;

    if (!lines || !columns || hideimage) return;

    // to prevent the image from being drawn multiple times wasting resources
    if (!needredraw) {
        needredraw = 1;
        return;
    }

    // load image cache
    if (sel && sel->image && strcmp(sel->image, limg ? limg : "")) {
        if (longestedge)
            loadimagecache(sel->image, &width, &height);
    } else if ((!sel || !sel->image) && image) { // free image
        cleanupimage();
    } if (image && longestedge && width && height) { // render the image
        flipimage();

        int leftmargin = imagegaps; // gaps between image and menu
        int wtr = 0; // remove from w
        int wta = 0; // add to w
        int xta = 0; // add to x

        if (hideprompt && hideinput && hidemode && hidematchcount && hidecaps) {
            wtr = bh;
        } else {
            wta = bh;
        }

        // margin
        xta += menumarginh;
        wta += menumarginv;

        if (mh != bh + height + leftmargin * 2 - wtr && imageresize) { // menu height cannot be smaller than image height
            resizetoimageheight(width, imlib_image_get_height() - (fullscreen ? 2 * menumarginv : 0));
        }

        drw_set_img(drw, imlib_image_get_data(), width, height);

        if (fullscreen) {
            xta = wta = leftmargin = 0;
            drw_img(drw, (imagewidth - width) / 2, 0);

            if (sel) {
                limg = sel->image;
            } else {
                limg = NULL;
            }

            return;
        }

        // render image on X11
        if (!imageposition && image) { // top mode = 0
            if (height > width)
                width = height;

            drw_img(drw, leftmargin + (imagewidth - width) / 2 + xta, wta + leftmargin);
        } else if (imageposition == 1 && image) { // bottom mode = 1
            if (height > width)
                width = height;

            drw_img(drw, leftmargin + (imagewidth - width) / 2 + xta, mh - height - leftmargin);
        } else if (imageposition == 2 && image) { // center mode = 2
            drw_img(drw, leftmargin + (imagewidth - width) / 2 + xta, (mh - wta - height) / 2 + wta);
        } else if (image) { // top center
            int minh = MIN(height, mh - bh - leftmargin * 2);
            drw_img(drw, leftmargin + (imagewidth - width) / 2 + xta, (minh - height) / 2 + wta + leftmargin);
        }
    }

    if (sel) {
        limg = sel->image;
    } else {
        limg = NULL;
    }
}

void setimageopts(void) {
    imlib_set_cache_size(8192 * 1024);
    imlib_set_color_usage(128);
}

void createifnexist(const char *dir) {
    // exists, so return
    if (access(dir, F_OK) == 0)
        return;

    // fatal: permission denied
    if (errno == EACCES)
        fprintf(stderr, "spmenu: no access to create directory: %s\n", dir);

    // mkdir() failure
    if (mkdir(dir, S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH) == -1)
        fprintf(stderr, "spmenu: failed to create directory: %s\n", dir);
}

void createifnexist_rec(const char *dir) {
    char *buf, *s = (char*)dir, *bs;

    if(!(buf = malloc(strlen(s)+1)))
        return;

    memset(buf, 0, strlen(s)+1);

    for(bs = buf; *s; ++s, ++bs) {
        if(*s == '/' && *buf)
            createifnexist(buf);

        *bs = *s;
    }

    free(buf);
}

void loadimage(const char *file, int *width, int *height) {
    if (!file) return;

    image = imlib_load_image(file);

    if (!image)
        return;

    imlib_context_set_image(image);

    *width = imlib_image_get_width();
    *height = imlib_image_get_height();
}

void scaleimage(int *width, int *height) {
    int new_width, new_height;
    float aspect = 1.0f;

    // depending on what size, we determine aspect ratio
    if (imagewidth > *width) {
        aspect = (float)(*width)/imagewidth;
    } else {
        aspect = (float)imagewidth/(*width);
    }

    new_width = *width * aspect;
    new_height = *height * aspect;

    if ((new_width == *width && new_height == *height) || (!image))
        return;

    image = imlib_create_cropped_scaled_image(0,0,*width,*height,new_width,new_height);

    imlib_free_image();

    if(!image)
        return;

    imlib_context_set_image(image);

    *width = new_width;
    *height = new_height;

    return;
}

void loadimagecache(const char *file, int *width, int *height) {
    int slen = 0, i;
    unsigned int digest_len = EVP_MD_size(EVP_md5());
    unsigned char *digest = (unsigned char *)OPENSSL_malloc(digest_len);
    char *xdg_cache, *home = NULL, *dsize, *buf = NULL;
    struct passwd *pw = NULL;

    // just load and don't store or try cache
    if (longestedge > maxcache) {
        loadimage(file, width, height);
        if (image)
            scaleimage(width, height);
        return;
    }

    if (generatecache) {
        // try find image from cache first
        if(!(xdg_cache = getenv("XDG_CACHE_HOME"))) {
            if(!(home = getenv("HOME")) && (pw = getpwuid(getuid())))
                home = pw->pw_dir;
            if(!home) {
                fprintf(stderr, "spmenu: could not find home directory");
                return;
            }
        }

        // which cache do we try?
        dsize = "normal";
        if (longestedge > 128)
            dsize = "large";

        slen = snprintf(NULL, 0, "file://%s", file)+1;

        if(!(buf = malloc(slen))) {
            fprintf(stderr, "spmenu: out of memory");
            return;
        }

        // calculate md5 from path
        sprintf(buf, "file://%s", file);

        EVP_MD_CTX *mdcontext = EVP_MD_CTX_new();
        EVP_DigestInit_ex(mdcontext, EVP_md5(), NULL);
        EVP_DigestUpdate(mdcontext, buf, slen);

        EVP_DigestFinal_ex(mdcontext, digest, &digest_len);
        EVP_MD_CTX_free(mdcontext);

        free(buf);

        char md5[digest_len*2+1];

        for (i = 0; i < digest_len; ++i)
            sprintf(&md5[i*2], "%02x", (unsigned int)digest[i]);

        // path for cached thumbnail
        if (!cachedir || !strcmp(cachedir, "default")) {
            if (xdg_cache || !strcmp(cachedir, "xdg"))
                slen = snprintf(NULL, 0, "%s/thumbnails/%s/%s.png", xdg_cache, dsize, md5)+1;
            else
                slen = snprintf(NULL, 0, "%s/.thumbnails/%s/%s.png", home, dsize, md5)+1;
        } else {
            slen = snprintf(NULL, 0, "%s/%s/%s.png", cachedir, dsize, md5)+1;
        }

        if(!(buf = malloc(slen))) {
            fprintf(stderr, "spmenu: out of memory");
            return;
        }

        if (!cachedir || !strcmp(cachedir, "default")) {
            if (xdg_cache)
                sprintf(buf, "%s/thumbnails/%s/%s.png", xdg_cache, dsize, md5);
            else
                sprintf(buf, "%s/.thumbnails/%s/%s.png", home, dsize, md5);
        } else {
                sprintf(buf, "%s/%s/%s.png", cachedir, dsize, md5);
        }

        loadimage(buf, width, height);

        if (image && *width < imagewidth && *height < imageheight) {
            imlib_free_image();
            image = NULL;
        } else if(image && (*width > imagewidth || *height > imageheight)) {
            scaleimage(width, height);
        }

        // we are done
        if (image) {
            free(buf);
            return;
        }
    }

    // we din't find anything from cache, or it was just wrong
    loadimage(file, width, height);
    scaleimage(width, height);

    if (!generatecache) return;

    if (image) imlib_image_set_format("png");

    if (buf && generatecache && image) {
        createifnexist_rec(buf);
        imlib_save_image(buf);
        free(buf);
    }
}

void jumptoindex(unsigned int index) {
    unsigned int i;
    sel = curr = matches;

    calcoffsets();

    for (i = 1; i < index; ++i) {
        if(sel && sel->right && (sel = sel->right) == next) {
            curr = next;
            calcoffsets();
        }
    }
}

void resizetoimageheight(int imagewidth, int imageheight) {
    int ih = imageheight;

#if USEX
    if (!protocol) {
        resizetoimageheight_x11(ih);
    } else {
#if USEWAYLAND
        resizetoimageheight_wl(ih);
#endif
    }
#elif USEWAYLAND
    resizetoimageheight_wl(ih);
#endif
}

#if USEX
void resizetoimageheight_x11(int imageheight) {
    int omh = mh, olines = lines;
    lines = reallines;

    int x, y;
#if USEXINERAMA
    int j, di, a, n, area = 0;
    XineramaScreenInfo *info;
    Window pw;
    unsigned int du;
    Window w, dw, *dws;
#endif
    XWindowAttributes wa;

    if (lines * bh < imageheight + imagegaps * 2) {
        lines = (imageheight + imagegaps * 2) / bh;

        if (fullscreen) {
            lines = imageheight / bh - 1;
        }
    }

    get_mh();

    // init xinerama screens
#if USEXINERAMA
    int i = 0;
    if (parentwin == root && (info = XineramaQueryScreens(dpy, &n))) {
        XGetInputFocus(dpy, &w, &di);
        if (mon >= 0 && mon < n) {
            i = mon;
        } else if (w != root && w != PointerRoot && w != None) {
            do {
                if (XQueryTree(dpy, (pw = w), &dw, &w, &dws, &du) && dws)
                    XFree(dws);
            } while (w != root && w != pw);
            if (XGetWindowAttributes(dpy, pw, &wa)) {
                for (j = 0; j < n; j++) {
                    if ((a = INTERSECT(wa.x, wa.y, wa.width, wa.height, info[j])) > area) {
                        area = a;
                        i = j;
                    }
                }
            }

            if (mon < 0 && !area && XQueryPointer(dpy, root, &dw, &dw, &x, &y, &di, &di, &du)) {
                for (i = 0; i < n; i++) {
                    if (INTERSECT(x, y, 1, 1, info[i])) {
                        break;
                    }
                }
            }
        }

        // calculate x/y position
        if (menuposition == 2) { // centered
            mw = MIN(MAX(max_textw() + promptw, minwidth), info[i].width);
            x = info[i].x_org + ((info[i].width  - mw) / 2);
            y = info[i].y_org + ((info[i].height - mh) / 2);
        } else { // top or bottom
            x = info[i].x_org + xpos;
            y = info[i].y_org + (menuposition ? 0 : info[i].height - mh - ypos);
            mw = (menuwidth > 0 ? menuwidth : info[i].width);
        }

        XFree(info);
    } else
#endif
    {
        if (!XGetWindowAttributes(dpy, parentwin, &wa))
            die("spmenu: could not get embedding window attributes: 0x%lx",
                    parentwin); // die because unable to get attributes for the parent window

        if (menuposition == 2) { // centered
            mw = MIN(MAX(max_textw() + promptw, minwidth), wa.width);
            x = (wa.width  - mw) / 2;
            y = (wa.height - mh) / 2;
        } else { // top or bottom
            x = 0;
            y = menuposition ? 0 : wa.height - mh - ypos;
            mw = (menuwidth > 0 ? menuwidth : wa.width);
        }
    }

    if (
        !win ||
        omh == mh) {
        return;
    }

    XMoveResizeWindow(dpy, win, x + sp, y + vp, mw - 2 * sp - borderwidth * 2, mh);
    drw_resize(drw, mw - 2 * sp - borderwidth, mh);

    if (olines != lines) {
        struct item *item;
        unsigned int i = 1;

        // walk through all matches
        for (item = matches; item && item != sel; item = item->right)
            ++i;

        jumptoindex(i);
    }

    drawmenu_layer();
}
#endif

#if USEWAYLAND
void resizetoimageheight_wl(int imageheight) {
    int omh = mh, olines = lines;
    lines = reallines;

    if (lines * bh < imageheight + imagegaps * 2) {
        lines = (imageheight + imagegaps * 2) / bh;

        if (fullscreen) {
            lines = imageheight / bh - 1;
        }
    }

    get_mh();

    if (omh == mh) {
        return;
    }

    if (olines != lines) {
        struct item *item;
        unsigned int i = 1;

        // walk through all matches
        for (item = matches; item && item != sel; item = item->right)
            ++i;

        jumptoindex(i);
    }

    state.width = mw;
    state.height = mh;

    state.buffer = create_buffer(&state);

    if (drw == NULL) {
        die("spmenu: drw == NULL");
    }

    if (state.buffer == NULL) {
        die("state.buffer == null");
    }

    set_layer_size(&state, state.width, state.height);
    drw_create_surface_wl(drw, state.data, state.width, state.height);

    drawmenu();

    wl_surface_set_buffer_scale(state.surface, 1);
	wl_surface_attach(state.surface, state.buffer, 0, 0);
	wl_surface_damage(state.surface, 0, 0, state.width, state.height);
	wl_surface_commit(state.surface);
}
#endif

void store_image_vars(void) {
    longestedge = MAX(imagewidth, imageheight);

    if (!imagew || !imageh || !imageg) {
        imagew = imagewidth;
        imageh = imageheight;
        imageg = imagegaps;
    }
}
#endif
