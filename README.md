# Logoon

A simple, quick, and minimalistic logging application designed for userspace programs. It utilizes Unix-like patterns for logging and aims to have no external dependencies.

Here are some examples:

```sh
# - means stdin and stdout.
echo hello | logoon -m - -f - # This will print "hello" to your console (both stdout and stderr turn into stdout)
really_long_task | logoon -m - -f task.log
# Output: Thu Oct 26 14:52:10 2023 nimbus.tower hello

# Although you can also use -q for quieter logging, without date and hostname.
echo hello | logoon -q -i my_app -m - -f -
# Output: [my_app] hello

# You can also use environment variables for logging
LOGOON_OUTPUT=- LOGOON_IDENTIFIER=my_app ./build/logoon -m hi
# Output: Thu Oct 26 14:56:27 2023 nimbus.tower [my_app] hi
```

## Building

You'll need glibC and a C compiler, then run `make build` to build this program.
