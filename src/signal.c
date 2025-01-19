#include "minishell.h"

// Seule variable globale autorisée
int g_received_signal = 0;

static void signal_handler(int signum)
{
    g_received_signal = signum;
    if (signum == SIGINT)
    {
        write(1, "\n", 1);
        rl_on_new_line();
        rl_replace_line("", 0);
        rl_redisplay();
    }
}

void signal_setup(void)
{
    signal(SIGINT, signal_handler);   // Ctrl+C
    signal(SIGQUIT, SIG_IGN);        // Ctrl+Backslash
}