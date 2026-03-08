# MD to HTML converter

## rules for converting markdown to html:

- `#`, `##`, and `###` become `<h1/2/3>` (no wrap)
- `---` becomes '<hr>' (no wrap, self closing)
- `*` or `_` becomes `<em>`
- `**` or `__` becomes `<strong>`
- `>` becomes `<blockquote>`
- `\`` becomes '<code>'
- `\`\`\`` becomes '<pre><code>'
- number list becomes `<ol>\n<li>`
- unordered list becomes `<ul>\n<li>`
- `[display](url)` becomes `<a href="url">display</a>`
- `![alt-text](url)` becomes '<img src="B" alt="alt-text">'

- for wrapped markers, replace first occurence with `<x>` and next with `</x>`
- for non wrapped markers, replace first occurence with `<x>` and insert `</x>` before newline
Order of operations: Markers which are subsets of other markers are formatted last (### -> ## -> #) as to stop false identification.
