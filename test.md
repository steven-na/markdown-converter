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
