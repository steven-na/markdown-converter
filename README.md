# MD to HTML converter

## rules for converting markdown to html:

- `#`, `##`, and `###` become `<h1>`, `<h2>`, `<h3>` (no wrap)
- `---` becomes `<hr>` (no wrap, self closing)
- `*` or `_` becomes `<em>`
- `**` or `__` becomes `<strong>`
- `>` becomes `<blockquote>` (no wrap)
- `` ` `` becomes `<code>`
- ` ``` ` becomes `<pre><code>`
- `[display](url)` becomes `<a href="url">display</a>`
- `![alt-text](url)` becomes `<img src="url" alt="alt-text">`

- for wrapped markers, replace first occurence with `<x>` and next with `</x>`
- for non wrapped markers, replace first occurence with `<x>` and insert `</x>` before newline
  Order of operations: Markers which are subsets of other markers are formatted last (### -> ## -> #) as to stop false identification. Next, wrap all text blocks that sit on new lines with `<p>` and `</p>`

## Performance:

<details>
```
--------------------Unformatted Md--------------------
# Heading Level 1

## Heading Level 2

### Heading Level 3

---

## Basic Inline Formatting

This is **bold with double asterisks** and **bold with double underscores**.
This is _italic with single asterisk_ and _italic with single underscore_.
This is **bold and _nested italic_ inside bold**.
This is _italic and **nested bold** inside italic_.

Escaped formatting: \*not italic\* and \*\*not bold\*\* and \_not italic\_.

---

## Code

Inline `code snippet` in a sentence.
Another inline `const x = 42;` example.

```
function greet(name) {
    return "Hello, " + name + "!";
}
console.log(greet("world"));
```

---

## Blockquotes

> This is a simple blockquote.

> This blockquote contains **bold** and _italic_ and `inline code` inside it.

---

## Links

[Simple link](https://example.com)
[Link with path](https://example.com/some/deep/path?query=value&other=123)
[Another link](https://openai.com)

---

## Images

![A random cat](https://cataas.com/cat?type=square)
![Another cat](https://cataas.com/cat?width=200&height=200)

---

## Edge Cases

**Bold at start of line**
_Italic at start of line_
Line ending in bold **bold end**
Line ending in italic _italic end_

Two **bold** words **in one** line.
Two _italic_ words _in one_ line.
Mix of **bold** and _italic_ and `code` all in one sentence.

A line with a [link](https://example.com) embedded mid-sentence, not at start.

Empty brackets edge: []() should produce an empty anchor.

---

## Adjacent Formatting

**bold**_italic_ right next to each other.
`code`**bold** no space between.

---

## Horizontal Rules

Before rule.

---

After rule. Another one coming.

---

Done.

---

## Deeply Nested Content

> Blockquote with **bold**, _italic_, `code`, and a [link](https://example.com) all inside.

---

## Repeated Patterns

**one** **two** **three**
_one_ _two_ _three_
`one` `two` `three`
[a](https://a.com) [b](https://b.com) [c](https://c.com)

---

## Long Line

This is a very long line with **bold text appearing multiple times**, and also _italic text appearing multiple times_, and some `inline code blocks` scattered throughout, plus a [hyperlink to example](https://example.com) somewhere in the middle, and it just keeps going with more **bold** and more _italic_ until it finally ends.

---

## Images Mixed with Links

![Image one](https://cataas.com/cat?1)
[Link after image](https://example.com)
![Image two](https://cataas.com/cat?2)
[Another link](https://example.org)

--------------------Formatted Html--------------------

<!-- .md formatted to .html by steven-na -->
<h1>Heading Level 1</h1>

<h2>Heading Level 2</h2>

<h3>Heading Level 3</h3>

<hr>

<h2>Basic Inline Formatting</h2>

<p>This is <strong>bold with double asterisks</strong> and <strong>bold with double underscores</strong>.</p>
<p>This is <em>italic with single asterisk</em> and <em>italic with single underscore</em>.</p>
<p>This is <strong>bold and <em>nested italic</em> inside bold</strong>.</p>
<p>This is <em>italic and <strong>nested bold</strong> inside italic</em>.</p>

<p>Escaped formatting: *not italic* and **not bold** and _not italic_.</p>

<hr>

<h2>Code</h2>

<p>Inline <code>code snippet</code> in a sentence.</p>
<p>Another inline <code>const x = 42;</code> example.</p>

<pre><code>function greet(name) {
    return "Hello, " + name + "!";
}
console.log(greet("world"));
</code></pre>

<hr>

<h2>Blockquotes</h2>

<blockquote>This is a simple blockquote.</blockquote>

<blockquote>This blockquote contains <strong>bold</strong> and <em>italic</em> and <code>inline code</code> inside it.</blockquote>

<hr>

<h2>Links</h2>

<p><a href="https://example.com">Simple link</a></p>
<p><a href="https://example.com/some/deep/path?query=value&other=123">Link with path</a></p>
<p><a href="https://openai.com">Another link</a></p>

<hr>

<h2>Images</h2>

<p><img src="https://cataas.com/cat?type=square" alt="A random cat"></p>
<p><img src="https://cataas.com/cat?width=200&height=200" alt="Another cat"></p>

<hr>

<h2>Edge Cases</h2>

<p><strong>Bold at start of line</strong></p>
<p><em>Italic at start of line</em></p>
<p>Line ending in bold <strong>bold end</strong></p>
<p>Line ending in italic <em>italic end</em></p>

<p>Two <strong>bold</strong> words <strong>in one</strong> line.</p>
<p>Two <em>italic</em> words <em>in one</em> line.</p>
<p>Mix of <strong>bold</strong> and <em>italic</em> and <code>code</code> all in one sentence.</p>

<p>A line with a <a href="https://example.com">link</a> embedded mid-sentence, not at start.</p>

<p>Empty brackets edge: []() should produce an empty anchor.</p>

<hr>

<h2>Adjacent Formatting</h2>

<p><strong>bold</strong><em>italic</em> right next to each other.</p>
<p><code>code</code><strong>bold</strong> no space between.</p>

<hr>

<h2>Horizontal Rules</h2>

<p>Before rule.</p>

<hr>

<p>After rule. Another one coming.</p>

<hr>

<p>Done.</p>

<hr>

<h2>Deeply Nested Content</h2>

<blockquote>Blockquote with <strong>bold</strong>, <em>italic</em>, <code>code</code>, and a <a href="https://example.com">link</a> all inside.</blockquote>

<hr>

<h2>Repeated Patterns</h2>

<p><strong>one</strong> <strong>two</strong> <strong>three</strong></p>
<p><em>one</em> <em>two</em> <em>three</em></p>
<p><code>one</code> <code>two</code> <code>three</code></p>
<p><a href="https://a.com">a</a> <a href="https://b.com">b</a> <a href="https://c.com">c</a></p>

<hr>

<h2>Long Line</h2>

<p>This is a very long line with <strong>bold text appearing multiple times</strong>, and also <em>italic text appearing multiple times</em>, and some <code>inline code blocks</code> scattered throughout, plus a <a href="https://example.com">hyperlink to example</a> somewhere in the middle, and it just keeps going with more <strong>bold</strong> and more <em>italic</em> until it finally ends.</p>

<hr>

<h2>Images Mixed with Links</h2>

<p><img src="https://cataas.com/cat?1" alt="Image one"></p>
<p><a href="https://example.com">Link after image</a></p>
<p><img src="https://cataas.com/cat?2" alt="Image two"></p>
<p><a href="https://example.org">Another link</a></p>

<p></p>
```
</details>
