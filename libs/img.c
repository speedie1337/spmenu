void
flipimage(void)
{
    switch (flip) {
        case 1: /* horizontal */
            imlib_image_flip_horizontal();
            break;
        case 2: /* vertical */
            imlib_image_flip_vertical();
            break;
        case 3: /* diagonal */
            imlib_image_flip_diagonal();
            break;
        default:
            flip = flip ? 1 : 0;
            return;
    }
}

void
rotateimage(void)
{
    rotation %= 4;
    imlib_image_orientate(rotation);
}

void
cleanupimage(void)
{
    if (image) {
        imlib_free_image();
        image = NULL;
    }
}

void
drawimage(void)
{
    int width = 0, height = 0;
    char *limg = NULL;

    if (!lines || hideimage) return;

    if (sel && sel->image && strcmp(sel->image, limg ? limg : "")) {
        if (longestedge)
            loadimagecache(sel->image, &width, &height);
    } else if ((!sel || !sel->image) && image) {
        imlib_free_image();
        image = NULL;
    } if (image && longestedge) {

        rotateimage();
        flipimage();

        int leftmargin = imagegaps;

       	if(mh != bh + height + imagegaps * 2) {
		    resizetoimageheight(height);
	    }

        if (!imageposition) { /* top mode = 0 */
            if (height > width)
                width = height;
            imlib_render_image_on_drawable(leftmargin+(imagewidth-width)/2, bh+imagegaps);
        } else if (imageposition == 1) { /* bottom mode = 1 */
            if (height > width)
                width = height;
            imlib_render_image_on_drawable(leftmargin+(imagewidth-width)/2, mh-height-imagegaps);
        } else if (imageposition == 2) { /* center mode = 2 */
            imlib_render_image_on_drawable(leftmargin+(imagewidth-width)/2, (mh-bh-height)/2+bh);
        } else {
            int minh = MIN(height, mh-bh-imagegaps*2);
            imlib_render_image_on_drawable(leftmargin+(imagewidth-width)/2, (minh-height)/2+bh+imagegaps);
        }


    } if (sel) {
        limg = sel->image;
    } else {
        limg = NULL;
    }
}

void
setimageopts(void)
{
    imlib_set_cache_size(8192 * 1024);
	imlib_context_set_blend(1);
	imlib_context_set_dither(1);
	imlib_set_color_usage(128);
	imlib_context_set_display(dpy);
	imlib_context_set_visual(visual);
	imlib_context_set_colormap(cmap);
	imlib_context_set_drawable(win);
}

void
createifnexist(const char *dir)
{
	if(access(dir, F_OK) == 0)
        return;

	if(errno == EACCES)
        fprintf(stderr, "spmenu: no access to create directory: %s\n", dir);

	if(mkdir(dir, S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH) == -1)
		fprintf(stderr, "spmenu: failed to create directory: %s\n", dir);
}

void
createifnexist_rec(const char *dir)
{
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

void
loadimage(const char *file, int *width, int *height)
{
	image = imlib_load_image(file);
	if (!image)
        return;

	imlib_context_set_image(image);

	*width = imlib_image_get_width();
	*height = imlib_image_get_height();
}

void
scaleimage(int *width, int *height)
{
	int nwidth, nheight;
	float aspect = 1.0f;

	if (imagewidth > *width)
		aspect = (float)(*width)/imagewidth;
	else
		aspect = (float)imagewidth/(*width);

	nwidth = *width * aspect;
	nheight = *height * aspect;

	if(nwidth == *width && nheight == *height)
        return;

	image = imlib_create_cropped_scaled_image(0,0,*width,*height,nwidth,nheight);

	imlib_free_image();

	if(!image)
        return;

	imlib_context_set_image(image);

	*width = nwidth;
	*height = nheight;

    return;
}

void
loadimagecache(const char *file, int *width, int *height)
{
	int slen = 0, i;
	unsigned char digest[MD5_DIGEST_LENGTH];
	char md5[MD5_DIGEST_LENGTH*2+1];
	char *xdg_cache, *home = NULL, *dsize, *buf;
	struct passwd *pw = NULL;

	/* just load and don't store or try cache */
	if (longestedge > 256) {
		loadimage(file, width, height);
        if (image)
            scaleimage(width, height);
		return;
	}

    if (generatecache) {
        /* try find image from cache first */
        if(!(xdg_cache = getenv("XDG_CACHE_HOME"))) {
            if(!(home = getenv("HOME")) && (pw = getpwuid(getuid())))
                home = pw->pw_dir;
            if(!home) {
                fprintf(stderr, "spmenu: could not find home directory");
                return;
            }
        }

        /* which cache do we try? */
        dsize = "normal";
        if (longestedge > 128)
            dsize = "large";

        slen = snprintf(NULL, 0, "file://%s", file)+1;

        if(!(buf = malloc(slen))) {
            fprintf(stderr, "spmenu: out of memory");
            return;
        }

        /* calculate md5 from path */
        sprintf(buf, "file://%s", file);
        MD5((unsigned char*)buf, slen, digest);

        free(buf);

        for(i = 0; i < MD5_DIGEST_LENGTH; ++i)
            sprintf(&md5[i*2], "%02x", (unsigned int)digest[i]);

        /* path for cached thumbnail */
        if (xdg_cache)
            slen = snprintf(NULL, 0, "%s/thumbnails/%s/%s.png", xdg_cache, dsize, md5)+1;
        else
            slen = snprintf(NULL, 0, "%s/.thumbnails/%s/%s.png", home, dsize, md5)+1;

        if(!(buf = malloc(slen))) {
            fprintf(stderr, "out of memory");
            return;
        }

        if (xdg_cache)
            sprintf(buf, "%s/thumbnails/%s/%s.png", xdg_cache, dsize, md5);
        else
            sprintf(buf, "%s/.thumbnails/%s/%s.png", home, dsize, md5);

        loadimage(buf, width, height);

        if (image && *width < imagewidth && *height < imageheight) {
            imlib_free_image();
            image = NULL;
        } else if(image && (*width > imagewidth || *height > imageheight)) {
            scaleimage(width, height);
        }

        /* we are done */
        if (image) {
            free(buf);
            return;
        }
    }

    /* we din't find anything from cache, or it was just wrong */
	loadimage(file, width, height);
	scaleimage(width, height);

    if (!generatecache) return;

	imlib_image_set_format("png");

    if (buf && generatecache) {
        createifnexist_rec(buf);
        imlib_save_image(buf);
        free(buf);
    }
}

void
jumptoindex(unsigned int index) {
	unsigned int i;
	sel = curr = matches;
	calcoffsets();
	for(i = 1; i < index; ++i) {
		if(sel && sel->right && (sel = sel->right) == next) {
			curr = next;
			calcoffsets();
		}
	}
}

void
resizetoimageheight(int imageheight)
{
	int omh = mh, olines = lines;
	lines = reallines;

	if(lines * bh < imageheight + imagegaps * 2)
        lines = (imageheight+imagegaps*2)/bh;

	mh = (lines + 1) * bh;

	if(mh - bh < imageheight + imagegaps * 2)
        mh = imageheight+imagegaps*2+bh;

	if(!win || omh == mh)
        return;

	XResizeWindow(dpy, win, mw, mh);
	drw_resize(drw, mw, mh);

	if (olines != lines) {
       	struct item *item;
		unsigned int i = 1;
		for (item = matches; item && item != sel; item = item->right)
            ++i;

		jumptoindex(i);
	}

	drawmenu();
}
