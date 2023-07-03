/* See LICENSE file for copyright and license details. */

#if USEIMAGE
int draw_icon(struct item *item, int x, int y, int w, int h) {
    int ich = 0;
    int icw = 0;

    if (hideimage) return x;

    if (item->image) {
        image = imlib_load_image(item->image);

        if (!image) {
            return x;
        }

        imlib_context_set_image(image);

        icw = imlib_image_get_width();
        ich = imlib_image_get_height();

        image = imlib_create_cropped_scaled_image(0, 0, icw, ich, w, h); // w = h = bh - lrpad / 2

        icw = imlib_image_get_width();
        ich = imlib_image_get_height();

        imlib_free_image();
        imlib_context_set_image(image);

        if (!image) {
            return x;
        }

        imlib_image_set_format("png");

        // Draw the image
        draw_set_img(draw, imlib_image_get_data(), w, h);
        draw_img(draw, x, y);

        x += w;
    }

    return x;
}
#endif
