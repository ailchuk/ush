# USH

![usage screenshot](https://github.com/okulinich/ush/blob/master/ush_usage.png?raw=true)

This is simple command-line interpreter (based on bash) for Unix.

## How to use

### Compilation:

```
make
./ush
```

### Reinstalliation:

```
make reinstall
./ush
```

### Uninstalliation:

```
make uninstall
```

## Implemented functionality

**Builtin commands withщге flags**
- `export`
- `unset`
- `exit`
- `true`
- `false`
- `return`
- `bye`
- `set`

**Builtin commands with flags**
- `env` with `-i`, `-P`, `-u`
- `cd`  with `-s`, `-P`  and `-` argument
- `pwd` with `-L`, `-P`
- `which` with `-a`, `-s`
- `echo` with `-n`, `-e`, `-E`

If command is not found among builtin ones, ush searches for
needed binary file in folders, specified by `$PATH` variable.

Shell manages signals `CTRL+D`, `CTRL+C`, `CTRL+Z`
and command separator `;`

These characters are escaped to be used literally:
`space`, `'`, `"`, `$`, `(`, `)`, `\\`, `{`, `}`.

Ush manages tilde expansion `~`, the basic form of parameter 
expansion `${parameter}`, command substitution ``command`` and `$(command)`.

Command editing is allowed with Arrow keys.

## Developers: 
- Oleksand Kulinich [@okulinich](https://github.com/okulinich)
- Andrii Ilchuk [@ailchuk](https://github.com/ailchuk)

## License

Collision is an open-sourced software licensed under the
[MIT license](LICENSE.md).
