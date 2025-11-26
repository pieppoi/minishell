#include "minishell.h"
#include <locale.h>

static struct termios	g_original_termios;
static struct termios	g_interactive_termios;
static struct termios	g_execution_termios;
static bool				g_termios_saved;

static void	apply_termios(const struct termios *term)
{
	if (!term || !g_termios_saved)
		return ;
	if (!isatty(STDIN_FILENO))
		return ;
	tcsetattr(STDIN_FILENO, TCSANOW, term);
}

static void	initialize_termios_modes(void)
{
	struct termios	current;

	if (!isatty(STDIN_FILENO))
		return ;
	if (tcgetattr(STDIN_FILENO, &current) == -1)
		return ;
	g_original_termios = current;
	g_interactive_termios = current;
	g_execution_termios = current;
	g_termios_saved = true;
#ifdef IUTF8
	g_interactive_termios.c_iflag |= IUTF8;
	g_execution_termios.c_iflag |= IUTF8;
#endif
#ifdef ECHOCTL
	g_interactive_termios.c_lflag &= ~ECHOCTL;
	g_execution_termios.c_lflag |= ECHOCTL;
#endif
	apply_termios(&g_interactive_termios);
}

void	set_interactive_terminal_mode(void)
{
	apply_termios(&g_interactive_termios);
}

void	set_execution_terminal_mode(void)
{
	apply_termios(&g_execution_termios);
}

void	configure_input_behavior(void)
{
	setlocale(LC_CTYPE, "");
	rl_catch_signals = 0;
	rl_catch_sigwinch = 0;
	rl_variable_bind("convert-meta", "off");
	rl_variable_bind("input-meta", "on");
	rl_variable_bind("output-meta", "on");
	initialize_termios_modes();
}

void	restore_terminal_behavior(void)
{
	if (g_termios_saved && isatty(STDIN_FILENO))
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
