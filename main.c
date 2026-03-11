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

    u8 fStepCount = 13;
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
                             .escapable = true,
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
                             .escapable = false,
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
    fSteps[11] = (formatStep){.mdForm = "\n$$ ",
                              .htOpenForm = "\n\\[",
                              .htCloseForm = "\\]\n",
                              .wrapping = true,
                              .escapable = false,
                              .selfClosing = false};
    fSteps[12] = (formatStep){.mdForm = "$",
                              .htOpenForm = "\\(",
                              .htCloseForm = "\\)",
                              .wrapping = true,
                              .escapable = true,
                              .selfClosing = false};
    // wrapping: the Markdown has an identical closing form
    // escapable: if there is a `\` before the format chars, don't format
    // selfClosing: the html tag closes itself
    for (u8 step = 0; step < fStepCount; step++) {
        u32 hits = 0;
        u32 prev_offset = 0;
        u32 guaranteed_offset = 0;

        for (;;) {
            bool nonwrap_trying_to_close =
                !fSteps[step].wrapping && hits % 2 == 1;

            const char *search_term =
                nonwrap_trying_to_close ? "\n" : fSteps[step].mdForm;

            char *pos;
            size_t offset;
            for (;;) {
                u32 search_offset = guaranteed_offset > prev_offset
                                        ? guaranteed_offset
                                        : prev_offset;
                pos = strstr(*output + search_offset, search_term);
                if (!pos) {
                    break;
                }
                offset = pos - *output;
                if (fSteps[step].escapable && strncmp(pos - 1, "\\", 1) == 0) {
                    memmove(pos - 1, pos, outputLen - offset - 1);
                    outputLen -= 1;
                    guaranteed_offset = (pos + 1) - *output;
                    continue;
                } else {
                    break;
                }
            }
            if (!pos) {
                break;
            }

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

    // Image markers
    for (;;) {
        char *altFirstPos = strstr(*output, "\n![");
        if (!altFirstPos) {
            break;
        }
        size_t altOffset = altFirstPos - *output;
        char *altNextPos = strstr(*output + altOffset, "]");
        if (!altNextPos || *(altNextPos + 1) != '(') {
            break;
        }
        size_t altEndset = altNextPos - *output;
        char *altText;
        size_t altLen = altEndset - altOffset - 3;
        altText = calloc(1, altLen);
        if (!altText) {
            return -1;
        }
        memcpy(altText, *output + altOffset + 3, altLen);

        char *urlFirstPos = strstr(*output + altEndset, "(");
        if (!urlFirstPos) {
            free(altText);
            break;
        }
        size_t urlOffset = urlFirstPos - *output;
        char *urlNextPos = strstr(*output + urlOffset, ")");
        if (!urlNextPos) {
            free(altText);
            break;
        }
        size_t urlEndset = urlNextPos - *output;
        char *urlText;
        size_t urlLen = urlEndset - urlOffset - 1;
        urlText = calloc(1, urlLen);
        if (!urlText) {
            free(altText);
            break;
        }
        memcpy(urlText, *output + urlOffset + 1, urlLen);

        size_t initialLen = urlNextPos - altFirstPos + 1;

        char *replaceText;
        const char wrap1[] = "\n<img src=\"";
        const char wrap2[] = "\" alt=\"";
        const char wrap3[] = "\">";
        size_t replaceLen =
            strlen(wrap1) + urlLen + strlen(wrap2) + altLen + strlen(wrap3);
        replaceText = malloc(replaceLen);
        if (!replaceText) {
            free(urlText);
            free(altText);
            break;
        }
        char *p = replaceText;
        memcpy(p, wrap1, strlen(wrap1));
        p += strlen(wrap1);
        memcpy(p, urlText, urlLen);
        p += urlLen;
        memcpy(p, wrap2, strlen(wrap2));
        p += strlen(wrap2);
        memcpy(p, altText, altLen);
        p += altLen;
        memcpy(p, wrap3, strlen(wrap3));

        size_t tailLen = outputLen - altOffset - initialLen;

        if (replaceLen > initialLen) {
            *output =
                realloc(*output, outputLen + (replaceLen - initialLen) + 1);
            altFirstPos = *output + altOffset;
        }
        memmove(altFirstPos + replaceLen, altFirstPos + initialLen, tailLen);
        memcpy(altFirstPos, replaceText, replaceLen);
        outputLen += (replaceLen - initialLen);
        free(altText);
        free(urlText);
        free(replaceText);
    }

    // URL markers
    size_t url_search_offset = 0;
    for (;;) {
        char *altFirstPos = strstr(*output + url_search_offset, "[");
        if (!altFirstPos) {
            break;
        }
        size_t altOffset = altFirstPos - *output;
        char *altNextPos = strstr(*output + altOffset, "]");
        if (!altNextPos) {
            break;
        }
        if (*(altNextPos + 1) != '(') {
            url_search_offset = altOffset + 1;
            continue;
        }

        size_t altEndset = altNextPos - *output;

        if (altEndset <= altOffset + 1) {
            url_search_offset = altOffset + 1;
            continue;
        }

        char *altText;
        size_t altLen = altEndset - altOffset - 1;
        altText = calloc(1, altLen);
        if (!altText) {
            return -1;
        }
        memcpy(altText, *output + altOffset + 1, altLen);

        const char *urlFirstPos = altNextPos + 1;
        size_t urlOffset = urlFirstPos - *output;
        const char *urlNextPos = strstr(*output + urlOffset + 1, ")");
        if (!urlNextPos) {
            free(altText);
            break;
        }
        size_t urlEndset = urlNextPos - *output;
        char *urlText;
        size_t urlLen = urlEndset - urlOffset - 1;
        urlText = calloc(1, urlLen);
        if (!urlText) {
            free(altText);
            break;
        }
        memcpy(urlText, *output + urlOffset + 1, urlLen);

        size_t initialLen = urlNextPos - altFirstPos + 1;

        char *replaceText;
        const char wrap1[] = "<a href=\"";
        const char wrap2[] = "\">";
        const char wrap3[] = "</a>";
        size_t replaceLen =
            strlen(wrap1) + urlLen + strlen(wrap2) + altLen + strlen(wrap3);
        replaceText = malloc(replaceLen);
        if (!replaceText) {
            free(urlText);
            free(altText);
            break;
        }
        char *p = replaceText;
        memcpy(p, wrap1, strlen(wrap1));
        p += strlen(wrap1);
        memcpy(p, urlText, urlLen);
        p += urlLen;
        memcpy(p, wrap2, strlen(wrap2));
        p += strlen(wrap2);
        memcpy(p, altText, altLen);
        p += altLen;
        memcpy(p, wrap3, strlen(wrap3));

        size_t tailLen = outputLen - altOffset - initialLen;

        if (replaceLen > initialLen) {
            *output =
                realloc(*output, outputLen + (replaceLen - initialLen) + 1);
            altFirstPos = *output + altOffset;
        }
        memmove(altFirstPos + replaceLen, altFirstPos + initialLen, tailLen);
        memcpy(altFirstPos, replaceText, replaceLen);
        outputLen += (replaceLen - initialLen);
        url_search_offset = 0;
        free(altText);
        free(urlText);
        free(replaceText);
    }

    outputLen = strlen(*output) + 1;

    // wrap <p> </p> around text sections
    size_t offset = 0;
    for (;;) {
        char *pos = strstr(*output + offset, "\n");
        if (!pos) {
            break;
        }
        if (strncmp(pos + 1, "<pre><code>", 11) == 0) {
            char *end = strstr(pos + 1, "</code></pre>");
            if (end) {
                offset = (end + strlen("</code></pre>")) - *output;
                continue;
            }
        }
        offset = pos - *output + 1;
        if (strncmp(pos + 1, "\n", 1) != 0 &&
            strncmp(pos + 1, "<h1>", 4) != 0 &&
            strncmp(pos + 1, "<h2>", 4) != 0 &&
            strncmp(pos + 1, "<h3>", 4) != 0 &&
            strncmp(pos + 1, "<hr>", 4) != 0 &&
            strncmp(pos + 1, "<blockquote>", 12) != 0) {
            char *line_end = strstr(pos + 1, "\n");
            if (!line_end) {
                line_end = pos + 1 + strlen(pos + 1);
            }
            size_t line_len = line_end - (pos + 1);
            size_t insert_len = 7;
            size_t tail_offset = line_end - *output;
            size_t tail_len = strlen(line_end) + 1;

            outputLen += insert_len;
            *output = realloc(*output, outputLen);

            pos = *output + offset - 1;
            line_end = *output + tail_offset;

            memmove(line_end + insert_len, line_end, tail_len);
            memmove(pos + 1 + 3, pos + 1, line_len);
            memcpy(pos + 1, "<p>", 3);
            memcpy(pos + 1 + 3 + line_len, "</p>", 4);

            offset = (pos + 1 + 3 + line_len + 4) - *output;
        }
    }
    return outputLen;
}

int main() {
    char *markdown;
    i32 mFilesize = read_file_in(&markdown, "test.md");
    if (mFilesize == -1) {
        perror("failed to read test.md");
        return -1;
    }
    printf("--------------------Unformatted Md--------------------\n");
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
