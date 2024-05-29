#include <dht11.h>//ͷ�ļ�

//дʱ��ĺ���
void DHT11_delay_us(uchar n)
{
    while (--n);
}

void DHT11_delay_ms(uint z)
{
    uint i, j;
    for (i = z; i > 0; i--)
        for (j = 122; j > 0; j--);
}

/*------------------------------------------------
              DHT11��ʼ�ź�
------------------------------------------------*/
void DHT11_start() {
	Data = 1;
	DHT11_delay_us(2);
	Data = 0;
	DHT11_delay_ms(20);   //��ʱ18ms����
	Data = 1;
	DHT11_delay_us(30);
}

/*------------------------------------------------
              ���հ�λ������
------------------------------------------------*/
uchar DHT11_rec_byte()
{
    uchar i, dat = 0;
    for (i = 0; i < 8; i++)
    {
        while (!Data);
        DHT11_delay_us(8);
        dat <<= 1;
        if (Data == 1)
            dat += 1;
        while (Data);
    }
    return dat;
}

/*------------------------------------------------
              ����40bit����
------------------------------------------------*/
void getdata(uchar *sensordata) {    //����40λ������
	uchar R_H, R_L, T_H, T_L, revise;
	DHT11_start();

	if (Data == 0) {

		while (Data == 0); //�ȴ�����

		DHT11_delay_us(40);  //���ߺ���ʱ80us
		R_H = DHT11_rec_byte();  //����ʪ�ȸ߰�λ
		R_L = DHT11_rec_byte();  //����ʪ�ȵͰ�λ
		T_H = DHT11_rec_byte();  //�����¶ȸ߰�λ
		T_L = DHT11_rec_byte();  //�����¶ȵͰ�λ
		revise = DHT11_rec_byte(); //����У��λ

		DHT11_delay_us(25);    //����

		sensordata[0] = T_H;
		sensordata[1] = R_H;

	}
}

