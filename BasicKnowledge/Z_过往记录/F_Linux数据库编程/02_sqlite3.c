#include <stdlib.h>
#include <stdio.h>
#include <strings.h>
#include "sqlite3.h"

int callback(void *d, int cols, char **col_values, char **col_names)
{
}

int main()
{
	sqlite3* db = NULL; 
	char* my_errmsg = NULL;
	char* my_sql = (char*)calloc(100, sizeof(char));
	char* tablename = (char*)calloc(100, sizeof(char));
	char**table = NULL;
	int r = 0, c = 0, i = 0;
	
	//数据库
	int ret = sqlite3_open("/tmp/my_sqlite.db", &db);
	if(ret != SQLITE_OK) {
		//printf("数据库连接失败，可能是因为存在\n");
		//printf("error: %s\n",sqlite3_errmsg(db));
	}
	
	//建表
	printf("please input your tablename:");
	scanf("%s", tablename);	
	sprintf(my_sql, "create table \'%s\'(id int, name text, date text);", tablename);
	ret = sqlite3_exec(db, my_sql, NULL, NULL, &my_errmsg);
	if (ret != SQLITE_OK) {
		//perror("数据库不可写，建表失败\n");
		//printf("%s\n", my_errmsg);
	}
	
	//开始学生系统管理
	int flag = 0, choose = 0;	
	int old_id = -1, id = 0;
	char* name = (char*)calloc(20, sizeof(char));	
	char* date = (char*)calloc(30, sizeof(char));
	while (flag == 0) {
		//菜单栏
		printf("\tstudent manage system\n");
		printf("1.show all students\n");
		printf("2.search one student\n");
		printf("3.change one student information\n");		
		printf("4.add one student information\n");
		printf("5.remove one student information\n");	
		printf("6.quit now\n");		
		printf("pls type[1--6]:");		
		
		id = 0;
		scanf("%d", &choose);
		switch (choose) {
			case 1:
				sprintf(my_sql, "select * from \'%s\';", tablename);	
				ret = sqlite3_get_table(db, my_sql, &table, &r, &c, &my_errmsg);
				if (ret != SQLITE_OK) {
					perror("can't read student information\n");
					printf("%s\n", my_errmsg);
					break;
				}
				for (i = 0; i < (r + 1) * c; i++) {
					if (i % 3 == 0)
						printf("\n");
					printf("%12s", table[i]);
				}
				printf("\n");
				sqlite3_free_table(table);							
				break;
			case 2:
				printf("\nplease input your ID:");
				scanf("%d", &id);			
				sprintf(my_sql, "select * from \'%s\' where id = %d;", tablename, id);	
				ret = sqlite3_get_table(db, my_sql, &table, &r, &c, &my_errmsg);
				if (ret != SQLITE_OK) {
					perror("can't read student information\n");
					printf("%s\n", my_errmsg);
					break;
				}
				for (i = 0; i < (r + 1) * c; i++) {
					if(i % 3 == 0)
						printf("\n");
					printf("%12s", table[i]);
				}
				printf("\n");
				sqlite3_free_table(table);				
				break;
			case 3:
				printf("please input your OLD ID:");
				scanf("%d", &old_id);
				printf("\nplease input your new ID:");
				scanf("%d", &id);
				printf("\nplease input your new NAME:");
				scanf("%s", name);
				printf("\nplease input your new DATE:");
				scanf("%s", date);			
				sprintf(my_sql, \
					"update \'%s\' set id = %d, name = \'%s\', date = \'%s\') where id = %d;" , \
					tablename, id, name, date, old_id);			
				ret = sqlite3_exec(db, my_sql, NULL, NULL, &my_errmsg);
				if (ret != SQLITE_OK) {
					perror("can't change student information\n");
					printf("%s\n",my_errmsg);
				}		
				break;
			case 4:
				printf("please input your ID:");
				scanf("%d", &id);
				printf("\nplease input your NAME:");
				scanf("%s", name);
				printf("\nplease input your DATE:");
				scanf("%s", date);
				sprintf(my_sql, "insert into \'%s\' values(%d,\'%s\',\'%s\');" , \
					tablename, id, name, date);
				ret = sqlite3_exec(db, my_sql, NULL, NULL, &my_errmsg);
				if (ret != SQLITE_OK) {
					perror("can't add student information\n");
					printf("%s\n", my_errmsg);
				}			
				break;
			case 5:
				printf("please input your ID:");
				scanf("%d", &id);
				sprintf(my_sql, "delete from \'%s\' where id = %d;" , \
					tablename, id);	
				ret = sqlite3_exec(db, my_sql, NULL, NULL, &my_errmsg);
				if (ret != SQLITE_OK) {
					perror("can't remove student information\n");
					printf("%s\n",my_errmsg);
				}					
				break;
			case 6:
				printf("thank you for your choice\n");
				flag = 1;//设置退出标志
				break;		
		}
		//为下一次接收做准备，除去\n的影响
		printf("press any key to continue!\n");
		getchar();
		getchar();
		system("clear");			
	}
	
	free(name);
	free(date);
	free(my_sql);
	free(tablename);	
	
	//是否关闭数据库
	ret = sqlite3_close(db);
	if (ret != SQLITE_OK)
		perror("close database failed\n");

	return 0;
}
