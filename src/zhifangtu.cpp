#include "zhifangtu.h"

Zhifangtu::Zhifangtu()
{
    //初始化算法名称
    arithName = "直方图均衡化"; 
}

void Zhifangtu::setParam(const QList<QImage> &dataList, 
                         const QList<QString> &paramList)
{
    //初始化数据
    sourceImage = dataList.first();  
  
    //初始化参数
    QList<QString>::const_iterator paramIter; 
    paramIter = paramList.begin(); 
    EqualMod = paramIter->toInt(); 
    paramIter++; 
    MValue = paramIter->toDouble(); 
}

void Zhifangtu::dealData()
{   
    QImage image = sourceImage; 
    
    int temp;
    int i,j;
    int *zhifangtu=new int[256];//存储各灰度级的个数
    unsigned char *ByteMap=new unsigned char[256];//存储直方图均衡后的各级灰度
    
    int h=sourceImage.height();
    int w=sourceImage.width();
    //申请二维数组
    int ** imageData;
    imageData=new int*[h];
    for(i=0;i<h;i++)
        imageData[i]=new int[w];
    for(i=0;i<h;i++)
    {
        for(j=0;j<w;j++)
        {//注意这里的处理是针对灰度图的，因此只需要取一个分量即可，
            //对于彩色图像，需要先灰度化处理
            imageData[i][j]=qRed(sourceImage.pixel(j,i));//得到一个通道分量即可
        }
    }
    Histogram myHist(h,w,imageData);//声明一个直方图类
    zhifangtu = myHist.Get_Hist ();//得到直方图的数据
    
    ByteMap=myHist.Equal_Hist(EqualMod,MValue);//把外界输入的参数传入，然后进行均衡化处理。
    for(i=0;i<h;i++)
    {
        for(j=0;j<w;j++)
        {
            temp=(sourceImage.pixel(j,i) >> 16) & 0xff;//原始图像的灰度值
            image.setPixel(j,i,qRgb(ByteMap[temp],ByteMap[temp],ByteMap[temp]));//灰度均衡后的显示
        }
    }
    
    finalDataList.push_back(image); 
}
