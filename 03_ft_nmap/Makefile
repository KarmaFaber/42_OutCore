#◉────▣───▢◇▢───▣────◉•◉───▣───▢   Colors:   ▢───▣───◉•◉───▣────▢◇▢───▣───◉#

BLACK   := \033[0;30m
RED     := \033[0;31m
GREEN   := \033[0;32m
YELLOW  := \033[0;33m
BLUE    := \033[0;34m
MAGENTA := \033[0;35m
CYAN    := \033[0;36m

CURRENT_COLOR := \033[0;34m
RESET   := \033[0m

#◉───▣───▢◇▢───▣───◉•◉───▣───▢   Variables:   ▢───▣───◉•◉───▣───▢◇▢───▣───◉#
PCAP_CFLAGS := $(shell pcap-config --cflags 2>/dev/null || echo "")
PCAP_LIBS   := $(shell pcap-config --libs 2>/dev/null || echo "-lpcap")

NAME        := ft_nmap
CC          := gcc
CFLAGS      := -Wall -Wextra -Werror $(PCAP_CFLAGS)
RM          := rm -rf

LIBS        := -lm $(PCAP_LIBS) -lpthread

SRC_DIR     := src
OBJ_DIR     := obj
INC_DIR     := inc

INCLUDES    := -I$(INC_DIR)

VALGRIND    := valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes --track-fds=all --error-limit=no --log-file=test/valgrind_outputs/valgrind_output_$(shell date +%Y%m%d_%H%M%S).log -s

#◉───▣───▢◇▢───▣───◉•◉───▣───▢  ft_nmap srcs ▢───▣───◉•◉───▣───▢◇▢───▣───◉#

SRCS                := \
                    parser/parser.c \
                    parser/parser_wrappers.c \
                    parser/parser_wrappers_bonus.c \
                    parser/parse_args.c \
                    parser/parse_ports.c \
                    parser/parse_args_aux.c \
                    parser/parse_file.c \
                    \
                    printers/print_usage.c \
                    printers/print_init.c \
					printers/print_result.c \
					printers/print_debug.c \
                    \
					network_engine/network.c \
					network_engine/pcap_handler.c \
					network_engine/scan.c \
					network_engine/thread_pool.c \
					\
                    ft_nmap.c \
                    utils.c \


SRC_PATHS           := $(addprefix $(SRC_DIR)/,$(SRCS))
OBJS                := $(addprefix $(OBJ_DIR)/, $(SRCS:.c=.o))

#◉───▣───▢◇▢───▣───◉•◉───▣───▢     rules      ▢───▣───◉•◉───▣───▢◇▢───▣───◉#

all: $(NAME)

$(NAME): $(OBJS) 
	$(CC) $(CFLAGS) $(INCLUDES) -o $(NAME) $(OBJS) $(LIBS)
	@echo "$(CURRENT_COLOR) $(NAME) created.\n$(RESET)"

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

clean:
	$(RM) $(OBJ_DIR)
	@echo "$(CURRENT_COLOR)All object files have been cleaned.\n$(RESET)"

fclean: clean
	$(RM) $(NAME)
	@echo "$(CURRENT_COLOR) $(NAME): have been cleaned.\n$(RESET)"

re: fclean all

#◉───▣───▢◇▢───▣───◉•◉───▣───▢    Valgrind    ▢───▣───◉•◉───▣───▢◇▢───▣───◉#

valgrind: all
	@echo "$(CURRENT_COLOR)➵⤐──╌╌➣⋆➣╌╌──⤏➵•➵⤐──╌╌➣⋆➣╌╌──Running Valgrind.──╌╌➣⋆➣╌╌──⤏➵•➵⤐──╌╌➣⋆➣╌╌──$(RESET)"
	@mkdir -p test/valgrind_outputs
	-sudo $(VALGRIND) ./$(NAME) --ip 8.8.8.8 --ports 2147483648
	@-test/sh_files/./open_valgrind_log.sh
	@echo "$(CURRENT_COLOR)➵⤐╌╌➣⋆➣╌─⤏➵•➵⤐─╌╌➣⋆➣── Valgrind completed. Check valgrind_output.log for details. ─╌➣⋆➣╌─⤏➵•➵⤐─╌╌➣⋆➣╌╌─$(RESET)"

clean_valgrind:
	test/sh_files/./clean_valgrind.sh

#◉───▣───▢◇▢───▣───◉•◉───▣───▢   Test    ▢───▣───◉•◉───▣───▢◇▢───▣───◉#

test_parse: all
	@echo "$(CURRENT_COLOR)➵⤐──╌╌➣⋆➣╌╌──⤏➵•➵⤐──╌╌➣⋆➣╌╌── Parse Test──╌╌➣⋆➣╌╌──⤏➵•➵⤐──╌╌➣⋆➣╌╌──$(RESET)"
	@test/sh_files/./test_parsing.sh
	@echo "$(CURRENT_COLOR)➵⤐╌╌➣⋆➣╌─⤏➵•➵⤐─╌╌➣⋆➣── End ─╌➣⋆➣╌─⤏➵•➵⤐─╌╌➣⋆➣╌╌─$(RESET)"



#◉───▣───▢◇▢───▣───◉•◉───▣───▢    Norminette    ▢───▣───◉•◉───▣───▢◇▢───▣───◉#

norm:
	@echo "$(CURRENT_COLOR)➵⤐──╌╌➣⋆➣╌╌──⤏➵•➵⤐──╌╌➣⋆➣╌╌── Checking Norminette: ──╌╌➣⋆➣╌╌──⤏➵•➵⤐──╌╌➣⋆➣╌╌➔$(RESET)"
	@norminette $(SRC_PATHS) $(INC_DIR)/*.h > file_norminette.log || true
	@code file_norminette.log
	@echo " Check file_norminette.log for more info.$(RESET)"
	@echo "$(CURRENT_COLOR)➵⤐──╌╌➣⋆➣╌╌──⤏➵•➵⤐──╌╌➣⋆➣╌╌── Norminette completed. Check your output for details. ─╌➣⋆➣╌─⤏➵•➵⤐─╌╌➣⋆➣╌╌─"

#◉───▣───▢◇▢───▣───◉•◉───▣───▢  Format ▢───▣───◉•◉───▣───▢◇▢───▣───◉#

format: 
	c_formatter_42 $(SRCS) datatypes.h ft_nmap.h network_engine.h parser.h printer.h

#◉───▣───▢◇▢───▣───◉•◉───▣───▢ Phony targets ▢───▣───◉•◉───▣───▢◇▢───▣───◉#

.PHONY: all clean fclean re valgrind clean_valgrind test norm format test_parse