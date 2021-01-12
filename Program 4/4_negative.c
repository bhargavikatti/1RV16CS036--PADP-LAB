#include <stdio.h>
#include <gd.h>
#include <string.h>
#include <omp.h>


int main() {
    FILE *fp;
    gdImagePtr img;
   
    char iname[30] = "cartoon.png", oname[30] = "cartoon_negative.png";
    int color, x, y, h, i=0, w, red, green, blue;
    double t;

    if ((fp = fopen(iname, "r")) != NULL) {
        img = gdImageCreateFromPng(fp);
        w = gdImageSX(img);
        h = gdImageSY(img);
        t = omp_get_wtime();
        #pragma omp parallel for private(y, color,red,blue,green) schedule(static,10) num_threads(4) //schedule(dynamic,50) schedule(guided,50)
        for (x=0; x<w; x++) {
            //remove for data scoping
            #pragma omp critical    
            for (y=0; y<h; y++) {
                color = gdImageGetPixel(img, x, y);
                red = 255 - gdImageRed(img, color);
                blue = 255 - gdImageBlue(img, color);
                green = 255 -gdImageGreen(img, color);
                color = gdImageColorAllocate(img, red, green, blue);
                gdImageSetPixel(img, x, y, color);
            }
        }
        fclose(fp);    
        if ((fp = fopen(oname,"w")) != NULL) {
            gdImagePng(img, fp);
            fclose(fp);
        }
    }    

    t = omp_get_wtime() - t;
    gdImageDestroy(img);
    printf("Time Taken: %g\n", t);
    return 0;
}