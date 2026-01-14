/**
 * @file svg.c
 * @brief Implementation of simple SVG drawing interface.
 *
 * Implements the basic functions for creating SVG documents.
 */
#include "svg.h"
#include <stdlib.h>
#include <stdio.h>


/**
 * @brief Opaque SVG drawing context.
 *
 * Holds the necessary data to implement functions.
 */
struct SVG_CONTEXT{
    svg_write_fn write_fn;
    svg_cleanup_fn cleanup_fn;
    svg_user_context_ptr user;
};


// Callback used to write SVG output.
typedef svg_return_t (*svg_write_fn)(svg_user_context_ptr user,
const char *text);

// Callback used to clean up user resources.
typedef svg_return_t (*svg_cleanup_fn)(svg_user_context_ptr user);


// Creates a new SVG drawing context.
svg_context_ptr svg_create(svg_write_fn write_fn, 
                           svg_cleanup_fn cleanup_fn, 
                           svg_user_context_ptr user, 
                           svg_px_t width, 
                           svg_px_t height){
    if (write_fn == NULL || width <= 0 || height <= 0) {
        return NULL;
    } else {
// initializing svg context
    svg_context_ptr context = (svg_context_ptr)malloc(sizeof(svg_context_t));
    context->write_fn = write_fn;
    context->cleanup_fn = cleanup_fn;
    context->user = user;

// C snprintf(str, size, format, ...); 
// writing out xml code
    int size = snprintf(NULL, 0, 
        "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n<svg width=\"%d\" heights=\"%d\" xmlns=\"http://www.w3.org/2000/svg\">",
        width, height);
    char* buffer = malloc(size + 1);
    snprintf(buffer, size, 
        "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n<svg width=\"%d\" heights=\"%d\" xmlns=\"http://www.w3.org/2000/svg\">",
        width, height);
    context->write_fn(context->user, buffer); // actually writing in the svg context
    free(buffer);
    return context;
    }
/*
on top of initializing our context, 
we want to write two lines of xml to our file here:
<?xml version="1.0" encoding="UTF-8"?>
<svg width="100" height="100" xmlns="http://www.w3.org/2000/svg">   
*/
}

// Destroys an SVG context.
svg_return_t svg_destroy(svg_context_ptr context){
    // Enter code here
    // for if context is not NULL 
    if(context){ 
    // will generate </svg>
    context->write_fn(context->user, "</svg>\n");
    if (context->cleanup_fn) {
        context->cleanup_fn(context->user);
    } // cleanup used here? idfk
        free(context);
        return SVG_OK;
    } else {
    return SVG_ERR_NULL;
    }
}

// Draws a circle.
svg_return_t svg_circle(svg_context_ptr context,
                        const svg_point_t *center,
                        svg_real_t radius,
                        const char *style){
    // Enter code here

/*
    context->write_fn("<circle cx=\"%lf\" ", center->x);
    context->write_fn("cy=\"%lf\" ", center->y);
    context->write_fn("r=\"%lf\" ", *radius);
    // The "%s" format specifier for printf always expects a char* argument.
    context->write_fn("style=\"%s\"/>", style); 
*/  
    // C snprintf(str, size, format, ...);
    if (!(context)) {
        return SVG_ERR_NULL;
    } 
    else if (radius == 0) {
        return SVG_ERR_INVALID_ARG;
    }
    else if (center == NULL) {
        return SVG_ERR_INVALID_ARG;
    }
    // for if style is NULL 
    else if(style != NULL){ 
    int circle_size = snprintf(NULL, 0, 
        "<circle cx=\"%lf\" cy=\"%lf\" r=\"%lf\" style=\"%s\"/>\n",
        center->x, center->y, radius, style);
    char* circle_buffer = malloc(circle_size + 1);
    snprintf(circle_buffer, circle_size, 
        "<circle cx=\"%lf\" cy=\"%lf\" r=\"%lf\" style=\"%s\"/>\n",
        center->x, center->y, radius, style);
    context->write_fn(context->user, circle_buffer); // actually writing in the svg context
    free(circle_buffer);
    return SVG_OK;
    } else {
    int circle_size = snprintf(NULL, 0, 
        "<circle cx=\"%lf\" cy=\"%lf\" r=\"%lf\"\n",
        center->x, center->y, radius);
    char* circle_buffer = malloc(circle_size + 1);
    snprintf(circle_buffer, circle_size, 
        "<circle cx=\"%lf\" cy=\"%lf\" r=\"%lf\"\n",
        center->x, center->y, radius);
    context->write_fn(context->user, circle_buffer); // actually writing in the svg context
    free(circle_buffer);
    return SVG_OK;
    }
    // returns 0 indicating circle was drawn sucessfully
    }

/*
thing in main.c
svg_return_t return_value = 
svg_circle(context,&center,45,"fill:none; stroke:green; stroke-width:2");

for this function we want to write the following xml code to our SVG file:

<circle cx="50.000000" cy="50.000000" r="45.000000" style="fill:none; stroke:green; stroke-width:2"/>
*/


// Draws a rectangle.
svg_return_t svg_rect(svg_context_ptr context,
                      const svg_point_t *top_left,
                      const svg_size_t *size,
                      const char* style){
    // Enter code here
    // for if style is NULL 
    if (!(context)) {
        return SVG_ERR_NULL;
    } 
    else if (top_left == NULL) {
        return SVG_ERR_INVALID_ARG;
    }
    else if(style == NULL){ 
    int rect_size = snprintf(NULL, 0, 
        "<rect x=\"%lf\" y=\"%lf\" width=\"%lf\" height=\"%lf\"\n",
        top_left->x, top_left->y, size->width, size->height);
    char* rect_buffer = malloc(rect_size + 1);
    snprintf(rect_buffer, rect_size, 
        "<rect x=\"%lf\" y=\"%lf\" width=\"%lf\" height=\"%lf\"\n",
        top_left->x, top_left->y, size->width, size->height);
    context->write_fn(context->user, rect_buffer); // actually writing in the svg context
    free(rect_buffer);
    return SVG_OK;
    } else {
    int rect_size = snprintf(NULL, 0, 
        "<rect x=\"%lf\" y=\"%lf\" width=\"%lf\" height=\"%lf\" style=\"%s\"/>\n",
        top_left->x, top_left->y, size->width, size->height, style);
    char* rect_buffer = malloc(rect_size + 1);
    snprintf(rect_buffer, rect_size, 
        "<rect x=\"%lf\" y=\"%lf\" width=\"%lf\" height=\"%lf\" style=\"%s\"/>\n",
        top_left->x, top_left->y, size->width, size->height, style);
    context->write_fn(context->user, rect_buffer); // actually writing in the svg context
    free(rect_buffer);
    return SVG_OK;
    }

    // returns 0 indicating rectangle was drawn sucessfully
/* 
<rect x="50.000000" y="50.000000" width="30.000000" height="40.000000" style="fill:none; stroke:green; stroke-width:2"/>

we are given our x and y as a svg_point_t, width and height as a svg_size_t, and want to use those
*/
}

// Draws a line segment.
svg_return_t svg_line(svg_context_ptr context,
                      const svg_point_t *start,
                      const svg_point_t *end,
                      const char* style){
    // Enter code here
/*
    context->write_fn("<line x1=\"%lf\" ", start->x);
    context->write_fn("y1=\"%lf\" ", start->y);
    context->write_fn("x2=\"%lf\" ", end->x);
    context->write_fn("y2=\"%lf\" ", end->y);
    context->write_fn("style=\"%s\"/>", style); 
*/
    if (!(context)) {
        return SVG_ERR_NULL;
    } 
    else if (start == NULL || end == NULL) {
        return SVG_ERR_INVALID_ARG;
    }
    else if(style == NULL){ 
    int line_size = snprintf(NULL, 0, 
        "<line x1=\"%lf\" y1=\"%lf\" x2=\"%lf\" y2=\"%lf\"\n",
        start->x, start->y, end->x, end->y);
    char* line_buffer = malloc(line_size + 1);
    snprintf(line_buffer, line_size, 
        "<line x1=\"%lf\" y1=\"%lf\" x2=\"%lf\" y2=\"%lf\"\n",
        start->x, start->y, end->x, end->y);
    context->write_fn(context->user, line_buffer); // actually writing in the svg context
    free(line_buffer);
    return SVG_OK;
    } else {
    int line_size = snprintf(NULL, 0, 
        "<line x1=\"%lf\" y1=\"%lf\" x2=\"%lf\" y2=\"%lf\" style=\"%s\"/>\n",
        start->x, start->y, end->x, end->y, style);
    char* line_buffer = malloc(line_size + 1);
    snprintf(line_buffer, line_size, 
        "<line x1=\"%lf\" y1=\"%lf\" x2=\"%lf\" y2=\"%lf\" style=\"%s\"/>\n",
        start->x, start->y, end->x, end->y, style);
    context->write_fn(context->user, line_buffer); // actually writing in the svg context
    free(line_buffer);
    return SVG_OK;
    }
    // returns 0 indicating line segment was drawn sucessfully
/*
write
<line x1="10.000000" y1="10.000000" x2="90.000000" y2="90.000000" style="stroke:green; stroke-width:2"/>
we are given our x1 and y1 as a svg_point_t, x2 and y2 as a svg_point_t, and want to use those

*/
}

// Begins an SVG group.
svg_return_t svg_group_begin(svg_context_ptr context, 
                             const char* attrs){
    // Enter code here
    // context->write_fn("<g %s>", attrs);
    if (!(context)) {
        return SVG_ERR_NULL;
    }    else if (attrs != NULL) {
    int size = snprintf(NULL, 0, "<g %s>", attrs);
    char* buffer = malloc(size + 1);
    snprintf(buffer, size, "<g %s>", attrs);
    context->write_fn(context->user, buffer); // actually writing in the svg context
    free(buffer);
    return SVG_OK;
    } else {
        return SVG_ERR_IO;
    }

/*
for this function,
write following xml code to our SVG file:
<g stroke:blue; stroke-width:2>
*/
}

// Ends the current SVG group.
svg_return_t svg_group_end(svg_context_ptr context){
    // Enter code here
    if (!(context)) {
        return SVG_ERR_NULL;
    } else {
    int size = snprintf(NULL, 0, "<g>");
    char* buffer = malloc(size + 1);
    snprintf(buffer, size, "<g>");
    context->write_fn(context->user, buffer); // actually writing in the svg context
    free(buffer);   
    }
    return 0;
/*
for this function,
simply write a closing </g> tag to our SVG file
*/
}
