
#include "inc/test.h"
#include "../../inc/malloc.h"

/*
// Una función simulada de split/duplicación con malloc para ver dobles punteros
char **allocate_words(void)
{
    char **tab;

    // Pedimos memoria para 3 punteros de strings (doble puntero) usando malloc
    tab = malloc(sizeof(char *) * 4);
    if (!tab)
        return (NULL);

    tab[0] = malloc(16); // TINY allocation
    tab[1] = malloc(16350); // SMALL
    tab[2] = malloc(16385); // LARGE
    tab[3] = NULL;

    if (tab[0]) strcpy(tab[0], "42");
    if (tab[1]) strcpy(tab[1], "Madrid");
    if (tab[2]) strcpy(tab[2], "OuterCore");

    return (tab);
}
    


int main(void)
{
    char **words;
    int i;

    printf("--- Test de Dobles Punteros y Malloc ---\n");
    words = allocate_words();
    if (!words)
    {
        printf("Error de asignación\n");
        return (1);
    }

    i = 0;
    while (words[i])
    {
        printf("Palabra[%d]: %s (dir: %p)\n", i, words[i], (void *)words[i]);
        i++;
    }
    
    //debug printers:
    debug_dump_state();

    // Liberamos la memoria (cuando implementemos free)
    i = 0;
    while (words[i])
    {
        free(words[i]);
        i++;
    }
    free(words);

    //debug printers:
    //debug_dump_state();

    printf("--- Test finalizado con éxito ---\n");
    return (0);
}
    */
    
/*
static void	put_str(const char *s)
{
	size_t	len;

	if (!s)
		return ;
	len = 0;
	while (s[len])
		len++;
	(void)write(1, s, len);
}


int	main(void)
{
	char	*large_ptr;
	char	buf[256];
	int		n;

	put_str("--- Test basico LARGE + debug_dump_state ---\n");
	large_ptr = malloc(20000);
	if (!large_ptr)
	{
		put_str("[DEBUG MAIN]Error de asignacion en LARGE\n");
		return (1);
	}
	memcpy(large_ptr, "42Madrid", 9);
	n = snprintf(buf, sizeof(buf), "Contenido: %s (dir: %p)\n",
			large_ptr, (void *)large_ptr);
	if (n > 0)
		(void)write(1, buf, (size_t)n);
	debug_dump_state();
	put_str("--- Fin del test basico ---\n");
	return (0);
}
    */

static void	put_str(const char *s)
{
	size_t	len;

	if (!s)
		return ;
	len = 0;
	while (s[len])
		len++;
	(void)write(1, s, len);
}

static void	put_ptr(void *ptr)
{
	const char	*hex;
	uintptr_t	val;
	char		buf[19];
	int			i;

	hex = "0123456789abcdef";
	val = (uintptr_t)ptr;
	buf[0] = '0';
	buf[1] = 'x';
	if (val == 0)
	{
		(void)write(1, "0x0", 3);
		return ;
	}
	i = 17;
	buf[18] = '\0';
	while (val > 0 && i >= 2)
	{
		buf[i] = hex[val % 16];
		val /= 16;
		i--;
	}
	(void)write(1, &buf[i + 1], 17 - i);
}

static void	log_alloc(const char *label, void *ptr, char fill)
{
	put_str(label);
	put_str(" -> ");
	put_ptr(ptr);
	put_str("\n");
	if (ptr)
		*(char *)ptr = fill;
}

int	main(void)
{
	void	*p1;
	void	*p2;
	void	*p3;
	void	*p4;
	void	*p5;
	void	*p0;

	p0 = malloc(0);
	log_alloc("malloc(0) [expect NULL]", p0, 0);

	p1 = malloc(16);
	log_alloc("malloc(16) [TINY #1]", p1, 'A');

	p2 = malloc(64);
	log_alloc("malloc(64) [TINY #2]", p2, 'B');

	p3 = malloc(2028);
	log_alloc("malloc(2028) [SMALL #1]", p3, 'C');

	p4 = malloc(4096);
	log_alloc("malloc(4096) [SMALL #2]", p4, 'D');

	p5 = malloc(100000);
	log_alloc("malloc(100000) [LARGE #1]", p5, 'E');

	debug_dump_state();
	return (0);
}