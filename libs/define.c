/* See LICENSE file for copyright and license details. */

#define INTERSECT(x,y,w,h,r)  (MAX(0, MIN((x)+(w),(r).x_org+(r).width)  - MAX((x),(r).x_org)) \
        && MAX(0, MIN((y)+(h),(r).y_org+(r).height) - MAX((y),(r).y_org)))
#define LENGTH(X)             (sizeof X / sizeof X[0])
#define TEXTW(X)              (drw_font_getwidth(drw, (X), False) + sp.lrpad)
#define TEXTWM(X)             (drw_font_getwidth(drw, (X), True) + sp.lrpad)
#define NUMBERSMAXDIGITS      100
#define NUMBERSBUFSIZE        (NUMBERSMAXDIGITS * 2) + 1
#define MAXITEMLENGTH         1024
