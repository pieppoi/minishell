#include "minishell.h"
#include <locale.h>

static struct termios	g_original_termios;
static bool				g_termios_saved;

static void	enable_utf8_mode(void)
{
	struct termios	term;

	if (!isatty(STDIN_FILENO))
		return ;
	if (tcgetattr(STDIN_FILENO, &term) == -1)
		return ;
	g_original_termios = term;
	g_termios_saved = true;
#ifdef IUTF8
	term.c_iflag |= IUTF8;
#endif
#ifdef ECHOCTL
	term.c_lflag &= ~ECHOCTL;
#endif
	tcsetattr(STDIN_FILENO, TCSANOW, &term);
}

void	configure_input_behavior(void)
{
	setlocale(LC_CTYPE, "");
	rl_catch_signals = 0;
	rl_catch_sigwinch = 0;
	rl_variable_bind("convert-meta", "off");
	rl_variable_bind("input-meta", "on");
	rl_variable_bind("output-meta", "on");
	enable_utf8_mode();
}

void	restore_terminal_behavior(void)
{
	if (g_termios_saved)
		tcsetattr(STDIN_FILENO, TCSANOW, &g_original_termios);
}

void	release_shell_resources(t_env **env)
{
	if (env && *env)
	{
		free_env_list(*env);
		*env = NULL;
	}
	rl_clear_history();
	restore_terminal_behavior();
}

void	shutdown_shell(t_env **env, int exit_code)
{
	release_shell_resources(env);
	exit(exit_code);
}
