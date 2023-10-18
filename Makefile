# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: agranger <agranger@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/07/28 14:32:24 by agranger          #+#    #+#              #
#    Updated: 2023/07/28 14:36:34 by agranger         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME			=	aureleqcm

COMPILER		=	c++

CXXFLAGS		=	-MMD -MP

SRCDIR			=	./srcs

OBJDIR			=	./objs

HDIR			=	./headers

CPPFILES		=	main.cpp \
					aureleQCM.cpp

HFILES			=	main.hpp \
					aureleQCM.hpp
				
HEADERS			=	$(addprefix $(HDIR)/,$(HFILES))

SRCS			=	$(addprefix $(SRCDIR)/,$(CPPFILES))

OBJS			=	$(addprefix $(OBJDIR)/,$(CPPFILES:.cpp=.o))


DEPS			=   $(OBJS:%.o=%.d)

DEBUG			=	0

ifeq ($(DEBUG), 1)
	CXXFLAGS += -g3
	COMPILER = g++
endif



all							:	$(NAME)

$(NAME)						:	$(OBJDIR) $(OBJS)
								@$(COMPILER) $(CXXFLAGS) -I$(HDIR) $(OBJS) -lcurl -o $(NAME)
								@echo "$(NAME) created"
	
$(OBJDIR)					:
								@mkdir -p $@ \
									$@/config \
									$@/eventLoop \
									$@/cgi

$(OBJDIR)/%.o				:   $(SRCDIR)/%.cpp $(OBJDIR)/.DEBUG.$(DEBUG)
								@$(COMPILER) -o $@ -c $< $(CXXFLAGS) -I$(HDIR)

$(OBJDIR)/.DEBUG.$(DEBUG)	:
								@rm -rf $(OBJDIR)/.DEBUG.*
								@touch $(OBJDIR)/.DEBUG.$(DEBUG)

clean						: 
								@rm -rf $(OBJDIR)
								@echo "object files removed"

fclean						:   clean
								@rm -rf $(NAME) 
								@echo "$(NAME) removed"

re							:   fclean all 

.PHONY						:   clean fclean re all

-include $(DEPS)

