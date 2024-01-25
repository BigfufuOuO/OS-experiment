/*
 * 本文件实现vga的相关功能，清屏和屏幕输出
 * clear_screen和append2screen必须按照如下借口实现
 * 可以增加其他函数供clear_screen和append2screen调用
 */
extern void outb (unsigned short int port_to, unsigned char value);
extern unsigned char inb(unsigned short int port_from);
//VGA字符界面规格：25行80列
//VGA显存初始地址为0xB8000

#define max_vga_line 25
#define max_vga_column 80
#define cur_port_index 0x3d4
#define cur_port_data 0x3d5
short cur_line=0;
short cur_column=0;//当前光标位置
char * vga_init_p=(char *)0xB8000;


void update_cursor(void){//通过当前行值cur_cline与列值cur_column回写光标
	short pos;
	pos = cur_line * 80 + cur_column;
	outb(cur_port_index, 0xe);//lower 8 bit
	outb(cur_port_data, pos & 0xff); //write lower 8 bit
	outb(cur_port_index, 0xf);
	outb(cur_port_data, pos >> 8);//higher 8 bit
	//填写正确的内容    
}

short get_cursor_position(void){//获得当前光标，计算出cur_line和cur_column的值
	short low, high, pos;
	outb(cur_port_index, 0xe);//lower 8 bit
	low = inb(cur_port_data); //get lower 8 bit
	outb(cur_port_index, 0xf);//higher 8 bit
	high = inb(cur_port_data);//get higher 8 bit
	pos = (high << 8) + low;
	cur_line = pos/80;
	cur_column = pos % 80;
	return pos;
}


void clear_screen(void) {
	int i, j;
	for(i = 0; i < max_vga_line; i++)
	{
		for(j = 0; j < max_vga_column*2; j++)
			*(vga_init_p + i * max_vga_column * 2 + j) = 0;
	}
}

void scroll_screan(void)
{
	int i, j;
	for(i = 0; i < max_vga_line-1; i++)
	{
		for(j = 0; j < max_vga_column*2; j++)
			*(vga_init_p + i * max_vga_column * 2 + j) = *(vga_init_p + (i+1)*max_vga_column*2 + j);
	}
	for(j = 0; j < max_vga_column*2; j++)
		*(vga_init_p + i*max_vga_column*2 + j)=0;
	cur_line = 24;
}

void append2screen(char *str,int color){ 
	char c;
	while (*str)
	{
		c = *str;
		switch (c)
		{
		case '\r':
			cur_column = 0;
			break;
		
		case '\n':
			cur_column = max_vga_column;
			//cur_line++;
			break;

		case '\t':
			cur_column += 4;
			break;

		default:
			*(vga_init_p + cur_line * max_vga_column * 2 + cur_column * 2) = c;
			*(vga_init_p + cur_line * max_vga_column * 2 + cur_column * 2 + 1) = color;
			cur_column++;
			break;
		}

		if(cur_column >= max_vga_column)
		{
			cur_column = cur_column % max_vga_column;
			cur_line++;
		}

		if(cur_line == max_vga_line)//reach the max line
			scroll_screan(); // scroll up one line
		
		//finish cursor info
		update_cursor();

		str++;
	}
}


