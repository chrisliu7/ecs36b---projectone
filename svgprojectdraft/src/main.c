#include "svg.h"
#include <stdio.h>

// this function will write strings to our SVG File
svg_return_t write_fn(svg_user_context_ptr user, const char *text){
    FILE *fp = (FILE *)user;
    if(0 >= fprintf(fp,text)){
        return SVG_ERR_IO;
    }
    return SVG_OK;
}

// this function will close our SVG file after we finish writing to it
// if any error occurs with either of these functions it will return a non-zero value indicating an error occured
svg_return_t cleanup_fn(svg_user_context_ptr user){
    FILE *fp = (FILE *)user;
    if(fclose(fp)){
        return SVG_ERR_IO;
    }
    return SVG_OK;
}

int main(int argc, char *argv[]){
    FILE *fp = fopen("checkmark.svg","w");
    svg_point_t center = {50,50};
    svg_point_t start = {15,55}, middle = {35,75}, end = {80,30};
    svg_context_ptr context = svg_create(write_fn,cleanup_fn,(svg_user_context_ptr)fp,100,100);
    // drawing circle 
    svg_return_t return_value = svg_circle(context,&center,45,"fill:none; stroke:green; stroke-width:2");
    if(return_value){
        svg_destroy(context);
        return return_value;
    }
    return_value = svg_line(context,&start,&middle,"stroke:green; stroke-width:2");
    if(return_value){
        svg_destroy(context);
        return return_value;
    }
    return_value = svg_line(context,&middle,&end,"stroke:green; stroke-width:2");
    if(return_value){
        svg_destroy(context);
        return return_value;
    }
    svg_destroy(context);
    return 0;
}