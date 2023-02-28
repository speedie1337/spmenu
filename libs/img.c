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

    /* we din't find anything from cache, or it was just wrong */
	loadimage(file, width, height);
	if (!image) {
		free(buf);
		return;
	}

	scaleimage(width, height);
	imlib_image_set_format("png");
	createifnexist_rec(buf);
	imlib_save_image(buf);
	free(buf);
}
