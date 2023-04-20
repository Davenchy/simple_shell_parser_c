# Command Parser

- simple command parser

## Example

```bash
> "hello \"world\";new command && stop here || stop here | zero
[CMD]=>[hello "world"]->|
[CMD]=>[new]->[command]->|
[AND]
[CMD]=>[stop]->[here]->|
[OR]
[CMD]=>[stop]->[here]->|
[PIPE]
[CMD]=>[zero]->|
[NULL]
```

```bash
> a && b || c | d
[CMD]=>[a]->|
[AND]
[CMD]=>[b]->|
[OR]
[CMD]=>[c]->|
[PIPE]
[CMD]=>[d]->|
[NULL]
> 
```

## Build and Run

`gcc -Wall -Werror -Wextra -pedantic -std=gnu89 *.c && a.out`
