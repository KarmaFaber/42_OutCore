/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   datatypes.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: KarmaFaber <github.com/KarmaFaber>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/27 10:55:02 by KarmaFaber        #+#    #+#             */
/*   Updated: 2026/10/29 18:02:37 by KarmaFaber       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef DATATYPES_H
# define DATATYPES_H

#define SCAN_SYN  (1 << 0)
#define SCAN_NULL (1 << 1)
#define SCAN_FIN  (1 << 2)
#define SCAN_XMAS (1 << 3)
#define SCAN_ACK  (1 << 4)
#define SCAN_UDP  (1 << 5)

#define MAX_PORTS	1024

//#➵⤐──╌╌➣⋆➣── Colors ────────────────➣⋆➣╌╌──⤏➵•➵⤐──╌╌➣⋆➣╌╌➔#
#define ANSI_RED		"\001\033[1;31m"
#define ANSI_GREEN		"\001\033[1;32m\002"

#define ANSI_BLUE		"\001\033[1;34m\002"
#define ANSI_MAGENTA    "\001\033[1;35m\002"

#define ANSI_YELLOW		"\001\033[1;33m\002"
#define ANSI_ORANGE		"\001\033[1;33m\002"
#define ANSI_ORANGE_256	"\001\033[38;5;214m\002"

#define ANSI_CGRAY_DARK	"\001\x1B[90m\002"
#define ANSI_CGRAY_LIGHT  "\001\x1B[37m\002"

#define ANSI_RESET		"\001\033[0m\002"

//#➵⤐──╌╌➣⋆➣── Structs ───────────────➣⋆➣╌╌──⤏➵•➵⤐──╌╌➣⋆➣╌╌➔#
typedef struct s_map t_map;
typedef int (*t_arg_handler)(char *arg, t_map *map);

typedef struct s_arg_map
{
	const char			*key;
	int					needs_value;
	t_arg_handler		handler;
} t_arg_map;

typedef struct s_port_report
{
	//t_port_state		states[6]; // Estado para cada uno de los 6 tipos de scan
	
}	t_port_report;


typedef struct s_nmap_cfg
{
	char			*target_name;   // El string original (ej: "google.com" o "8.8.8.8")
	char			*ip;
	char			*file;
	int				ports[1024];	// Lista de puertos a escanear
	int				num_ports;
	int				threads;		// Default 0, max 250
	int				scan_types;		// Bitmask para manejar varios scans a la vez

	//
	double			scan_time;		// Tiempo total del escaneo (en segundos)
	/* Variables de Bonus */
	int				os_detect;		// Booleano para activar la detección de sistema operativo
	char			*spoof_ip;		// String con la IP falsa
	int				bad_sum;		// Booleano para forzar checksum corrupto
	int				packet_trace;   // Booleano para activar el volcado de trazas
	int				ttl_value;      // Valor entero de TTL personalizado (0-255)
} t_nmap_cfg;


/* ** function prototypes ** */
//main struct
typedef struct	s_map
{
	t_nmap_cfg			cfg;
	//t_pseudo_header		pseudo_header;			// Necesario para calcular el checksum TCP/UDP
	pcap_t				*pcap_handle;			// Necesario para libpcap

	struct addrinfo		*addrinfo;
	int					socket_fd;

	/* Matriz de estados: indexada directamente por número de puerto (0-65535) */
	t_port_report	results[65536];
	int				detected_os_ttl;			// Almacenará el TTL cazado para el bonus
}	t_map;



/*
+-------------------------------+
|     Pseudocabecera IP         |  (12 bytes)
+-------------------------------+
|     Cabecera TCP              |  (mínimo 20 bytes, con campo Checksum = 0)
+-------------------------------+
|     Datos / Payload           |  (si los hay; en escaneos SYN suele ser 0)
+-------------------------------+

Pseudo-cabecera necesaria para calcular el Checksum de TCP y UDP */

typedef struct s_pseudo_header
{
	uint32_t	src_addr;    // IP origen
	uint32_t	dst_addr;    // IP destino
	uint8_t		placeholder; // Siempre a 0 (cero)
	uint8_t		protocol;    // IPPROTO_TCP o IPPROTO_UDP
	uint16_t	tcp_length;  // Tamaño de la cabecera TCP + Datos
} t_pseudo_header;

/* 

todo: solo para parte dev. luego quitar. 

struct addrinfo
{
		int              ai_flags;
		int              ai_family;
		int              ai_socktype;
		int              ai_protocol;
		socklen_t        ai_addrlen;
		struct sockaddr *ai_addr;
		char            *ai_canonname;
		struct addrinfo *ai_next;
	};
*/

//#➵⤐──╌╌➣⋆➣── Port States ─────────────➣⋆➣╌╌──⤏➵•➵⤐──╌╌➣⋆➣╌╌➔#
typedef enum e_port_state
{
	STATE_UNKNOWN = 0,
	STATE_OPEN,
	STATE_CLOSED,
	STATE_FILTERED,
	STATE_UNFILTERED,
	STATE_OPEN_FILTERED,
	MAX_PORT_STATES /* Nos da automáticamente el tamaño total del array (6) */
} t_port_state;



#endif