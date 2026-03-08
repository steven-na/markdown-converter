# MD to HTML converter

## Rules for converting markdown to HTML:

- `#`, `##`, and `###` become `<h1/2/3>` (no wrap)
- `---` becomes `<hr>` (no wrap, self closing)
- `*` or `_` becomes `<em>`
- `**` or `__` becomes `<strong>`
- `>` becomes `<blockquote>`
- `` ` `` becomes `<code>`
- ` ``` ` becomes `<pre><code>`
- number list becomes `<ol>\n<li>`
- unordered list becomes `<ul>\n<li>`
- `[display](url)` becomes `<a href="url">display</a>`
- `![alt-text](url)` becomes `<img src="url" alt="alt-text">`

- For wrapped markers, replace first occurrence with `<x>` and next with `</x>`
- For non-wrapped markers, replace first occurrence with `<x>` and insert `</x>` before newline

Order of operations: Markers which are subsets of other markers are formatted
last (`###` → `##` → `#`) to stop false identification.
