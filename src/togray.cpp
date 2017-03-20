#include "togray.h"

ToGray::ToGray()
{
    //��ʼ���㷨����
    arithName = "ͼ��ҶȻ�"; 
}

void ToGray::setParam(const QList<QImage> &dataList, 
                      const QList<QString> &paramList)
{    
    //��ʼ������
    sourceImage = dataList.first(); 
    
    //��ʼ������
}

void ToGray::dealData()
{
    QImage image(sourceImage.size(), sourceImage.format());
    
    int height = image.height();
    int width = image.width();
    for ( int w = 0; w != width; ++w)
    {
        for ( int h = 0; h != height; ++h)
        {
            int pixel = sourceImage.pixel(w,h);
            int gray = qGray( pixel);
            gray = gray + (gray<<8) + (gray<<16) + (qAlpha(pixel)<<24);
            image.setPixel(w, h, gray); 
        }
    }
    emit 
       
    finalDataList.push_back(image); 
}
