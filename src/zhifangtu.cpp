#include "zhifangtu.h"

Zhifangtu::Zhifangtu()
{
    //��ʼ���㷨����
    arithName = "ֱ��ͼ���⻯"; 
}

void Zhifangtu::setParam(const QList<QImage> &dataList, 
                         const QList<QString> &paramList)
{
    //��ʼ������
    sourceImage = dataList.first();  
  
    //��ʼ������
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
    int *zhifangtu=new int[256];//�洢���Ҷȼ��ĸ���
    unsigned char *ByteMap=new unsigned char[256];//�洢ֱ��ͼ�����ĸ����Ҷ�
    
    int h=sourceImage.height();
    int w=sourceImage.width();
    //�����ά����
    int ** imageData;
    imageData=new int*[h];
    for(i=0;i<h;i++)
        imageData[i]=new int[w];
    for(i=0;i<h;i++)
    {
        for(j=0;j<w;j++)
        {//ע������Ĵ�������ԻҶ�ͼ�ģ����ֻ��Ҫȡһ���������ɣ�
            //���ڲ�ɫͼ����Ҫ�ȻҶȻ�����
            imageData[i][j]=qRed(sourceImage.pixel(j,i));//�õ�һ��ͨ����������
        }
    }
    Histogram myHist(h,w,imageData);//����һ��ֱ��ͼ��
    zhifangtu = myHist.Get_Hist ();//�õ�ֱ��ͼ������
    
    ByteMap=myHist.Equal_Hist(EqualMod,MValue);//���������Ĳ������룬Ȼ����о��⻯����
    for(i=0;i<h;i++)
    {
        for(j=0;j<w;j++)
        {
            temp=(sourceImage.pixel(j,i) >> 16) & 0xff;//ԭʼͼ��ĻҶ�ֵ
            image.setPixel(j,i,qRgb(ByteMap[temp],ByteMap[temp],ByteMap[temp]));//�ҶȾ�������ʾ
        }
    }
    
    finalDataList.push_back(image); 
}
