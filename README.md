# Command Parser

- simple command parser

## Example

- handle variables

```bash
> hello $world "$every one"
[CMD]=>[hello]->[$world]->[$every]->[ one]->|
[NULL]
> 
```

```bash
> "hello \"world\"";new command && stop here || stop here | zero
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

```bash
> echo $$ && echo $? && echo $PATH && exit # && ls -la
[CMD]=>[echo]->[$$]->|
[AND]
[CMD]=>[echo]->[$?]->|
[AND]
[CMD]=>[echo]->|
[VAR](PATH)
[AND]
[CMD]=>[exit]->|
[NULL]
> 
```

- inactive mode

```bash
$ echo "hello world" | a.out
[CMD]=>[hello]->[world]->|
[NULL]
$
```

## Build and Run

`gcc -Wall -Werror -Wextra -pedantic -std=gnu89 *.c && a.out`
