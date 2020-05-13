#include  "bsp_645.h"
#include <stdarg.h>
uint8_t flag = 0;

extern UART_HandleTypeDef huart1;
//200322016969
uint8_t  Addr[2][6] =
{
 {0x99,0x99,0x99,0x99,0x99,0x99},
 {0xAA,0xAA,0xAA,0xAA,0xAA,0xAA}
};



uint8_t  Cmd[CMD_LEN][4] =
{
	//0,读取当前总电量+当前费率（尖峰，峰，平，谷的累计电能）
	{0x33,0x32,0x34,0x33},
	//1,读取当前正向有功总电量+当前费率（尖峰，峰，平，谷的累计电能）
	{0x33,0x32,0x34,0x33},
	//2,读取当前反向有功总电量+当前费率（尖峰，峰，平，谷的累计电能）
	{0x33,0x32,0x35,0x33},
	//3,读取当前正向无功总电量+当前费率（尖峰，峰，平，谷的累计电能）
	{0x33,0x32,0x36,0x33},
	//4,读取当前反向无功总电量+当前费率（尖峰，峰，平，谷的累计电能）
	{0x33,0x32,0x37,0x33},	
	
	//5,读取当前第一象限无功总电量+当前费率（尖峰，峰，平，谷的累计电能）
	{0x33,0x32,0x38,0x33},
	//6,读取当前第二象限无功总电量+当前费率（尖峰，峰，平，谷的累计电能）
	{0x33,0x32,0x39,0x33},
	//7,读取当前第三象限无功总电量+当前费率（尖峰，峰，平，谷的累计电能）
	{0x33,0x32,0x3A,0x33},
	//8,读取当前第四象限无功总电量+当前费率（尖峰，峰，平，谷的累计电能）
	{0x33,0x32,0x3B,0x33},
	
	//9,读取当前正向视在总电量+当前费率（尖峰，峰，平，谷的累计电能）
	{0x33,0x32,0x3C,0x33},
	//10,读取当前反向视在总电量+当前费率（尖峰，峰，平，谷的累计电能）
	{0x33,0x32,0x3D,0x33},
	
	//11,(当前)关联总电能
	{0x33,0x33,0xB3,0x33},
	//12,(当前)正向有功基波总电能
	{0x33,0x33,0xB4,0x33},
	//13,(当前)反向有功基波总电能
	{0x33,0x33,0xB5,0x33},
	//14,(当前)正向有功谐波总电能
	{0x33,0x33,0xB6,0x33},
	//15,(当前)反向有功谐波总电能
	{0x33,0x33,0xB7,0x33},
	//16,(当前)铜损有功总电能补偿量
	{0x33,0x33,0xB8,0x33},
	//17,(当前)铁损有功总电能补偿量
	{0x33,0x33,0xB9,0x33},
	
	//18,(当前)A 相正向有功电能
	{0x33,0x33,0x48,0x33},
	//19,(当前)A 相反向有功电能
	{0x33,0x33,0x49,0x33},
	//20,(当前)A 相组合无功 1 电能
	{0x33,0x33,0x4A,0x33},
	//21,(当前)A 相组合无功 2 电能
	{0x33,0x33,0x4B,0x33},
	//22,(当前)A 相第一象限无功电能
	{0x33,0x33,0x4C,0x33},
	//23,(当前)A 相第二象限无功电能
	{0x33,0x33,0x4D,0x33},
	//24,(当前)A 相第三象限无功电能
	{0x33,0x33,0x4E,0x33},
	//25,(当前)A 相第四象限无功电能
	{0x33,0x33,0x4F,0x33},
	//26,(当前)A 相正向视在电能
	{0x33,0x33,0x40,0x33},
	//27,(当前)A 相反向视在电能
	{0x33,0x33,0x41,0x33},
	//28,(当前)A 相关联电能
	{0x33,0x33,0xC7,0x33},
	//29,(当前)A 相正向有功基波电能
	{0x33,0x33,0xC8,0x33},
	//30,(当前)A 相反向有功基波电能
	{0x33,0x33,0xC9,0x33},
	//31,当前)A 相正向有功谐波电能
	{0x33,0x33,0xCA,0x33},
	//32,(当前)A 相反向有功谐波电能
	{0x33,0x33,0xCB,0x33},
	//33,(当前)A 相铜损有功电能补偿量
	{0x33,0x33,0xCC,0x33},
	//34,(当前)A 相铁损有功电能补偿量
	{0x33,0x33,0xCD,0x33},
	
	//35,(当前)B 相正向有功电能
	{0x33,0x33,0x5C,0x33},
	//36,(当前)B 相反向有功电能
	{0x33,0x33,0x5D,0x33},
	//37,(当前)B 相组合无功 1 电能
	{0x33,0x33,0x5E,0x33},
	//38,(当前)B 相组合无功 2 电能
	{0x33,0x33,0x5F,0x33},
	//39,(当前)B 相第一象限无功电能
	{0x33,0x33,0x50,0x33},
	//40,(当前)B 相第二象限无功电能
	{0x33,0x33,0x51,0x33},
	//41,(当前)B 相第三象限无功电能
	{0x33,0x33,0x52,0x33},
	//42,(当前)B 相第四象限无功电能
	{0x33,0x33,0x63,0x33},
	//43,(当前)B 相正向视在电能
	{0x33,0x33,0x64,0x33},
	//44,(当前)B 相反向视在电能
	{0x33,0x33,0x65,0x33},
	//45,(当前)B 相关联电能
	{0x33,0x33,0xDB,0x33},
	//46,(当前)B 相正向有功基波电能
	{0x33,0x33,0xDC,0x33},
	//47,(当前)B 相反向有功基波电能
	{0x33,0x33,0xDD,0x33},
	//48,(当前)B 相正向有功谐波电能
	{0x33,0x33,0xDE,0x33},
	//49,(当前)B 相反向有功谐波电能
	{0x33,0x33,0xDF,0x33},
	//50,(当前)A 相铜损有功电能补偿量
	{0x33,0x33,0xD0,0x33},
	//51,(当前)A 相铁损有功电能补偿量
	{0x33,0x33,0xD1,0x33},
	
	
	//52,(当前)C 相正向有功电能
	{0x33,0x33,0x60,0x33},
	//53,(当前)C 相反向有功电能
	{0x33,0x33,0x61,0x33},
	//54,(当前)C 相组合无功 1 电能
	{0x33,0x33,0x62,0x33},
	//55,(当前)C 相组合无功 2 电能
	{0x33,0x33,0x73,0x33},
	//56,(当前)C 相第一象限无功电能
	{0x33,0x33,0x74,0x33},
	//57,(当前)C 相第二象限无功电能
	{0x33,0x33,0x75,0x33},
	//58,(当前)C 相第三象限无功电能
	{0x33,0x33,0x76,0x33},
	//59,(当前)C 相第四象限无功电能
	{0x33,0x33,0x77,0x33},
	//60,(当前)C 相正向视在电能
	{0x33,0x33,0x78,0x33},
	//61,(当前)C 相反向视在电能
	{0x33,0x33,0x79,0x33},
	//62,(当前)C 相关联电能
	{0x33,0x33,0xEF,0x33},
	//63,(当前)C 相正向有功基波电能
	{0x33,0x33,0xE0,0x33},
	//64,(当前)C 相反向有功基波电能
	{0x33,0x33,0xE1,0x33},
	//65,(当前)C 相正向有功谐波电能
	{0x33,0x33,0xE2,0x33},
	//66,(当前)C 相反向有功谐波电能
	{0x33,0x33,0xF3,0x33},
	//67,(当前)C 相铜损有功电能补偿量
	{0x33,0x33,0xF4,0x33},
	//68,(当前)C 相铁损有功电能补偿量
	{0x33,0x33,0xF5,0x33},
	
	
	//69,A,B,C 相的电压
	{0x33,0x32,0x34,0x35},
	
	//70,A,B,C相电流
	{0x33,0x32,0x35,0x35},
	
	//71,瞬时有功功率数据
	{0x33,0x32,0x36,0x35},
	
	//72,瞬时无功功率数据
	{0x33,0x32,0x37,0x35},
	
	//73,总视在功率
	{0x33,0x32,0x38,0x35},
	
	//74,总功率因数数据
	{0x33,0x32,0x39,0x35},
	
	//75,相角数据块
	{0x33,0x32,0x3A,0x35},
	
	
};


/*************************************************
函数名称：Cmd_Bucket（）；
函数功能：得到电表指令；
入口参数：命令数字；
出口参数：命令结构体；

**************************************************/

Data_Frame Cmd_Bucket(int a)
{
	/*
	0:总能量+当前费率（尖峰，峰，平，谷的累计电能）；
	1:读取当前正向有功总电量+当前费率（尖峰，峰，平，谷的累计电能）;
	*/
	int i=0;
	int j=0;
	Data_Frame   Common_Cmd;
	Common_Cmd.Sta_Cha = START_CHAR ;
	for(i=0;i<6;i++)
	{
		Common_Cmd.addr[i]=Addr[ADDR_DEFINE][i];
	}
	Common_Cmd.Re_Sta_Cha = RE_START_CHAR;
	Common_Cmd.cmd = 0x11;
	Common_Cmd.len = 0x04;
	for(j=0;j<4;j++)
	{
		Common_Cmd.data[j]=Cmd[a][j];
	}
	Common_Cmd.CR = Add_Fun(Addr[ADDR_DEFINE],Cmd[a]);
	Common_Cmd.End = 0x16;
	
	return Common_Cmd;
}


/********************************************
函数名称：Read_Ack（）
函数功能：向电表发送相应的指令来获取数据
入口参数：命令结构体地址，命令长度（固定为16）
出口参数：无
*********************************************/
void Read_Ack(Data_Frame *p,uint8_t len)     //形参1为结构体的首地址 &read_back   形参2 为发送帧的长度
{
	uint8_t date,i;
	uint8_t Buff_Arry[16]={0};
	for(i=0;i<len;i++)
	{
		date = * (((uint8_t*) &p->Sta_Cha)+i); 		//每次循环将指针指向结构体中的下一个数据
		Buff_Arry[i] = date;
  	}
  	RS485_SendString(Buff_Arry,17);
}

/***********************************************************
函数名称：Add_Fun（）；
函数功能：CS校验产生；
入口参数：数组地址a，数组地址b；
出口参数：CS校验值；
************************************************************/

uint8_t Add_Fun(uint8_t *a,uint8_t *b)
{
    uint8_t add=0;
    uint8_t add1=0;
    uint8_t  add2=0;
    uint8_t i=0;
    uint8_t j=0;
    for(i=0;i<6;i++)
    {
        add1+=a[i];
    }


    for(j=0;j<4;j++)
    {
        add2+=b[j];
    }
    add=add1+add2+0x68+0x68+0x11+0x04;

    return add%256;
}

/*******
函数名称：Voltage_Process()；
函数功能：处理电压命令返回参数；
入口参数：接收数组，Data_Frame结构体
出口参数：无
********/

void  Voltage_Process(Rec_Data *temp,uint8_t *str ,Data_Frame* Temp )
{
	uint8_t i=0;
	uint8_t Rec_Frame[2];
	// Rec_Data  temp;
	uint8_t b[2]={0};
//	uint8_t c[2];
//	uint8_t d[2];

	for(i=0;i<2;i++)
	{
		Rec_Frame[i]=(str[18+i]-0x33);
	}
	for(i=0;i<2;i++)
	{
		Temp->Rec_Buff[1-i] = Rec_Frame[i];  //1,0
	}
//	for(i=2;i<4;i++)
//	{
//		Temp->Rec_Buff[5-i] = Rec_Frame[i];   //3,2
//	}
//	for(i=4;i<6;i++)
//	{
//		Temp->Rec_Buff[9-i] = Rec_Frame[i]; // 5,4
//	}
	// 得到数组，使得能够转换
	for(i=0;i<2;i++)
	{
		b[i]= Temp->Rec_Buff[i];//0,1
	}
	printf("b:0x%02x 0x%02x\r\n",b[0],b[1]);
//	for(i=2;i<4;i++)
//	{
//		c[i-2]= Temp->Rec_Buff[i];//0,1
//	}
//	for(i=4;i<6;i++)
//	{
//		d[i-4]= Temp->Rec_Buff[i];//0,1
//	}
//	Usart_SendString( USART1, b,2);
	temp->Voltage.A_Phase=To_Real_part(b,10,2);
//	temp.Voltage.B_Phase=To_Real_part(c,10,2);
//	temp.Voltage.C_Phase=To_Real_part(d,10,2);
}

/*******
函数名称：Current_Process()；
函数功能：处理电流命令返回参数；
入口参数：接收数组，Data_Frame结构体
出口参数：无
********/

Rec_Data  Current_Process(uint8_t *str ,Data_Frame* Temp )
{
	Rec_Data  temp;
	uint8_t b[3];
	uint8_t c[3];
	uint8_t d[3];
	uint8_t i=0;
	uint8_t Rec_Frame[9];

	for(i=0;i<9;i++)
	{
		Rec_Frame[i]=(str[18+i]-0x33);
	}
	for(i=0;i<3;i++)
	{
		Temp->Rec_Buff[2-i] = Rec_Frame[i];  //2,1,0
	}
	for(i=3;i<6;i++)
	{
		Temp->Rec_Buff[8-i] = Rec_Frame[i];   //5,4,3
	}
	for(i=6;i<9;i++)
	{
		Temp->Rec_Buff[14-i] = Rec_Frame[i]; // 8,7,6
	}
	//存入数组中
	for(i=0;i<3;i++)
	{
		b[i]= Temp->Rec_Buff[i];//0,1,2
	}
	for(i=3;i<6;i++)
	{
		c[i-3]= Temp->Rec_Buff[i];//0,1,2
	}
	for(i=6;i<9;i++)
	{
		d[i-6]= Temp->Rec_Buff[i];//0,1,2
	}
	temp.Current.A_Phase=To_Real_part(b,1000,3);
	temp.Current.B_Phase=To_Real_part(c,1000,3);
	temp.Current.C_Phase=To_Real_part(d,1000,3);
	return temp;
}


/*******
函数名称：Energy_Process()；
函数功能：处理功率命令返回参数；
入口参数：接收数组，Data_Frame结构体
出口参数：无
********/

Rec_Data  Energy_Process(uint8_t *str ,Data_Frame* Temp )
{
	Rec_Data  temp;
	uint8_t b[4];
	uint8_t c[4];
	uint8_t d[4];
	uint8_t e[4];
	uint8_t f[4];
	uint8_t i=0;
	uint8_t Rec_Frame[20];

	for(i=0;i<20;i++)
	{
		Rec_Frame[i]=(str[18+i]-0x33);
	}
	for(i=0;i<4;i++)
	{
		Temp->Rec_Buff[3-i] = Rec_Frame[i];  //3,2,1,0
	}
	for(i=4;i<8;i++)
	{
		Temp->Rec_Buff[11-i] = Rec_Frame[i];   //7,6,5,4
	}
	for(i=8;i<12;i++)
	{
		Temp->Rec_Buff[19-i] = Rec_Frame[i]; // 11,10,9,8
	}
		for(i=12;i<16;i++)
	{
		Temp->Rec_Buff[27-i] = Rec_Frame[i];   //15,14,13,12
	}
	for(i=16;i<20;i++)
	{
		Temp->Rec_Buff[35-i] = Rec_Frame[i]; // 19,18,17,16
	}
	//存入数组
	for(i=0;i<4;i++)
	{
		b[i]= Temp->Rec_Buff[i];//0,1,2,3
	}
	for(i=4;i<8;i++)
	{
		c[i-4]= Temp->Rec_Buff[i];//0,1,2,3
	}
	for(i=8;i<12;i++)
	{
		d[i-8]= Temp->Rec_Buff[i];//0,1,2,3
	}
	for(i=12;i<16;i++)
	{
		e[i-12]= Temp->Rec_Buff[i];//0,1,2,3
	}
	for(i=16;i<20;i++)
	{
		f[i-16]= Temp->Rec_Buff[i];//0,1,2,3
	}
	temp.Energy.Total   = To_Real_part(b,100,4);
	temp.Energy.Top     = To_Real_part(c,100,4);
	temp.Energy.Peak    = To_Real_part(d,100,4);
	temp.Energy.Flat    = To_Real_part(e,100,4);
	temp.Energy.Bottom  = To_Real_part(f,100,4);
	return temp;
}

/*******
函数名称：Power_Process()；
函数功能：处理功率命令返回参数；
入口参数：接收数组，Data_Frame结构体
出口参数：无
********/

Rec_Data  Power_Process(uint8_t *str ,Data_Frame* Temp )
{
	Rec_Data  temp;
	uint8_t b[3];
	uint8_t c[3];
	uint8_t d[3];
	uint8_t e[3];
	uint8_t i=0;
	uint8_t Rec_Frame[12];

	for(i=0;i<12;i++)
	{
		Rec_Frame[i]=(str[18+i]-0x33);
	}
	for(i=0;i<3;i++)
	{
		Temp->Rec_Buff[2-i] = Rec_Frame[i];  //2,1,0
	}
	for(i=3;i<6;i++)
	{
		Temp->Rec_Buff[8-i] = Rec_Frame[i];   //5,4,3
	}
	for(i=6;i<9;i++)
	{
		Temp->Rec_Buff[14-i] = Rec_Frame[i]; // 8,7,6
	}
	for(i=6;i<12;i++)
	{
		Temp->Rec_Buff[17-i] = Rec_Frame[i]; // 11,10,9
	}
	//存入数组
	for(i=0;i<3;i++)
	{
		b[i]= Temp->Rec_Buff[i];//0,1,2
	}
	for(i=3;i<6;i++)
	{
		c[i-3]= Temp->Rec_Buff[i];//0,1,2
	}
	for(i=6;i<9;i++)
	{
		d[i-6]= Temp->Rec_Buff[i];//0,1,2
	}
	for(i=9;i<12;i++)
	{
		e[i-9]= Temp->Rec_Buff[i];//0,1,2
	}
	
	temp.Power.Total=To_Real_part(b,10000,3);
	temp.Power.A_Phase=To_Real_part(c,10000,3);
	temp.Power.B_Phase=To_Real_part(d,10000,3);
	temp.Power.C_Phase=To_Real_part(e,10000,3);
	return temp;
}

/**********************
函数名称：Power_Factor_Process()；
函数功能：处理电压命令返回参数；
入口参数：接收数组，Data_Frame结构体
出口参数：无
**********************/

Rec_Data  Power_Factor_Process(uint8_t *str ,Data_Frame* Temp )
{
	Rec_Data  temp;
	uint8_t b[2];
	uint8_t c[2];
	uint8_t d[2];
	uint8_t e[2];
	uint8_t i=0;
	uint8_t Rec_Frame[8];

	for(i=0;i<8;i++)
	{
		Rec_Frame[i]=(str[18+i]-0x33);
	}
	for(i=0;i<2;i++)
	{
		Temp->Rec_Buff[1-i] = Rec_Frame[i];  //1,0
	}
	for(i=2;i<4;i++)
	{
		Temp->Rec_Buff[5-i] = Rec_Frame[i];   //3,2
	}
	for(i=4;i<6;i++)
	{
		Temp->Rec_Buff[9-i] = Rec_Frame[i]; // 5,4
	}
	for(i=6;i<8;i++)
	{
		Temp->Rec_Buff[13-i] = Rec_Frame[i]; // 7,6
	}
		//得到数组，使得能够转换
	for(i=0;i<2;i++)
	{
		b[i]= Temp->Rec_Buff[i];//0,1
	}
	for(i=2;i<4;i++)
	{
		c[i-2]= Temp->Rec_Buff[i];//0,1
	}
	for(i=4;i<6;i++)
	{
		d[i-4]= Temp->Rec_Buff[i];//0,1
	}
	for(i=6;i<8;i++)
	{
		e[i-6]= Temp->Rec_Buff[i];//0,1
	}
	
	temp.PowerFactor.OverAll=To_Real_part(b,1000,2);
	temp.PowerFactor.A_Phase=To_Real_part(c,1000,2);
	temp.PowerFactor.B_Phase=To_Real_part(d,1000,2);
	temp.PowerFactor.C_Phase=To_Real_part(e,1000,2);
	return temp;	
}



/**********************************************
函数名称：DataProcess（）；
函数功能：解析得到的数据，对其进行处理
入口参数：返回的数据数组,数据结构指针
出口参数：返回结构指针
*************************************************/
Rec_Data  DataProcess(uint8_t *str,Data_Frame*  Temp)
{
	Rec_Data  temp = {0};
	//电压处理函数
	// if((str[10]==0x33)&&(str[11]==0x32)&&(str[12]==0x34)&&(str[13]==0x35)) 
	printf("DataProcess:0x%02x,0x%02x,0x%02x,0x%02x.\r\n",str[14],str[15],str[16],str[17]);
	// printf("DataProcess:0x%02x,0x%02x,0x%02x,0x%02x.\r\n",*(str+14),*(str+15),*(str+16),*(str+17));
	// if((*(str+14)==0x33)&&(*(str+15)==0x32)&&(*(str+16)==0x34)&&(*(str+17)==0x35))//电流处理
	if(str[14] ==0x33 && str[15] ==0x32 && str[16] == 0x34 && str[17] == 0x35)
	{
		printf("process v.\r\n");
		Voltage_Process(&temp,str,Temp);
	}
	// //电流处理函数
	// else if((*(str+14)==0x33)&&(*(str+15)==0x32)&&(*(str+16)==0x35)&&(*(str+17)==0x35))//电流处理
	// {
	// 	 temp = Current_Process(str,Temp);
	// }
	// //能量处理
	// else if((*(str+14)==0x33)&&(*(str+15)==0x32)&&(*(str+16)==0x34)&&(*(str+17)==0x33))
	// {
	//   temp = Energy_Process(str ,Temp );
	// }
	// else if((*(str+14)==0x33)&&(*(str+15)==0x32)&&(*(str+16)==0x35)&&(*(str+17)==0x33))
	// {
	//   temp = Energy_Process(str ,Temp );
	// }
	// else if((*(str+14)==0x33)&&(*(str+15)==0x32)&&(*(str+16)==0x36)&&(*(str+17)==0x33))
	// {
	//   temp = Energy_Process(str ,Temp );
	// }
	// else if((*(str+14)==0x33)&&(*(str+15)==0x32)&&(*(str+16)==0x37)&&(*(str+17)==0x33))
	// {
	//   temp = Energy_Process(str ,Temp );
	// }
	// //功率处理
	// else if((*(str+14)==0x33)&&(*(str+15)==0x32)&&(*(str+16)==0x37)&&(*(str+17)==0x35))
	// {
	// 	temp = Power_Process( str , Temp );
	// }
	// else if((*(str+14)==0x33)&&(*(str+15)==0x32)&&(*(str+16)==0x36)&&(*(str+17)==0x35))
	// {
	// 	temp = Power_Process( str , Temp );
	// }
	// //功率因数处理
	// else if((*(str+14)==0x33)&&(*(str+15)==0x32)&&(*(str+16)==0x39)&&(*(str+17)==0x35))
	// {
	// 	temp = Power_Factor_Process( str , Temp );
	// }
	return temp;
}


/*******************************
函数名称：OutPut_Ack（）；
函数功能：输出命令检测结果是否正确
入口参数：Data_Frame结构体指针，输出数据长度；
出口参数：无
**********/

//void OutPut_Ack(Data_Frame *p,uint8_t len)     //形参1为结构体的首地址 &read_back   形参2 为发送帧的长度
//{
//	static uint8_t date,i;
//	uint8_t Buff_Arry[9];

//	for(i=0;i<len;i++)
//	{
//		date= * (((uint8_t*)&p->Rec_Buff)+i); 		//每次循环将指针指向结构体中的下一个数据
//    Buff_Arry[i] = date;
//  }
//	Usart_SendString(DEBUG_USART,Buff_Arry,len);
//}

//void DataPut_Ack(Rec_Data *p,uint8_t len)     //形参1为结构体的首地址 &read_back   形参2 为发送帧的长度
//{
//	static uint8_t date,i;
//	float Buff_Arry[3];

//	for(i=0;i<len;i++)
//	{
//		date= * (((uint8_t*)&p->Current)+i); 		//每次循环将指针指向结构体中的下一个数据
//    Buff_Arry[i] = date;
//  }
////	Usart_SendString1(DEBUG_USART,Buff_Arry,len);
//}

/*******************************
函数名称：To_Real_part（）；
函数功能：输出成实际的值
入口参数：数组，保留小数位,数组的实际数值；
出口参数：float数
*********************************/

float To_Real_part(uint8_t *str,uint16_t Res,uint8_t Len)
{
    unsigned char c1,c2;
	  uint8_t i;
    char s[10]={0};
    int c;
    float e;
    for( i=0;i<Len;i++)
    {
       c1=str[i]&0xF;
       c2=(str[i]>>4)&0xF;
       sprintf(s+i*2,"%x%x",c2,c1);
    }
    c=atoi(s);
    e=(float)c/((float)Res);
    return e;
}


/***************** 发送一个字符  **********************/
//使用单字节数据发送前要使能发送引脚，发送后要使能接收引脚。
void RS485_SendByte(  uint8_t ch )
{
	/* 发送一个字节数据到USART1 */
	// USART_SendData(RS485_USART,ch);
	uint8_t temp[1]={ch};
	HAL_UART_Transmit(&huart1,temp,1,0xffff);
	/* 等待发送完毕 */
}


/*****************  发送指定长度的字符串 **********************/
void RS485_SendStr_length( uint8_t *str,uint32_t strlen )
{
	unsigned int k=0;
    do 
    {
        RS485_SendByte( *(str + k) );
        k++;
    } while(k < strlen);
	/*加短暂延时，保证485发送数据完毕*/
}



/*****************  发送字符串 **********************/
void RS485_SendString( uint8_t *str, uint8_t num)
{
	unsigned int k=0;
	
	printf("send:");
    for(k=0;k<num;k++) 
    {
        RS485_SendByte(  *(str + k) );
		printf("%02x",*(str + k));
    } 
	printf("\r\n");
	  
	
}



int isspace(int x)
{
	if(x==' '||x=='\t'||x=='\n'||x=='\f'||x=='\b'||x=='\r')
		return 1;
		else
			return 0;
}

int isdigit(int x)
{
	if(x<='9'&&x>='0')
		return 1;
		else
			return 0;
}


int atoi(const char *nptr)
{
        int c;              /* current char */
        int total;         /* current total */
        int sign;           /* if '-', then negative, otherwise positive */

        /* skip whitespace */
        while ( isspace((int)(unsigned char)*nptr) )
            ++nptr;

        c = (int)(unsigned char)*nptr++;
        sign = c;           /* save sign indication */
        if (c == '-' || c == '+')
            c = (int)(unsigned char)*nptr++;    /* skip sign */

        total = 0;

    
				while (isdigit(c)) {
            total = 10 * total + (c - '0');     /* accumulate digit */
            c = (int)(unsigned char)*nptr++;    /* get next char */
        }

        if (sign == '-')
            return -total;
        else
            return total;   /* return result, negated if necessary */
}



