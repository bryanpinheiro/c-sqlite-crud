#include "../include/todo.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main()
{
  sqlite3 *db;
  int rc = sqlite3_open("todos.db", &db);

  if (rc != SQLITE_OK)
  {
    fprintf(stderr, "Cannot open database: %s\n", sqlite3_errmsg(db));
    sqlite3_close(db);
    return 1;
  }

  // Check if the 'todos' table exists
  int table_exists = 0;
  sqlite3_stmt *stmt;
  const char *query = "SELECT name FROM sqlite_master WHERE type='table' AND name='todos';";
  if (sqlite3_prepare_v2(db, query, -1, &stmt, NULL) == SQLITE_OK)
  {
    if (sqlite3_step(stmt) == SQLITE_ROW)
    {
      table_exists = 1;
    }
    sqlite3_finalize(stmt);
  }

  if (!table_exists)
  {
    // Table does not exist, create it
    if (executeSQLFromFile(db, "sql/create_todos.sql") != 0)
    {
      sqlite3_close(db);
      return 1;
    }
    printf("Todos table created successfully!\n");
  }

  // if (insertTodoItem(db, "Learn C++ programming", 0) != 0)
  // {
  //   sqlite3_close(db);
  //   return 1;
  // }

  // updateTodoCompleted(db, 1, 1);

  // deleteTodoByID(db, 1);

  TodoListWithCount todoListWithCount = getAllTodoItemsWithCount(db, "sql/get_all_todos.sql");
  TodoItem *todoList = todoListWithCount.todoList;
  int numItems = todoListWithCount.numTodos;

  if (!todoList)
  {
    printf("Failed to retrieve todo items.\n");
    sqlite3_close(db);
    return 1;
  }

  printf("Number of Todo items: %d\n", numItems);

  displayTodos(todoList, numItems);

  sqlite3_close(db);
  return 0;
}
