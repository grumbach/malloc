# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: agrumbac <agrumbac@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2017/05/25 11:36:26 by agrumbac          #+#    #+#              #
#    Updated: 2018/02/06 22:07:36 by agrumbac         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

############################## HOST ############################################

ifeq ($(HOSTTYPE),)
	HOSTTYPE := $(shell uname -m)_$(shell uname -s)
endif

############################## VAR #############################################

NAME = libft_malloc.so

HOSTLIB = libft_malloc_$(HOSTTYPE).so

SRC = malloc.c free.c realloc.c show_alloc.c

SRCDIR = srcs/

OBJDIR = objs

CFLAGS = -Wall -Wextra -Werror

LDFLAGS = -shared

DEP = libft/includes/libft.h includes/malloc.h

CC = clang

OBJ = $(addprefix $(OBJDIR)/, $(SRC:.c=.o))

LIB = -Llibft -lft

INCL = -Ilibft/includes -Iincludes

############################## COLORS ##########################################

BY = "\033[33;1m"
BR = "\033[31;1m"
BG = "\033[32;1m"
BB = "\033[34;1m"
BM = "\033[35;1m"
BC = "\033[36;1m"
BW = "\033[37;1m"
Y = "\033[33m"
R = "\033[31m"
G = "\033[32m"
B = "\033[34m"
C = "\033[36m"
KW = "\033[37;5m"
KR = "\033[31;5m"
KG = "\033[32;5m"
KB = "\033[34;5m"
X = "\033[0m"
UP = "\033[A"
CUT = "\033[K"

############################## RULES ###########################################

all: art ${NAME}
	@echo ${G}Success"   "[${NAME}]${X}

libft/libft.a:
	@make -C libft

${NAME}: ${HOSTLIB}
	@echo ${C}Linking"   "[${HOSTLIB}]...${X}
	@ln -fs ${HOSTLIB} ${NAME}

${HOSTLIB}: ${OBJ} ${DEP} libft/libft.a
	@echo ${B}Compiling [${HOSTLIB}]...${X}
	@${CC} ${LDFLAGS} ${LIB} ${INCL} -o $@ ${OBJ}

${OBJDIR}/%.o: ${SRCDIR}%.c ${DEP} libft/libft.a
	@echo ${Y}Compiling [$@]...${X}
	@/bin/mkdir -p ${OBJDIR}
	@${CC} ${CFLAGS} ${INCL} -c -o $@ $<
	@printf ${UP}${CUT}

clean:
	@echo ${R}Cleaning"  "[libft objs]...${X}
	@make -C libft/ clean
	@echo ${R}Cleaning"  "[objs]...${X}
	@/bin/rm -Rf ${OBJDIR}

fclean: clean
	@make -C libft/ fclean
	@echo ${R}Cleaning"  "[${NAME}]...${X}
	@/bin/rm -f ${NAME}
	@/bin/rm -f ${HOSTLIB}

test: art libft/libft.a ${NAME}
	@${CC} ${INCL} ${LIB} tests/test.c -L. -lft_malloc -o test
	@./run.sh ./test

re: fclean all

############################## ART #############################################

art:
	@echo ${BW}"                            ,-."
	@echo ${BW}"       ___"${BG}",---.__        "${BW}"  /'|\`\\  "${BG}"        __,---,"${BW}"___"
	@echo ${BW}"    ,-'    "${BG}"\\\`    \`-.____,-"${BW}"'  |  \`"${BG}"-.____,-'    //"${BW}"    \`-."
	@echo ${BW}"  ,'        "${BG}"|           ~'"${BW}"\\     /"${BG}"\`~           |   "${BW}"     \`."
	@echo ${BW}" /      ___"${BG}"//              "${BW}"\`. ,' "${BG}"         ,  , \\ "${BW}"__      \\"
	@echo ${BW}"|    ,-'  "${BG}" \`-.__   _         |        ,    __,-' "${BW}"  \`-.    |"
	@echo ${BW}"|   /       "${BG}"   /\\_  \`   .    |    ,      _/\\   "${BW}"       \\   |"
	@echo ${BW}"\\  |        "${BG}"   \\ \\\`-.___ \\   |   / ___,-'/ / "${BW}"          |  /"
	@echo ${BW}" \\  \\     "${BG}"      | \`._ "${KR}"()"${X}${BG}"\`\\\\\\  |  //'"${KR}"()"${X}${BG}" _,' |  "${BW}"         /  /"
	@echo ${BW}"  \`-.\\    "${BG}"     /'  _ \`---'' , . \`\`---' _  \`\\ "${BW}"        /,-'"
	@echo ${BW}"     \`\`   "${BG}"    /     \\    ,='/ \\\`=.    /     \\ "${BW}"      ''"
	@echo "            "${BG}" |__   /|\\_,--.,-.--,--._/|\\   __|"
	@echo ${BR}"            "${BW}" /"${BG}"  \`./ \\ "${BW}"\\\`\\ |  |  | /,/"${BG}"/' \\,' "${BW}" \\"
	@echo ${BR}"    ,.  "${KR}" (  "${X}${BR}""${BW}"/   / "${BG}"   | "${BW}"|--+--|--+-/-"${BG}" |   "${BW}" \\   \\ "${KR}")   "${X}${BR}"     .    "
	@echo ${BR}"  "${KR}" ("${X}${BR}"\"     )"${BW}"|   |  "${BG}"   /'"${BW}"\\_\\_\\ | /_/_/"${BG}"\`\\ "${BW}"    |   | "${BR}","${KR}"'        )"${X}${BR}" ."
	@echo ${BR}" .; )  ' ("${KR}"("${X}${BW}" \\   \\__,"${BG}" \\_     \`~'     _/ "${BW}".__/   / "${BR}";"${KR}"(,     ("${X}${BR}"(  ( "
	@echo ${BR}" _\"."${KR}", ,"${X}${BR}"._'_."${BW}" \`-._,-' "${BG}"  \`-._______,-'  "${BW}" \`-._,-'"${BR}"( . )_  _' )_"${KR}"'"${X}${BR}")"
	@echo ${BR}"    malloc is EVIL, by using it, you agree to these TERMS"
	@echo ${X}

.PHONY: all clean fclean re test art test
