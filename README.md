# Logoon

A simple, quick, and minimalistic logging application designed for userspace programs. It utilizes Unix-like patterns for logging and aims to have no external dependencies.

Here are some examples:

```sh
# - means stdin and stdout.
echo hello | logoon -m - -f - # This will print "hello" to your console (both stdout and stderr turn into stdout)
really_long_task | logoon -m - -f task.log
```

## Building

You'll need glibC and a C compiler, then run `make build` to build this program.
