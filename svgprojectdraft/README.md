# Project X README

Student 1: Christopher Liu (925037747)

## Project Status
This the current status of the project ...

## Known Issues
The project fails to function properly in ...

## Code References
* [Unmet dependencies error when installing and utilizing Docker](https://unix.stackexchange.com/questions/724518/the-following-packages-have-unmet-dependencies-containerd-io)
* [Referencing pointers when printing](https://stackoverflow.com/questions/6799470/can-a-pointer-to-a-string-be-used-in-a-printf)

## Generative AI Use
I used ChatGPT for Prompt 1

### Prompt 1
svg_return_t svg_circle(svg_context_ptr context, const svg_point_t *center, svg_real_t radius, const char *style){ int size = snprintf(NULL, 0, "<circle cx=\"%lf\" cy=\"%lf\" r=\"%lf\" style=\"%s\"/>", *center->x, *center->y, *radius, *style); char* buffer = malloc(size + 1); snprintf(buffer, size, "<circle cx=\"%lf\" cy=\"%lf\" r=\"%lf\" style=\"%s\"/>", *center->x, *center->y, *radius, *style); context->write_fn(buffer); // actually writing in the svg context free(buffer); are my use of pointers here correct?

### Response 1
Short answer: no — your pointer usage is not correct, and you also have multiple snprintf mistakes. These bugs are exactly why your output is corrupt or crashing.

Let’s go line-by-line.

1. center is already a pointer — do NOT dereference its members

Your struct is:

const svg_point_t *center;


So center->x is already the value.

You wrote:

*center->x


This means:

*(center->x)


So now you're dereferencing whatever x points to — which is not a pointer — undefined behavior.

❌ Wrong
*center->x

✅ Correct
center->x


Same mistake for center->y.

2. radius is NOT a pointer — do not dereference it

You declared:

svg_real_t radius


But you used:

*radius


This is illegal unless radius is a pointer, which it is not.

❌ Wrong
*radius

✅ Correct
radius

3. style is a string pointer — %s expects the pointer itself

You passed:

*style


That gives only the first character of the string and treats it as a pointer — instant crash.

❌ Wrong
*style

✅ Correct
style

4. Your second snprintf size is wrong

You wrote:

snprintf(buffer, size, ...);


This truncates the last character and does not write the null terminator.

Must be:
snprintf(buffer, size + 1, ...);

5. Your fixed function

Here is the correct version:

svg_return_t svg_circle(svg_context_ptr context,
                        const svg_point_t *center,
                        svg_real_t radius,
                        const char *style)
{
    if (!context || !center || !style)
        return SVG_ERR_INVALID_ARG;

    int size = snprintf(NULL, 0,
        "<circle cx=\"%lf\" cy=\"%lf\" r=\"%lf\" style=\"%s\"/>",
        center->x, center->y, radius, style);

    char *buffer = malloc(size + 1);
    if (!buffer)
        return SVG_ERR_NO_MEM;

    snprintf(buffer, size + 1,
        "<circle cx=\"%lf\" cy=\"%lf\" r=\"%lf\" style=\"%s\"/>",
        center->x, center->y, radius, style);

    context->write_fn(buffer);
    free(buffer);

    return SVG_OK;
}

Why yours breaks
Bug	Effect
*center->x	Dereferences garbage memory
*radius	Dereference of non-pointer
*style	%s reads from random memory
snprintf(buffer, size, ...)	Missing null terminator

This is why your SVG output is corrupted or segfaulting.
### Chagnes 1
I changed how I referenced the parameters in the function from using the pointer asterisk to nothing
// AI Generated
for (char c : bar) {
```

### Prompt 2
My other prompt...

### Response 2
The other response...

### Changes 2
The other changes made...