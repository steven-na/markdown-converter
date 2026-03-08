#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef int8_t i8;
typedef int16_t i16;
typedef int32_t i32;
typedef int64_t i64;
typedef uint8_t u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;

i32 read_file_in(char **, const char *);

typedef struct {
    char *mdForm;
    char *htOpenForm;
    char *htCloseForm;
    bool wrapping;
    bool escapable;
    bool selfClosing;
} formatStep;

i32 reformat_markdown(const char *markdown, char **output) {
    char attribution[] = "<!-- .md formatted to .html by steven-na -->\n";
    i8 attrLen = strlen(attribution);
    i32 outputLen = strlen(markdown) + 1 + attrLen;
    *output = calloc(1, outputLen);
    if (!*output) {
        return -1;
    }
    memcpy(*output, attribution, attrLen);
    memcpy(*output + attrLen, markdown, strlen(markdown));

    u8 fStepCount = 11;
    formatStep fSteps[fStepCount];
    fSteps[0] = (formatStep){.mdForm = "**",
                             .htOpenForm = "<strong>",
                             .htCloseForm = "</strong>",
                             .wrapping = true,
                             .escapable = true,
                             .selfClosing = false};
    fSteps[1] = (formatStep){.mdForm = "__",
                             .htOpenForm = "<strong>",
                             .htCloseForm = "</strong>",
                             .wrapping = true,
                             .escapable = true,
                             .selfClosing = false};
    fSteps[2] = (formatStep){.mdForm = "*",
                             .htOpenForm = "<em>",
                             .htCloseForm = "</em>",
                             .wrapping = true,
                             .escapable = true,
                             .selfClosing = false};
    fSteps[3] = (formatStep){.mdForm = "_",
                             .htOpenForm = "<em>",
                             .htCloseForm = "</em>",
                             .wrapping = true,
                             .escapable = true,
                             .selfClosing = false};
    fSteps[4] = (formatStep){.mdForm = "```\n",
                             .htOpenForm = "<pre><code>",
                             .htCloseForm = "</code></pre>\n",
                             .wrapping = true,
                             .escapable = false,
                             .selfClosing = false};
    fSteps[5] = (formatStep){.mdForm = "`",
                             .htOpenForm = "<code>",
                             .htCloseForm = "</code>",
                             .wrapping = true,
                             .escapable = true,
                             .selfClosing = false};
    fSteps[6] = (formatStep){.mdForm = "\n---\n",
                             .htOpenForm = "\n<hr>\n",
                             .htCloseForm = "debug",
                             .wrapping = false,
                             .escapable = false,
                             .selfClosing = true};
    fSteps[7] = (formatStep){.mdForm = "\n> ",
                             .htOpenForm = "\n<blockquote>",
                             .htCloseForm = "</blockquote>\n",
                             .wrapping = false,
                             .escapable = true,
                             .selfClosing = false};
    fSteps[8] = (formatStep){.mdForm = "\n### ",
                             .htOpenForm = "\n<h3>",
                             .htCloseForm = "</h3>\n",
                             .wrapping = false,
                             .escapable = false,
                             .selfClosing = false};
    fSteps[9] = (formatStep){.mdForm = "\n## ",
                             .htOpenForm = "\n<h2>",
                             .htCloseForm = "</h2>\n",
                             .wrapping = false,
                             .escapable = false,
                             .selfClosing = false};
    fSteps[10] = (formatStep){.mdForm = "\n# ",
                              .htOpenForm = "\n<h1>",
                              .htCloseForm = "</h1>\n",
                              .wrapping = false,
                              .escapable = false,
                              .selfClosing = false};

    // wrapping: the Markdown has an identical closing form
    // escapable: if there is a `\` before the format chars, don't format
    // selfClosing: the html tag closes itself
    for (u8 step = 0; step < fStepCount; step++) {
        u32 hits = 0;
        u32 prev_offset = 0;

        for (;;) {
            bool nonwrap_trying_to_close =
                !fSteps[step].wrapping && hits % 2 == 1;

            const char *search_term =
                nonwrap_trying_to_close ? "\n" : fSteps[step].mdForm;

            char *pos = strstr(*output + prev_offset, search_term);
            if (!pos)
                break;

            size_t offset = pos - *output;
            size_t mdLen = strlen(search_term);
            const char *htRep = hits % 2 == 0 ? fSteps[step].htOpenForm
                                              : fSteps[step].htCloseForm;
            size_t htLen = strlen(htRep);
            size_t tailLen = outputLen - offset - mdLen;

            if (htLen > mdLen) {
                *output = realloc(*output, outputLen + (htLen - mdLen) + 1);
                pos = *output + offset;
            }
            memmove(pos + htLen, pos + mdLen, tailLen);
            memcpy(pos, htRep, htLen);
            outputLen += (htLen - mdLen);

            hits += 1;
            if (fSteps[step].selfClosing) {
                hits = 0;
                prev_offset = 0;
            } else if (hits % 2 == 0) {
                prev_offset = 0;
            } else {
                prev_offset = offset + htLen;
            }
        }
    }
    return 0;
}

int main() {
    char *markdown;
    i32 mFilesize = read_file_in(&markdown, "test.md");
    if (mFilesize == -1) {
        perror("failed to read test.md");
        return -1;
    }
    printf("%s\n", markdown);

    char *html;
    i32 hFilesize = reformat_markdown(markdown, &html);
    if (hFilesize == -1) {
        perror("failed to format");
        free(markdown);
        return -1;
    }
    free(markdown);
    if (html) {
        printf("--------------------Formatted Html--------------------\n");
        printf("%s\n", html);
    }
}

i32 read_file_in(char **file_buffer, const char *filename) {
    FILE *fp = fopen(filename, "rb");
    if (!fp)
        perror("file load"), exit(1);

    fseek(fp, 0L, SEEK_END);
    long lSize = ftell(fp);
    rewind(fp);

    *file_buffer = calloc(1, lSize + 1);
    if (!*file_buffer) {
        fclose(fp);
        perror("memory alloc fails");
        return -1;
    }

    if (1 != fread(*file_buffer, lSize, 1, fp)) {
        fclose(fp);
        free(*file_buffer);
        perror("entire read fails");
        return -1;
    }
    fclose(fp);
    return lSize + 1;
}
