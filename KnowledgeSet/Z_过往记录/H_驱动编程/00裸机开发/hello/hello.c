
void main(void)
{
	char* str = "hello world!\n";
	unsigned int *p = 0xC00a1020;
	while (*str != '\0')
		*p = *str++;
	
}


