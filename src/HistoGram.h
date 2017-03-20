#pragma once

#if !defined(HISTOGRAM_H)
#define HISTOGRAM_H
/*
//����һ������ֱ��ͼ�Ļ��࣬������ֱ��ͼ��һЩ�㷨����ֱ��ͼ�ļ��㣬ֱ��ͼ����
//�Լ���ص�һЩ�Ľ��㷨���ڵ���ʱ�����Ը�����Ҫ���иĽ�����Ҫָ������
//ֱ��ͼ����ԻҶ�ͼ�ģ������������һ�������㷨��Ҳ����ֻ��������صġ�����ʱ
//��ͷ�ļ�������ȥ
//�㷨����ߣ���Ԩ��
//ʱ��2009-3-28
*/
//#include <QtCore>
//#include "BaseImages.h"
#include "memory.h"
#include "math.h"

class Histogram// :public LLBaseImages
{
	//Q_OBJECT

public:
   Histogram( int ImageHeight,int ImageWidth,int **ImageData)
    {//���캯��     
	PicWidth=ImageWidth;
	PicHeight=ImageHeight;
	PicData=ImageData;//���ݴ���
	//��ֱ��ͼ���Ҷȼ��ĸ�����ֵΪ0
	memset(Histo,0,sizeof(int)*256);//�����ڴ棬��ÿ����ֵΪ0
    //ͳ��ֱ��ͼ������
	Statistic_Hist(); 
	}
	
   
   //
	~Histogram(void){}
	//
    int * Get_Hist()
	{//�������ֱ��ͼ����
		return Histo;
	}
	
	//�õ�ֱ��ͼ����
    void Statistic_Hist()
	{
	  int i,j;
	  //ͳ��
	  memset(Histo,0,sizeof(int)*256);//�����ڴ棬��ÿ����ֵΪ0
	 
	  for(i=0;i<PicHeight;i++)
	   { for(j=0;j<PicWidth;j++)
		{
		 Histo[PicData[i][j]]++;//�Ծ��д˻Ҷȼ���ֱ��ͼ��1
		}
	   }
	}
	//
	//�����漰һ���ۺϵ�ֱ��ͼ�����㷨��������ͬ������Ͳ���

	unsigned char * Equal_Hist(int mod,double Mvalue)
	{//mod��ֱ��ͼ�㷨�����࣬Mvalue�Ǽ��ټ򲢶������Ĳ���
	unsigned char * ByteTemp=new unsigned char[256];
		
	switch(mod)
		{
		case 1:
			ByteTemp=Classic_Equal();//�����ֱ��ͼ�㷨
			break;
		case 2:
			ByteTemp=TY_JIA_Equal(Mvalue);//���ǸĽ����㷨
			break;
		default:
			break;
		}

		return ByteTemp;
	}

	//���洦����Ǵ�ͳֱ��ͼ���ⷽ��ĳ���
	unsigned char * Classic_Equal()
		{//���Ѿ����ڵĻҶ�ͼ�������¾��⻯
           int i,j;
		     long nTemp;
			int k;
			double temp;
			double PicSize;//����ͼ�����������
			PicSize=(double)(PicWidth*PicHeight);
			
			//�����ۼ�ֱ��ͼ
			for(i=0;i<256;i++)
			{
				nTemp=0;
				for(j=0;j<=i;j++)
				{
					nTemp+=Histo[j];//�ۼӻҶ���,����ӵ���һ��Ҫע��
				}
				//�����¶�Ӧ���»Ҷ�ֵ
				temp=(double)(0.5+255.0*nTemp/PicSize);
				k=(unsigned char)(temp);//�Ƚ���תΪ����ֵ
				ByteMap[i]=k;//�ٽ���תΪ
			}
			
			return ByteMap;//���ػҶ�ӳ���
		}
	//
   //���洦���ֱ��ͼ�������ɺ��ϴ�ѧ����־���ṩȻ���޸ĵ�
	unsigned char * Tang_JIA_Equal(double Mvalue)
		{//������������иĽ���,�����MValue�����˹�����
		   int i,j,k;
		  long nTemp;
		  //double Mvalue;
		  double ptemp,PicSize;
		
		  Mvalue=0.5;//��ȡ�������
        
		  int* temp=new int[256];
		  nTemp=0;

		  for(i=0;i<256;i++)
		  {//��������
			  if(Histo[i]!=0)
			  {
				  temp[i]=(int)(exp(Mvalue*log((double)Histo[i])));//��������
				  nTemp+=temp[i];//�����µ���������
			  }
		  }

		  PicSize=(double)nTemp;//����ͼ����������
        //��������ۻ�ֱ��ͼ
		  for(i=0;i<256;i++)
		  {
			  nTemp=0;
			  for(j=0;j<=i;j++)
			  {
				  nTemp+=temp[j];//�ۼӻҶ���,����ӵ���һ��Ҫע��
			  }
			  //�����¶�Ӧ���»Ҷ�ֵ
			  ptemp=(double)(0.5+255.0*nTemp/PicSize);
			  k=(unsigned char)(ptemp);//�Ƚ���תΪ����ֵ
			  ByteMap[i]=k;//�ٽ���תΪ
		  }
         return ByteMap;
		}

	
	//�������������Ҧ�����ṩ��һ���㷨
	unsigned char * YAO_JIA_Equal()
		{
			//������֪����ͼ���ֱ��ͼ������ֱ�ӽ�������
			int i,j;
			long nTemp;
			int k;
			double temp;
			double PicSize;//����ͼ�����������
			PicSize=(double)(PicWidth*PicHeight);
			
			//�����ۼ�ֱ��ͼ
			for(i=0;i<255;i++)
			{
				nTemp=0;
				for(j=0;j<i;j++)
				{
					nTemp+=Histo[j];//�ۼӻҶ���,����ӵ���һ��Ҫע��
				}
				//�����¶�Ӧ���»Ҷ�ֵ
				temp=(double)(0.5+255.0*nTemp/(PicSize-Histo[i]));
				k=(unsigned char)(temp);//�Ƚ���תΪ����ֵ
				ByteMap[i]=k;//�ٽ���תΪ
			}
			return ByteMap;//���ػҶ�ӳ���				  
		}
		
		
		//�������ۺ����������㷨������һ���㷨
		unsigned char * TY_JIA_Equal(double Mvalue)
		{
			int i,j,k;
			long nTemp;
			
			double ptemp,PicSize;
			
			//���������������������µ�
			unsigned char * ByteTemp=new unsigned char[256];
			int Sum;
           //
			int * temp=new int[256];//�����µĻҶ�Ƶ��
			//unsigned char * byteTemp=new unsigned char[256];//�ݴ�
			nTemp=0;
			for(i=0;i<256;i++)
			{//��������
				temp[i]=0;//�ȶ�
				if(Histo[i]!=0)
				{
					temp[i]=(int)(exp(Mvalue*log((double)(Histo[i]))));//��������
					nTemp+=temp[i];//�����µ���������
				}
			}
			PicSize=(double)nTemp;//����ͼ����������
			//��������ۻ�ֱ��ͼ
			//�Ե�һ������ֱ�Ӹ�ֵ
			//
	       ByteMap[0]=(unsigned char)0;
			for(i=1;i<255;i++)
			{
				nTemp=0;
				for(j=0;j<i;j++)
				{
					nTemp+=temp[j];//�ۼӻҶ���,����ӵ���һ��Ҫע��
				}
				//�����¶�Ӧ���»Ҷ�ֵ
				ptemp=(double)(0.5+255.0*nTemp/(PicSize-temp[i]));
				//ptemp=(double)(255.0*nTemp/PicSize);
				k=(unsigned char)(ptemp);//�Ƚ���תΪ����ֵ
				ByteMap[i]=k;//�ٽ���תΪ
			}
			ByteMap[255]=(unsigned char)255;
			//���������������ɳ�Ǯ�ṩ�ģ������˽��Ҷ��ٵȼ������			
			for(i=0;i<256;i++)
			{
				ByteTemp[i]=0;//�����Ҷȵ�Ƶ��
			}
			//����
			for(i=0;i<256;i++)
			{
				if(Histo[i]!=0)
				{//֤���任֮��ĻҶ�����Ч���������ĻҶȿ����Ǽ򲢵ġ�
					ByteTemp[ByteMap[i]]++;
				}
			}
			//
			Sum=0;//����������Ч�Ҷȼ�����
			for(i=0;i<256;i++)
			{
				if(ByteTemp[i]!=0)
					Sum++;
			}
			PicSize=(double)Sum;
			//���ڽ����ۼ�
			for(i=0;i<256;i++)
			{
				nTemp=0;
				for(j=0;j<=ByteMap[i];j++)
				{//�����ѭ������Ǽ���ģ���Ϊ�Ƕ�ByteMap[i]���Ե�
				//���в�����Ч�ĻҶȼ����еķ�Χ
					if(ByteTemp[j]!=0)
						nTemp++;
				}
				ptemp=(double)(0.5+255.0*(nTemp-1.0)/(PicSize-1.0));
				k=(unsigned char)ptemp;
				ByteMap[i]=k;
			}

			return ByteMap;
		}

		
		//������������ǽ���Ǯ������ʱ�����¿���
		unsigned char * Chen()
		{
			//byte[] myMap;//����һ��������н���
			unsigned char * byteTemp=new unsigned char[256];
			int i,j,nTemp,Sum;
			double ptemp;
			unsigned char k;
			//�ȵõ�ԭ�ȵĻҶ�ӳ���
			//myMap=TY_JIA_Equal();
			//�������ͳ��
			
			for(i=0;i<256;i++)
			{
				byteTemp[i]=0;//�����Ҷȵ�Ƶ��
			}
			//����
			for(i=0;i<256;i++)
			{
				if(Histo[i]!=0)
				{
					byteTemp[i]++;
				}
			}

			Sum=0;
			for(i=0;i<256;i++)
			{
				if(byteTemp[i]!=0)
					Sum++;
			}
			//���ڽ����ۼ�
			for(i=0;i<256;i++)
			{
				nTemp=0;
				for(j=0;j<=i;j++)
				{
					if(byteTemp[j]!=0)
						nTemp++;
				}
				ptemp=(double)(0.5+255.0*(nTemp)/(Sum));
				k=(unsigned char)ptemp;
				ByteMap[i]=k;
			}
			//���ڸ���ȥ
			return ByteMap;
		}
		

		/*������㷨��������������ṩ�Ļ���ֱ��ͼ��
		//�Ҷȷ����������㷨
		unsigned char * NonLinear()
		{
            //�����ԭ��ͼ�������ֵ����Сֵ
			
			int i,KValue;
			
		
			//��һ�����������������������
			Sum=0;
			for(i=81;i<149;i++)
				Sum+=Hist[i];
			Sum+=((Hist[80]+Hist[150])/2);
			//�����ȵõ�һ��
			KValue=(int)(60+60*Hist[80]/Sum);
			ByteMap[80]=(byte)KValue;
			//���������һ���Ҷ�����
			for(i=81;i<150;i++)
			{
				KValue+=(int)(60*(Hist[i-1]+Hist[i])/Sum);
				ByteMap[i]=(byte)KValue;
			}
			//����ԻҶ�����ӳ��
            //�ȼ�¼��һ����С��ֵ
			ByteMap[MinValue]=(byte)(128*Hist[MinValue]/SumOfPic);
			KValue=ByteMap[MinValue];//�ȼ�¼����
			for(i=MinValue+1;i<MaxValue;i++)
			{
				KValue+=Convert.ToInt32((128*(Hist[i-1]+Hist[i]))/SumOfPic);//���µ�һ������������
				ByteMap[i]=(byte)KValue;
			}
			ByteMap[MaxValue]=(byte)255;//ֱ�Ӹ�ֵ
			//�����ٽ��о���
			//���������������ɳ�Ǯ�ṩ�ģ������˽��Ҷ��ٵȼ������
			nTemp=0;
			//�ȼ���Ҷ��ۻ�ֵ
			for(i=0;i<256;i++)
			{
				if(Hist[i]!=0)
					nTemp+=ByteMap[i];
			}
			//������֮����
			PicSize=nTemp;
			//�����ٽ�������ֱ��ͼ����
			for(i=0;i<256;i++)
			{
				nTemp=0;
				for(j=0;j<=i;j++)
				{
					if(Hist[j]!=0)
						nTemp+=ByteMap[j];
				}
				//����������¸�ֵ
				ptemp=(double)(MinValue+0.5+(255.0-MinValue)*nTemp/PicSize);
				k=(int)ptemp;
				byteTemp[i]=(byte)k;
			}
			//�����ڸ���ȥ
			for(i=0;i<256;i++)
				ByteMap[i]=byteTemp[i];
            return ByteMap;
		}*/
      



private:
   int PicWidth;//ͼ��Ŀ�����
   int PicHeight;//ͼ��ĸߣ�����
   int **PicData;//�����ά�����ָ�������洢�Ҷ�ͼ������
   int Histo[256];//ֱ��ͼ������,�洢���Ҷȼ��ĸ���
   unsigned char ByteMap[256];//�Ҷ�ӳ���

   //
};

#endif

