#include <stdio.h>
#include <mysql.h>

//finish
MYSQL * conn = NULL;
char  sql[256]={'\0'};
int id;
char name[20];
float score;

void menu(void)
{
	printf("\t学生管理系统\n");
	printf("1.显示所有学生\n");
	printf("2.查找学生\n");
	printf("3.修改学生\n");
	
	printf("5.退出\n");
	printf("6.新增一个学生\n");
	printf("7.删除一个学生\n");
	printf("pls type[1--5]:");
}

void show(coid)
{
	
}

void find(char* table_name)
{
	//查询数据
	printf("input id:");
	scanf("%d", &id);
	printf("%-5s%10s%10s\n", "id", "name", "score");	
	
	char query[1024] = {0};
	sprintf(query, "select id, name, score from %s where id = %d;", table_name, id);
	printf("%s", query);
	mysql_query(conn, (const char *)query);

	MYSQL_RES *res = mysql_store_result(conn);
	if(res == NULL) {
		printf("%s\n", mysql_error(conn));
		return;
	}
	
	int cols = mysql_num_fields(res);
	int rows = (int)mysql_num_rows(res);
	printf("cols=%d, rows=%d\n", cols, rows);
	
	MYSQL_ROW row = NULL;
	while((row = mysql_fetch_row(res)) != NULL)
		printf("%-5s%10s%10s\n", row[0], row[1], row[2]);
	
	mysql_free_result(res);
}

void update(char* table_name)
{
	printf("请输入id:\n");
	scanf("%d", &id);
	printf("请输入name:\n");
	scanf("%s", name);
	printf("请输入score:\n");
	scanf("%f", &score);
	
	char query[1024] = {0};
	sprintf(query, "update %s set name = \'%s\' , score = %f where id = %d;", \
		table_name, name, score, id);
	mysql_query(conn, (const char *)query);	
}

void insert(char* table_name)
{
	printf("请输入id:\n");
	scanf("%d",&id);
	printf("请输入name:\n");
	scanf("%s",name);
	printf("请输入score:\n");
	scanf("%f",&score);
	
	char query[1024] = {0};
	sprintf(query, "insert into %s values(%d, \'%s\', %f);", \
		table_name, id, name, score);
	mysql_query(conn, (const char *)query);	
}

void del(char* table_name)
{
	printf("input id:");
	scanf("%d",&id);
	
	char query[1024] = {0};
	sprintf(query, "delete from %s where id=%d;", table_name, id);
	mysql_query(conn, (const char *)query);
}

void init(char* database_name)
{
	conn = mysql_init(NULL);
	if (mysql_real_connect(conn, "localhost", "debian-sys-maint", \
		"CrFy1RwMaMHbZ677", database_name, 3306, NULL, 0) == NULL) {
		fprintf(stderr, "%s\n", "连接错误!");
		fprintf(stderr, "%s\n", mysql_error(conn));
	}
	fprintf(stdout, "%s\n", "连接成功!");
}

void destory(void)
{
	mysql_close(conn);
	fprintf(stdout, "%s\n", "连接关闭!");
}

int main(int argc, char *argv[])
{
	//初始化数据库，无需修改
	init(argv[1]);

	int select;
	while (1) {
		//菜单显示，无需修改
		menu();
		scanf("%d",&select);
		switch (select) {
			case 1:show();break;
			case 2:find(argv[2]);break;
			case 3:update(argv[2]);break;
						//数据库关闭，无需修改			
			case 5: destory();return 0;			
			case 6: insert(argv[2]); break;
			case 7: del(argv[2]); break;
		}		
		//为下一次接收做准备，除去\n的影响
		printf("press any key to continue!\n");
		getchar();
		getchar();
		system("clear");
	}
}



