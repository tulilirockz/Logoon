package main

import (
	"flag"
	"fmt"
	"os"
	"time"
)

const (
	DEFAULT_IDENTIFIER = "GEN"
	ENV_VAR_PREFIX     = "LOGOON_"
)

func default_or_env_var(variable *string, env_var string) {
	if *variable == "" {
		*variable = os.Getenv(env_var)
	}
}

func append_message_fragment(s *string, other string) {
	if *s == "" {
		*s += "\n[" + other + "]"
		return
	}

	*s += " [" + other + "]"
}

func main() {
	f_message := flag.String("m", "", "Message that will be read")
	f_context := flag.String("c", "", "Context for the message")
	f_app_identifier := flag.String("i", DEFAULT_IDENTIFIER, "Identifier for the message")
	f_output_file := flag.String("o", "", "Filepath for log")
	f_quiet_logging := flag.Bool("q", false, "Quieter logging")

	flag.Parse()

	default_or_env_var(f_message, ENV_VAR_PREFIX+"MESSAGE")
	default_or_env_var(f_context, ENV_VAR_PREFIX+"CONTEXT")
	default_or_env_var(f_app_identifier, ENV_VAR_PREFIX+"IDENTIFIER")
	default_or_env_var(f_output_file, ENV_VAR_PREFIX+"OUTPUT_FILE")
	if os.Getenv(ENV_VAR_PREFIX+"QUIET") == "1" {
		*f_quiet_logging = true
	}

	var final_message string = ""

	if *f_quiet_logging == false {
		hostname, err := os.Hostname()
		if err != nil {
			fmt.Println("Failed reading hostname")
			os.Exit(1)
		}

		append_message_fragment(&final_message, hostname)
		append_message_fragment(&final_message, time.Now().Format(time.RubyDate))
	}

	append_message_fragment(&final_message, *f_app_identifier)
	append_message_fragment(&final_message, *f_context)
	final_message += ": " + *f_message

	var opened_file *os.File = os.Stdout

	if *f_output_file != "" && *f_output_file != "-" {
		final_file, err := os.OpenFile(*f_output_file, os.O_CREATE|os.O_APPEND|os.O_WRONLY, 0600)

		if err != nil {
			fmt.Println("Failed writing to logfile")
			os.Exit(1)
		}

		defer final_file.Close()
		opened_file = final_file
	}

	defer opened_file.Close()
	fmt.Fprintf(opened_file, "%v", final_message)
}
