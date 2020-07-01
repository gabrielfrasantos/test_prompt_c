/*
 ============================================================================
 Name        : Prompt.c
 Author      :
 Version     :
 Copyright   :
 Description : Hello World in C, Ansi-style
 ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Linked list functions */
typedef struct list
{
	int value;
	int key;
	struct list *next;
}list_t;

typedef struct
{
	list_t * head;
	list_t * current;
	int counter;
}linked_list_t;

linked_list_t m_ll_object;

static void ll_init()
{
	m_ll_object.head = NULL;
	m_ll_object.current = NULL;

	m_ll_object.counter = 0;
}

static void ll_put(int data)
{
	list_t * link = (list_t*) malloc(sizeof(list_t));

	m_ll_object.counter++;

	link->key = m_ll_object.counter;
	link->value = data;

	link->next = m_ll_object.head;

	m_ll_object.head = link;
}

static int ll_get(int index)
{
   list_t * current = m_ll_object.head;

   if(m_ll_object.head == NULL)
   {
      return -1;
   }

   while(current->key != index)
   {
      if(current->next == NULL)
	  {
         return 0;
      }
	  else
	  {
         current = current->next;
      }
   }

   return current->value;
}

static void ll_list()
{
   list_t * ptr = m_ll_object.head;

   while(ptr != NULL)
   {
      printf("%d ", ptr->value);
      ptr = ptr->next;
   }


}

static int ll_remove(int key)
{
   list_t * current = m_ll_object.head;
   list_t * previous = NULL;

   if(m_ll_object.head == NULL)
   {
      return -1;
   }

   while(current->key != key)
   {
      if(current->next == NULL)
	  {
         return -1;
      }
	  else
	  {
         previous = current;
         current = current->next;
      }
   }

   if(current == m_ll_object.head)
   {
      m_ll_object.head = m_ll_object.head->next;
   }
   else
   {
      previous->next = current->next;
   }

   return current->value;
}

static void ll_clear()
{
	int i = 0;
	for(i = 0; i < m_ll_object.counter; i++)
	{
		ll_remove(i);
	}
}

static int ll_first()
{
   return m_ll_object.head->value;
}

static int ll_last()
{
   list_t * current = m_ll_object.head;

   if(m_ll_object.head == NULL)
   {
      return -1;
   }

   while(1)
   {
      if(current->next == NULL)
	  {
         return current->value;
      }
	  else
	  {
         current = current->next;
      }
   }

   return -1;
}

static int ll_size()
{
   int length = 0;
   list_t * current;

   for(current = m_ll_object.head; current != NULL; current = current->next)
   {
      length++;
   }

   return length;
}

/* End of linked list */

/* Terminal parser */
static int get_number_from_array(char * input)
{
	char * ptr = input;
	int counter = 0;

	while(((*ptr < '0') || (*ptr > '9')) && (counter < 20))
	{
		ptr++;
		counter++;
	}

	if(counter < 20)
	{
		return *ptr - '0';
	}

	return 0;
}

static void shell_put_cmd(char * input)
{
	int element = get_number_from_array(input);

	ll_put(element);

	ll_list();
}

static void shell_get_cmd(char * input)
{
	int element = get_number_from_array(input);

	printf("%d", ll_get(element));
}

static void shell_first_cmd(char * input)
{
	printf("%d", ll_first());
}

static void shell_last_cmd(char * input)
{
	printf("%d", ll_last());
}

static void shell_remove_cmd(char * input)
{
	printf("%d", ll_remove(get_number_from_array(input)));
}

static void shell_clear_cmd(char * input)
{
	ll_clear();
}

static void shell_list_cmd(char * input)
{
	ll_list();
}

static void shell_sort_cmd(char * input)
{
	int i, j, tmp_key, tmp_value;
	list_t * current;
	list_t * next;
	int k = ll_size();

	for (i = 0; i < ll_size() - 1; i++, k--)
	{
		current = m_ll_object.head;
		next = m_ll_object.head->next;

		for (j = 1 ; j < k ; j++)
		{
			if (current->value > next->value)
			{
				tmp_value = current->value;
				current->value = next->value;
				next->value = tmp_value;

				tmp_key = current->key;
				current->key = next->key;
				next->key = tmp_key;
			}

			current = current->next;
			next = next->next;
		}
	}

	ll_list();
}

typedef void (*callback_t)(char * input);

typedef struct
{
	const char * command;
	callback_t callback;
}shell_t;

const static shell_t m_shell_table[]=
{
	{"put", &shell_put_cmd},
	{"get", &shell_get_cmd},
	{"first", &shell_first_cmd},
	{"last", &shell_last_cmd},
	{"list", &shell_list_cmd},
	{"clear", &shell_clear_cmd},
	{"remove", &shell_remove_cmd},
	{"sort", &shell_sort_cmd},
};
#define MAX_TABLE ((sizeof(m_shell_table))/(sizeof(shell_t)))

static int search_and_execute_command(char * input)
{
	int i = 0;
	for(i = 0; i < MAX_TABLE; i++)
	{
		if(strncmp(input, m_shell_table[i].command, strlen(m_shell_table[i].command)) == 0)
		{
			m_shell_table[i].callback(input);
			return 0;
		}
	}

	return -1; /* command not found */
}

/* End of terminal parser */

int main(int argc, char *argv[])
{
	char input[201];

	ll_init();

	while(1)
	{
		printf("\nprompt> ");
		if (fgets(input, 200, stdin) == NULL)
		{
			printf("An error ocurred.\n");
			break;
		}

		if (strncmp(input, "exit\n", 5) == 0)
		{
			printf("Leaving. Good bye.\n");
			break;
		}

		if(search_and_execute_command(input) < 0)
		{
			printf("Command not found!\n");
		}
	}

	return EXIT_SUCCESS;
}
