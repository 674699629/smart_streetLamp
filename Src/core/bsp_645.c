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
	//0,��ȡ��ǰ�ܵ���+��ǰ���ʣ���壬�壬ƽ���ȵ��ۼƵ��ܣ�
	{0x33,0x32,0x34,0x33},
	//1,��ȡ��ǰ�����й��ܵ���+��ǰ���ʣ���壬�壬ƽ���ȵ��ۼƵ��ܣ�
	{0x33,0x32,0x34,0x33},
	//2,��ȡ��ǰ�����й��ܵ���+��ǰ���ʣ���壬�壬ƽ���ȵ��ۼƵ��ܣ�
	{0x33,0x32,0x35,0x33},
	//3,��ȡ��ǰ�����޹��ܵ���+��ǰ���ʣ���壬�壬ƽ���ȵ��ۼƵ��ܣ�
	{0x33,0x32,0x36,0x33},
	//4,��ȡ��ǰ�����޹��ܵ���+��ǰ���ʣ���壬�壬ƽ���ȵ��ۼƵ��ܣ�
	{0x33,0x32,0x37,0x33},	
	
	//5,��ȡ��ǰ��һ�����޹��ܵ���+��ǰ���ʣ���壬�壬ƽ���ȵ��ۼƵ��ܣ�
	{0x33,0x32,0x38,0x33},
	//6,��ȡ��ǰ�ڶ������޹��ܵ���+��ǰ���ʣ���壬�壬ƽ���ȵ��ۼƵ��ܣ�
	{0x33,0x32,0x39,0x33},
	//7,��ȡ��ǰ���������޹��ܵ���+��ǰ���ʣ���壬�壬ƽ���ȵ��ۼƵ��ܣ�
	{0x33,0x32,0x3A,0x33},
	//8,��ȡ��ǰ���������޹��ܵ���+��ǰ���ʣ���壬�壬ƽ���ȵ��ۼƵ��ܣ�
	{0x33,0x32,0x3B,0x33},
	
	//9,��ȡ��ǰ���������ܵ���+��ǰ���ʣ���壬�壬ƽ���ȵ��ۼƵ��ܣ�
	{0x33,0x32,0x3C,0x33},
	//10,��ȡ��ǰ���������ܵ���+��ǰ���ʣ���壬�壬ƽ���ȵ��ۼƵ��ܣ�
	{0x33,0x32,0x3D,0x33},
	
	//11,(��ǰ)�����ܵ���
	{0x33,0x33,0xB3,0x33},
	//12,(��ǰ)�����й������ܵ���
	{0x33,0x33,0xB4,0x33},
	//13,(��ǰ)�����й������ܵ���
	{0x33,0x33,0xB5,0x33},
	//14,(��ǰ)�����й�г���ܵ���
	{0x33,0x33,0xB6,0x33},
	//15,(��ǰ)�����й�г���ܵ���
	{0x33,0x33,0xB7,0x33},
	//16,(��ǰ)ͭ���й��ܵ��ܲ�����
	{0x33,0x33,0xB8,0x33},
	//17,(��ǰ)�����й��ܵ��ܲ�����
	{0x33,0x33,0xB9,0x33},
	
	//18,(��ǰ)A �������й�����
	{0x33,0x33,0x48,0x33},
	//19,(��ǰ)A �෴���й�����
	{0x33,0x33,0x49,0x33},
	//20,(��ǰ)A ������޹� 1 ����
	{0x33,0x33,0x4A,0x33},
	//21,(��ǰ)A ������޹� 2 ����
	{0x33,0x33,0x4B,0x33},
	//22,(��ǰ)A ���һ�����޹�����
	{0x33,0x33,0x4C,0x33},
	//23,(��ǰ)A ��ڶ������޹�����
	{0x33,0x33,0x4D,0x33},
	//24,(��ǰ)A ����������޹�����
	{0x33,0x33,0x4E,0x33},
	//25,(��ǰ)A ����������޹�����
	{0x33,0x33,0x4F,0x33},
	//26,(��ǰ)A ���������ڵ���
	{0x33,0x33,0x40,0x33},
	//27,(��ǰ)A �෴�����ڵ���
	{0x33,0x33,0x41,0x33},
	//28,(��ǰ)A ���������
	{0x33,0x33,0xC7,0x33},
	//29,(��ǰ)A �������й���������
	{0x33,0x33,0xC8,0x33},
	//30,(��ǰ)A �෴���й���������
	{0x33,0x33,0xC9,0x33},
	//31,��ǰ)A �������й�г������
	{0x33,0x33,0xCA,0x33},
	//32,(��ǰ)A �෴���й�г������
	{0x33,0x33,0xCB,0x33},
	//33,(��ǰ)A ��ͭ���й����ܲ�����
	{0x33,0x33,0xCC,0x33},
	//34,(��ǰ)A �������й����ܲ�����
	{0x33,0x33,0xCD,0x33},
	
	//35,(��ǰ)B �������й�����
	{0x33,0x33,0x5C,0x33},
	//36,(��ǰ)B �෴���й�����
	{0x33,0x33,0x5D,0x33},
	//37,(��ǰ)B ������޹� 1 ����
	{0x33,0x33,0x5E,0x33},
	//38,(��ǰ)B ������޹� 2 ����
	{0x33,0x33,0x5F,0x33},
	//39,(��ǰ)B ���һ�����޹�����
	{0x33,0x33,0x50,0x33},
	//40,(��ǰ)B ��ڶ������޹�����
	{0x33,0x33,0x51,0x33},
	//41,(��ǰ)B ����������޹�����
	{0x33,0x33,0x52,0x33},
	//42,(��ǰ)B ����������޹�����
	{0x33,0x33,0x63,0x33},
	//43,(��ǰ)B ���������ڵ���
	{0x33,0x33,0x64,0x33},
	//44,(��ǰ)B �෴�����ڵ���
	{0x33,0x33,0x65,0x33},
	//45,(��ǰ)B ���������
	{0x33,0x33,0xDB,0x33},
	//46,(��ǰ)B �������й���������
	{0x33,0x33,0xDC,0x33},
	//47,(��ǰ)B �෴���й���������
	{0x33,0x33,0xDD,0x33},
	//48,(��ǰ)B �������й�г������
	{0x33,0x33,0xDE,0x33},
	//49,(��ǰ)B �෴���й�г������
	{0x33,0x33,0xDF,0x33},
	//50,(��ǰ)A ��ͭ���й����ܲ�����
	{0x33,0x33,0xD0,0x33},
	//51,(��ǰ)A �������й����ܲ�����
	{0x33,0x33,0xD1,0x33},
	
	
	//52,(��ǰ)C �������й�����
	{0x33,0x33,0x60,0x33},
	//53,(��ǰ)C �෴���й�����
	{0x33,0x33,0x61,0x33},
	//54,(��ǰ)C ������޹� 1 ����
	{0x33,0x33,0x62,0x33},
	//55,(��ǰ)C ������޹� 2 ����
	{0x33,0x33,0x73,0x33},
	//56,(��ǰ)C ���һ�����޹�����
	{0x33,0x33,0x74,0x33},
	//57,(��ǰ)C ��ڶ������޹�����
	{0x33,0x33,0x75,0x33},
	//58,(��ǰ)C ����������޹�����
	{0x33,0x33,0x76,0x33},
	//59,(��ǰ)C ����������޹�����
	{0x33,0x33,0x77,0x33},
	//60,(��ǰ)C ���������ڵ���
	{0x33,0x33,0x78,0x33},
	//61,(��ǰ)C �෴�����ڵ���
	{0x33,0x33,0x79,0x33},
	//62,(��ǰ)C ���������
	{0x33,0x33,0xEF,0x33},
	//63,(��ǰ)C �������й���������
	{0x33,0x33,0xE0,0x33},
	//64,(��ǰ)C �෴���й���������
	{0x33,0x33,0xE1,0x33},
	//65,(��ǰ)C �������й�г������
	{0x33,0x33,0xE2,0x33},
	//66,(��ǰ)C �෴���й�г������
	{0x33,0x33,0xF3,0x33},
	//67,(��ǰ)C ��ͭ���й����ܲ�����
	{0x33,0x33,0xF4,0x33},
	//68,(��ǰ)C �������й����ܲ�����
	{0x33,0x33,0xF5,0x33},
	
	
	//69,A,B,C ��ĵ�ѹ
	{0x33,0x32,0x34,0x35},
	
	//70,A,B,C�����
	{0x33,0x32,0x35,0x35},
	
	//71,˲ʱ�й���������
	{0x33,0x32,0x36,0x35},
	
	//72,˲ʱ�޹���������
	{0x33,0x32,0x37,0x35},
	
	//73,�����ڹ���
	{0x33,0x32,0x38,0x35},
	
	//74,�ܹ�����������
	{0x33,0x32,0x39,0x35},
	
	//75,������ݿ�
	{0x33,0x32,0x3A,0x35},
	
	
};


/*************************************************
�������ƣ�Cmd_Bucket������
�������ܣ��õ����ָ�
��ڲ������������֣�
���ڲ���������ṹ�壻

**************************************************/

Data_Frame Cmd_Bucket(int a)
{
	/*
	0:������+��ǰ���ʣ���壬�壬ƽ���ȵ��ۼƵ��ܣ���
	1:��ȡ��ǰ�����й��ܵ���+��ǰ���ʣ���壬�壬ƽ���ȵ��ۼƵ��ܣ�;
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
�������ƣ�Read_Ack����
�������ܣ���������Ӧ��ָ������ȡ����
��ڲ���������ṹ���ַ������ȣ��̶�Ϊ16��
���ڲ�������
*********************************************/
void Read_Ack(Data_Frame *p,uint8_t len)     //�β�1Ϊ�ṹ����׵�ַ &read_back   �β�2 Ϊ����֡�ĳ���
{
	uint8_t date,i;
	uint8_t Buff_Arry[16]={0};
	for(i=0;i<len;i++)
	{
		date = * (((uint8_t*) &p->Sta_Cha)+i); 		//ÿ��ѭ����ָ��ָ��ṹ���е���һ������
		Buff_Arry[i] = date;
  	}
  	RS485_SendString(Buff_Arry,17);
}

/***********************************************************
�������ƣ�Add_Fun������
�������ܣ�CSУ�������
��ڲ����������ַa�������ַb��
���ڲ�����CSУ��ֵ��
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
�������ƣ�Voltage_Process()��
�������ܣ������ѹ����ز�����
��ڲ������������飬Data_Frame�ṹ��
���ڲ�������
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
	// �õ����飬ʹ���ܹ�ת��
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
�������ƣ�Current_Process()��
�������ܣ������������ز�����
��ڲ������������飬Data_Frame�ṹ��
���ڲ�������
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
	//����������
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
�������ƣ�Energy_Process()��
�������ܣ�����������ز�����
��ڲ������������飬Data_Frame�ṹ��
���ڲ�������
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
	//��������
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
�������ƣ�Power_Process()��
�������ܣ�����������ز�����
��ڲ������������飬Data_Frame�ṹ��
���ڲ�������
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
	//��������
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
�������ƣ�Power_Factor_Process()��
�������ܣ������ѹ����ز�����
��ڲ������������飬Data_Frame�ṹ��
���ڲ�������
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
		//�õ����飬ʹ���ܹ�ת��
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
�������ƣ�DataProcess������
�������ܣ������õ������ݣ�������д���
��ڲ��������ص���������,���ݽṹָ��
���ڲ��������ؽṹָ��
*************************************************/
Rec_Data  DataProcess(uint8_t *str,Data_Frame*  Temp)
{
	Rec_Data  temp = {0};
	//��ѹ������
	// if((str[10]==0x33)&&(str[11]==0x32)&&(str[12]==0x34)&&(str[13]==0x35)) 
	printf("DataProcess:0x%02x,0x%02x,0x%02x,0x%02x.\r\n",str[14],str[15],str[16],str[17]);
	// printf("DataProcess:0x%02x,0x%02x,0x%02x,0x%02x.\r\n",*(str+14),*(str+15),*(str+16),*(str+17));
	// if((*(str+14)==0x33)&&(*(str+15)==0x32)&&(*(str+16)==0x34)&&(*(str+17)==0x35))//��������
	if(str[14] ==0x33 && str[15] ==0x32 && str[16] == 0x34 && str[17] == 0x35)
	{
		printf("process v.\r\n");
		Voltage_Process(&temp,str,Temp);
	}
	// //����������
	// else if((*(str+14)==0x33)&&(*(str+15)==0x32)&&(*(str+16)==0x35)&&(*(str+17)==0x35))//��������
	// {
	// 	 temp = Current_Process(str,Temp);
	// }
	// //��������
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
	// //���ʴ���
	// else if((*(str+14)==0x33)&&(*(str+15)==0x32)&&(*(str+16)==0x37)&&(*(str+17)==0x35))
	// {
	// 	temp = Power_Process( str , Temp );
	// }
	// else if((*(str+14)==0x33)&&(*(str+15)==0x32)&&(*(str+16)==0x36)&&(*(str+17)==0x35))
	// {
	// 	temp = Power_Process( str , Temp );
	// }
	// //������������
	// else if((*(str+14)==0x33)&&(*(str+15)==0x32)&&(*(str+16)==0x39)&&(*(str+17)==0x35))
	// {
	// 	temp = Power_Factor_Process( str , Temp );
	// }
	return temp;
}


/*******************************
�������ƣ�OutPut_Ack������
�������ܣ�������������Ƿ���ȷ
��ڲ�����Data_Frame�ṹ��ָ�룬������ݳ��ȣ�
���ڲ�������
**********/

//void OutPut_Ack(Data_Frame *p,uint8_t len)     //�β�1Ϊ�ṹ����׵�ַ &read_back   �β�2 Ϊ����֡�ĳ���
//{
//	static uint8_t date,i;
//	uint8_t Buff_Arry[9];

//	for(i=0;i<len;i++)
//	{
//		date= * (((uint8_t*)&p->Rec_Buff)+i); 		//ÿ��ѭ����ָ��ָ��ṹ���е���һ������
//    Buff_Arry[i] = date;
//  }
//	Usart_SendString(DEBUG_USART,Buff_Arry,len);
//}

//void DataPut_Ack(Rec_Data *p,uint8_t len)     //�β�1Ϊ�ṹ����׵�ַ &read_back   �β�2 Ϊ����֡�ĳ���
//{
//	static uint8_t date,i;
//	float Buff_Arry[3];

//	for(i=0;i<len;i++)
//	{
//		date= * (((uint8_t*)&p->Current)+i); 		//ÿ��ѭ����ָ��ָ��ṹ���е���һ������
//    Buff_Arry[i] = date;
//  }
////	Usart_SendString1(DEBUG_USART,Buff_Arry,len);
//}

/*******************************
�������ƣ�To_Real_part������
�������ܣ������ʵ�ʵ�ֵ
��ڲ��������飬����С��λ,�����ʵ����ֵ��
���ڲ�����float��
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


/***************** ����һ���ַ�  **********************/
//ʹ�õ��ֽ����ݷ���ǰҪʹ�ܷ������ţ����ͺ�Ҫʹ�ܽ������š�
void RS485_SendByte(  uint8_t ch )
{
	/* ����һ���ֽ����ݵ�USART1 */
	// USART_SendData(RS485_USART,ch);
	uint8_t temp[1]={ch};
	HAL_UART_Transmit(&huart1,temp,1,0xffff);
	/* �ȴ�������� */
}


/*****************  ����ָ�����ȵ��ַ��� **********************/
void RS485_SendStr_length( uint8_t *str,uint32_t strlen )
{
	unsigned int k=0;
    do 
    {
        RS485_SendByte( *(str + k) );
        k++;
    } while(k < strlen);
	/*�Ӷ�����ʱ����֤485�����������*/
}



/*****************  �����ַ��� **********************/
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



