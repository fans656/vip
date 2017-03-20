#pragma once

#if !defined(HISTOGRAM_H)
#define HISTOGRAM_H
/*
//这是一个描述直方图的基类，包括了直方图的一些算法，如直方图的计算，直方图均衡
//以及相关的一些改进算法，在调用时，可以根据需要进行改进，需要指出的是
//直方图是针对灰度图的，这里给出的是一个基本算法，也就是只和数据相关的。调用时
//将头文件包括进去
//算法设计者：贾渊，
//时间2009-3-28
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
    {//构造函数     
	PicWidth=ImageWidth;
	PicHeight=ImageHeight;
	PicData=ImageData;//数据传入
	//将直方图各灰度级的个数赋值为0
	memset(Histo,0,sizeof(int)*256);//申请内存，且每个赋值为0
    //统计直方图的数据
	Statistic_Hist(); 
	}
	
   
   //
	~Histogram(void){}
	//
    int * Get_Hist()
	{//返回这个直方图数组
		return Histo;
	}
	
	//得到直方图数据
    void Statistic_Hist()
	{
	  int i,j;
	  //统计
	  memset(Histo,0,sizeof(int)*256);//申请内存，且每个赋值为0
	 
	  for(i=0;i<PicHeight;i++)
	   { for(j=0;j<PicWidth;j++)
		{
		 Histo[PicData[i][j]]++;//对具有此灰度级的直方图加1
		}
	   }
	}
	//
	//这里涉及一个综合的直方图均衡算法，包括不同的情况和参数

	unsigned char * Equal_Hist(int mod,double Mvalue)
	{//mod是直方图算法的种类，Mvalue是减少简并而给出的参数
	unsigned char * ByteTemp=new unsigned char[256];
		
	switch(mod)
		{
		case 1:
			ByteTemp=Classic_Equal();//经典的直方图算法
			break;
		case 2:
			ByteTemp=TY_JIA_Equal(Mvalue);//这是改进的算法
			break;
		default:
			break;
		}

		return ByteTemp;
	}

	//下面处理的是传统直方图均衡方面的程序
	unsigned char * Classic_Equal()
		{//对已经存在的灰度图进行重新均衡化
           int i,j;
		     long nTemp;
			int k;
			double temp;
			double PicSize;//这是图像的像素总数
			PicSize=(double)(PicWidth*PicHeight);
			
			//计算累计直方图
			for(i=0;i<256;i++)
			{
				nTemp=0;
				for(j=0;j<=i;j++)
				{
					nTemp+=Histo[j];//累加灰度数,这里加的量一定要注意
				}
				//计算新对应的新灰度值
				temp=(double)(0.5+255.0*nTemp/PicSize);
				k=(unsigned char)(temp);//先将其转为整数值
				ByteMap[i]=k;//再将其转为
			}
			
			return ByteMap;//返回灰度映射表
		}
	//
   //下面处理的直方图均衡是由湖南大学的唐志文提供然后修改的
	unsigned char * Tang_JIA_Equal(double Mvalue)
		{//这个方法是稍有改进的,这里的MValue是由人工输入
		   int i,j,k;
		  long nTemp;
		  //double Mvalue;
		  double ptemp,PicSize;
		
		  Mvalue=0.5;//采取均衡的量
        
		  int* temp=new int[256];
		  nTemp=0;

		  for(i=0;i<256;i++)
		  {//重新修正
			  if(Histo[i]!=0)
			  {
				  temp[i]=(int)(exp(Mvalue*log((double)Histo[i])));//这是修正
				  nTemp+=temp[i];//计算新的像素总数
			  }
		  }

		  PicSize=(double)nTemp;//这是图像像素总数
        //下面计算累积直方图
		  for(i=0;i<256;i++)
		  {
			  nTemp=0;
			  for(j=0;j<=i;j++)
			  {
				  nTemp+=temp[j];//累加灰度数,这里加的量一定要注意
			  }
			  //计算新对应的新灰度值
			  ptemp=(double)(0.5+255.0*nTemp/PicSize);
			  k=(unsigned char)(ptemp);//先将其转为整数值
			  ByteMap[i]=k;//再将其转为
		  }
         return ByteMap;
		}

	
	//下面给出的是由姚若河提供的一个算法
	unsigned char * YAO_JIA_Equal()
		{
			//由于已知道了图像的直方图，所以直接进行修正
			int i,j;
			long nTemp;
			int k;
			double temp;
			double PicSize;//这是图像的像素总数
			PicSize=(double)(PicWidth*PicHeight);
			
			//计算累计直方图
			for(i=0;i<255;i++)
			{
				nTemp=0;
				for(j=0;j<i;j++)
				{
					nTemp+=Histo[j];//累加灰度数,这里加的量一定要注意
				}
				//计算新对应的新灰度值
				temp=(double)(0.5+255.0*nTemp/(PicSize-Histo[i]));
				k=(unsigned char)(temp);//先将其转为整数值
				ByteMap[i]=k;//再将其转为
			}
			return ByteMap;//返回灰度映射表				  
		}
		
		
		//下面是综合上面两个算法而作的一个算法
		unsigned char * TY_JIA_Equal(double Mvalue)
		{
			int i,j,k;
			long nTemp;
			
			double ptemp,PicSize;
			
			//下面这个区域是用来计算陈的
			unsigned char * ByteTemp=new unsigned char[256];
			int Sum;
           //
			int * temp=new int[256];//存贮新的灰度频数
			//unsigned char * byteTemp=new unsigned char[256];//暂存
			nTemp=0;
			for(i=0;i<256;i++)
			{//重新修正
				temp[i]=0;//先对
				if(Histo[i]!=0)
				{
					temp[i]=(int)(exp(Mvalue*log((double)(Histo[i]))));//这是修正
					nTemp+=temp[i];//计算新的像素总数
				}
			}
			PicSize=(double)nTemp;//这是图像像素总数
			//下面计算累积直方图
			//对第一个采用直接赋值
			//
	       ByteMap[0]=(unsigned char)0;
			for(i=1;i<255;i++)
			{
				nTemp=0;
				for(j=0;j<i;j++)
				{
					nTemp+=temp[j];//累加灰度数,这里加的量一定要注意
				}
				//计算新对应的新灰度值
				ptemp=(double)(0.5+255.0*nTemp/(PicSize-temp[i]));
				//ptemp=(double)(255.0*nTemp/PicSize);
				k=(unsigned char)(ptemp);//先将其转为整数值
				ByteMap[i]=k;//再将其转为
			}
			ByteMap[255]=(unsigned char)255;
			//下面的这个内容是由陈钱提供的，表现了将灰度再等间距排列			
			for(i=0;i<256;i++)
			{
				ByteTemp[i]=0;//存贮灰度的频数
			}
			//存贮
			for(i=0;i<256;i++)
			{
				if(Histo[i]!=0)
				{//证明变换之后的灰度是有效，但这样的灰度可能是简并的。
					ByteTemp[ByteMap[i]]++;
				}
			}
			//
			Sum=0;//存贮的是有效灰度级总数
			for(i=0;i<256;i++)
			{
				if(ByteTemp[i]!=0)
					Sum++;
			}
			PicSize=(double)Sum;
			//现在进行累加
			for(i=0;i<256;i++)
			{
				nTemp=0;
				for(j=0;j<=ByteMap[i];j++)
				{//这里的循环设计是极妙的，因为是对ByteMap[i]而言的
				//序列才是有效的灰度级所有的范围
					if(ByteTemp[j]!=0)
						nTemp++;
				}
				ptemp=(double)(0.5+255.0*(nTemp-1.0)/(PicSize-1.0));
				k=(unsigned char)ptemp;
				ByteMap[i]=k;
			}

			return ByteMap;
		}

		
		//下面这个程序是将陈钱的问题时行重新考虑
		unsigned char * Chen()
		{
			//byte[] myMap;//声明一个数组进行接收
			unsigned char * byteTemp=new unsigned char[256];
			int i,j,nTemp,Sum;
			double ptemp;
			unsigned char k;
			//先得到原先的灰度映射表
			//myMap=TY_JIA_Equal();
			//下面进行统计
			
			for(i=0;i<256;i++)
			{
				byteTemp[i]=0;//存贮灰度的频数
			}
			//存贮
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
			//现在进行累加
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
			//现在赋回去
			return ByteMap;
		}
		

		/*下面的算法程序是由杨必武提供的基于直方图的
		//灰度非线性拉伸算法
		unsigned char * NonLinear()
		{
            //先求出原先图像的像素值的最小值
			
			int i,KValue;
			
		
			//第一步先求出这个区间的总像素数
			Sum=0;
			for(i=81;i<149;i++)
				Sum+=Hist[i];
			Sum+=((Hist[80]+Hist[150])/2);
			//下面先得到一个
			KValue=(int)(60+60*Hist[80]/Sum);
			ByteMap[80]=(byte)KValue;
			//下面进行下一步灰度拉伸
			for(i=81;i<150;i++)
			{
				KValue+=(int)(60*(Hist[i-1]+Hist[i])/Sum);
				ByteMap[i]=(byte)KValue;
			}
			//下面对灰度重新映射
            //先记录第一个最小的值
			ByteMap[MinValue]=(byte)(128*Hist[MinValue]/SumOfPic);
			KValue=ByteMap[MinValue];//先记录下来
			for(i=MinValue+1;i<MaxValue;i++)
			{
				KValue+=Convert.ToInt32((128*(Hist[i-1]+Hist[i]))/SumOfPic);//将新的一个分区数加入
				ByteMap[i]=(byte)KValue;
			}
			ByteMap[MaxValue]=(byte)255;//直接赋值
			//下面再进行均衡
			//下面的这个内容是由陈钱提供的，表现了将灰度再等间距排列
			nTemp=0;
			//先计算灰度累积值
			for(i=0;i<256;i++)
			{
				if(Hist[i]!=0)
					nTemp+=ByteMap[i];
			}
			//计算完之后再
			PicSize=nTemp;
			//下面再进行重新直方图均衡
			for(i=0;i<256;i++)
			{
				nTemp=0;
				for(j=0;j<=i;j++)
				{
					if(Hist[j]!=0)
						nTemp+=ByteMap[j];
				}
				//下面进行重新赋值
				ptemp=(double)(MinValue+0.5+(255.0-MinValue)*nTemp/PicSize);
				k=(int)ptemp;
				byteTemp[i]=(byte)k;
			}
			//下面在赋回去
			for(i=0;i<256;i++)
				ByteMap[i]=byteTemp[i];
            return ByteMap;
		}*/
      



private:
   int PicWidth;//图像的宽，列数
   int PicHeight;//图像的高，行数
   int **PicData;//这个二维数组的指针用来存储灰度图的数据
   int Histo[256];//直方图的数据,存储各灰度级的个数
   unsigned char ByteMap[256];//灰度映射表

   //
};

#endif

